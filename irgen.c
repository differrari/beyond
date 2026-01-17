#include "irgen.h"
#include "common.h"
#include "codegen.h"
#include "codegenrules/codeformat.h"

#define MAX_CHARS 10000
char buf[MAX_CHARS];

uintptr_t cursor;
int curr_indent = 0;

stack_navigator gsn;

void print_stack(ast_node *stack, uint32_t count){
    for (uint32_t i = 0; i < count; i++){
        if (!stack[i].t.kind)
            printf("%c[%i,%i] %s ->",*rule_names[stack[i].rule],stack[i].option,stack[i].sequence,indent_by(stack[i].depth));
        else
            printf("%c[%i,%i] %s %v",*rule_names[stack[i].rule],stack[i].option,stack[i].sequence,indent_by(stack[i].depth),token_to_slice(stack[i].t));
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

codegen_t eval_rule(int current_rule, int curr_option){
    
    semantic_rules statement_type = language_rules[current_rule].action;
    codegen_t gen = begin_rule(statement_type);

    // print("Current rule %s@%i",rule_name(current_rule),curr_option);
    for (int s = 0; s < language_rules[current_rule].options[curr_option].num_elements; s++){
        grammar_elem elem = language_rules[current_rule].options[curr_option].rules[s];
        if (elem.rule){
            int new_rule;
            int new_opt;
            tern res = switch_rule(&gsn, &new_rule, &new_opt);
            if (res != true){
                if (!res){
                    print("Rule not found %i",elem.value);
                    return (codegen_t){};
                } 
                return gen;
            }
            if (elem.sem_value){
                codegen_t new_codegen = eval_rule(new_rule, new_opt);
                if (gen.ptr) register_subrule(gen, elem.sem_value, new_codegen);
                else return new_codegen;
            } else eval_rule(new_rule, new_opt);
        } else if (elem.sem_value && !elem.lit){
            ast_node node;
            pop_stack(&gsn, &node);
            if (node.t.kind != elem.value){
                print("Wrong token found. Expected %i, found %i (%v)",elem.value, node.t.kind, token_to_slice(node.t));
                return (codegen_t){};
            }
            if (gen.ptr && node.sem_value) register_elem(gen, node.action == sem_action_declare ? node.sem_value : node.sem_value, node.t);
        }
    }

    end_rule(statement_type);
    
    return gen;
}

void gen_code(ast_node *stack, uint32_t count, const char *out_name){
    gsn = (stack_navigator){};
    gsn.ast_stack = stack;
    gsn.stack_count = count;
    ast_node node = {};
    int new_rule;
    int new_opt;
    if (!switch_rule(&gsn, &new_rule,&new_opt)) return;
    codegen_t cg = eval_rule(new_rule,new_opt);
    emit_code(cg);
    output_code(out_name);
}