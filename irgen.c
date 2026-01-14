#include "irgen.h"
#include "common.h"

#define MAX_CHARS 10000
char buf[MAX_CHARS];

uintptr_t cursor;
int curr_indent = 0;

stack_navigator gsn;

// scope =>
// assign => var val
// call => func args
// cond => cond scope
// dec => type name val
// jmp => jmp
// label => name
// val => val op exp | var op exp | val | var

void print_stack(ast_node *stack, uint32_t count){
    for (uint32_t i = 0; i < count; i++){
        if (!stack[i].t.kind)
            printf("%c[%i,%i] %s ->",*rule_name(stack[i].rule),stack[i].option,stack[i].sequence,indent_by(stack[i].depth));
        else
            printf("%c[%i,%i] %s %v",*rule_name(stack[i].rule),stack[i].option,stack[i].sequence,indent_by(stack[i].depth),token_to_slice(stack[i].t));
    }
}

void output_newline(){
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "\n%s", indent_by(curr_indent));
}

void output_debug_rule(grammar_rules rule){
    output_newline();
    cursor += string_format_buf((buf+cursor), MAX_CHARS, "Current rule %s", rule_name(rule));
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

void eval_rule(grammar_rules current_rule, int curr_option){
    // print("Current rule %s@%i",rule_name(current_rule),curr_option);
    
    semantic_rules statement_type = language_rules[current_rule].action;
    bool gen = true;
    
    switch (current_rule){
        // case rule_block: print("+SCOPE");
        case rule_declaration: print("DECLARATION"); break;
        case rule_assignment: print("ASSIGNMENT"); break;
        case rule_expression: print("EXPRESSION"); break;
        case rule_funccall: print("FUNCCALL"); break;
        case rule_argument: print("ARGUMENT"); break;
        case rule_conditional: print("CONDITIONAL"); break;
        case rule_jump: print("JUMP"); break;
        case rule_label: print("LABEL"); break;
        default: gen = false; break;
    }

    for (int s = 0; s < language_rules[current_rule].options[curr_option].num_elements; s++){
        grammar_elem elem = language_rules[current_rule].options[curr_option].rules[s];
        if (elem.rule){
            grammar_rules new_rule;
            int new_opt;
            tern res = switch_rule(&gsn, &new_rule, &new_opt);
            if (res != true){
                if (!res) print("Rule not found %i",elem.value);
                return;
            }
            if (elem.sem_value){
                print("Begin %s",sem_rule_name(elem.sem_value));
                eval_rule(new_rule, new_opt);
                print("End %s",sem_rule_name(elem.sem_value));
            } else eval_rule(new_rule, new_opt);
        } else {
            if (elem.value == TOK_IDENTIFIER || elem.value == TOK_STRING || elem.value == TOK_CONST || elem.value == TOK_OPERATOR){
                if (!elem.lit){
                    ast_node node;
                    pop_stack(&gsn, &node);
                    if (node.t.kind != elem.value){
                        print("Wrong token found. Expected %i, found %i (%v)",elem.value, node.t.kind, token_to_slice(node.t));
                        return;
                    }
                    if (gen){
                        if (elem.sem_value) print("%v",token_to_slice(node.t));
                    }
                }
            }
        }
    }
    // print("Finished rule %s@%i",rule_name(current_rule),curr_option);
}

void gen_code(ast_node *stack, uint32_t count){
    gsn = (stack_navigator){};
    gsn.ast_stack = stack;
    gsn.stack_count = count;
    // output_debug_rule(current_rule);
    ast_node node = {};
    grammar_rules new_rule;
    int new_opt;
    if (!switch_rule(&gsn, &new_rule,&new_opt)) return;
    eval_rule(new_rule,new_opt);
    print(buf);
    // print_stack(stack, count);
}