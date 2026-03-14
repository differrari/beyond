#include "irgen.h"
#include "common.h"
#include "codegen/code_generator.h"
#include "parser/ast.h"

#define MAX_CHARS 10000
char buf[MAX_CHARS];

uintptr_t cursor;
int curr_indent = 0;

stack_navigator gsn;

void print_stack(ast_node *stack, uint32_t count){
    for (uint32_t i = 0; i < count; i++){
        if (!stack[i].t.kind)
            print("%c[%i,%i] %s ->",*rule_names[stack[i].rule],stack[i].option,stack[i].sequence,indent_by(stack[i].depth));
        else
            print("%c[%i,%i] %s %v",*rule_names[stack[i].rule],stack[i].option,stack[i].sequence,indent_by(stack[i].depth),token_to_slice(stack[i].t));
    }
}

void output_newline(){
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "\n%s", indent_by(curr_indent));
}

void output_debug_rule(int rule){
    output_newline();
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "Current rule %s", rule_names[rule]);
}

void output_token(Token t){
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "%v ", token_to_slice(t));
}

void output_literal(char* lit, bool space){
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "%s", lit);
    if (space) {
        *(buf+cursor) = ' ';
        cursor++;
    }
}

codegen eval_rule(int current_rule, int curr_option){
    
    sem_rule statement_type = language_rules[current_rule].semrule;
    codegen gen = begin_rule(statement_type);

    // print("Current rule %s@%i",rule_name(current_rule),curr_option);
    bool terminator = false;
    for (int s = 0; s < language_rules[current_rule].options[curr_option].num_elements; s++){
        grammar_elem elem = language_rules[current_rule].options[curr_option].rules[s];
        // print("Now checking %s(%s) inside %s",elem.action == sem_action_check ? sem_rule_strings[elem.sem_value] : sem_elem_strings[elem.sem_value],rule_names[elem.value],rule_names[current_rule]);
        ast_node node;
        if (!pop_stack(&gsn, &node)){
            // print("End of tree");
            break;
        }
        if (node.terminator){
            if (node.rule == current_rule){
                // print("Terminator for %s",rule_names[node.rule]);
                terminator = true;
                break;
            } else {
                // print("Unrelated terminator for %s",rule_names[node.rule]);
                gsn.stack_cursor--;
                continue;
            }
        }
        if (elem.rule){
            if (node.rule != elem.value && elem.optional){
                // print("Optional rule %s@%s skipped since %s was found instead",rule_names[elem.value],rule_names[current_rule],rule_names[node.rule]);
                gsn.stack_cursor--;
                continue;
            }
            // print("Registering subrule %s as part of %s",sem_rule_strings[elem.sem_value],sem_rule_strings[statement_type]);
            codegen new_codegen = eval_rule(node.rule, node.option);
            if (gen.ptr) codegen_register_subrule(gen, elem.sem_value, new_codegen);
            else {
              gen = new_codegen;
              break;  
            }
        } else {
            if (!elem.sem_value){
                gsn.stack_cursor--;
                continue;
            }
            // print("Found tokn %v %i vs %i %i vs %i",token_to_slice(node.t),node.sequence,s);
            if (node.t.kind != elem.value || node.sequence != s){
                if (elem.optional){
                    // print("Optional %s inside %s skipped since we found %s instead",elem.action == sem_action_check ? sem_rule_strings[elem.sem_value] : sem_elem_strings[elem.sem_value],rule_names[current_rule],sem_rule_strings[node.sem_value]);
                    gsn.stack_cursor--;
                    continue;
                }
                // print("Wrong token found. Expected %i, found %i on %i (%v). Has rule %s", elem.value, node.t.kind, node.rule, token_to_slice(node.t),rule_names[node.rule]);
                gen = (codegen){};
                break;  
            }
            if (gen.ptr){
                // print("Registering token %v as part of %s",token_to_slice(node.t),rule_names[statement_type]);
                codegen_register_elem(gen, node.sem_value, node.t);
            }
        }
    }
    if (!terminator){
        ast_node node;
        if (pop_stack(&gsn, &node)){
            if (!node.terminator || node.rule != current_rule){
                // print("Found wrong for %s instead of %s. Terminator? %i",rule_names[node.rule],rule_names[current_rule],node.terminator);
                gsn.stack_cursor--;
            }
        }
    }
    return gen;
}

codegen gen_code(chunk_array_t *stack, const char *out_name){
    gsn = (stack_navigator){};
    gsn.stack = stack;
    ast_node node = {};
    int new_rule;
    int new_opt;
    if (!switch_rule(&gsn, &new_rule,&new_opt)) return (codegen){};
    return eval_rule(new_rule,new_opt);
}