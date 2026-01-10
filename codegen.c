#include "codegen.h"
#include "common.h"

#define MAX_CHARS 10000
char buf[MAX_CHARS];

uintptr_t cursor;
uintptr_t stack_cursor = 0;
int curr_indent = 0;
ast_node *ast_stack;
uint32_t stack_count;

char* rule_name(grammar_rules rule){
    switch (rule) {
        case rule_block: return "block";
        case rule_statement: return "statement";
        case rule_declaration: return "declaration";
        case rule_assignment: return "assignment";
        case rule_expression: return "expression";
        case rule_funccall: return "funccall";
        case rule_argument: return "argument";
        case rule_conditional: return "conditional";
        case rule_jump: return "jump";
        case rule_label: return "label";
        default: return "UNK";
    }
}

char *tok_symbol(TokenKind kind){
    switch (kind) {
        case TOK_LPAREN: return "(";
        case TOK_RPAREN: return ")";
        case TOK_LBRACE: return "{";
        case TOK_RBRACE: return "}";
        case TOK_LBRACKET: return "[";
        case TOK_RBRACKET: return "]";
        case TOK_COMMA: return ",";
        case TOK_COLON: return ":\n";
        case TOK_SEMICOLON: return ";\n";
        case TOK_DOT: return ".";
        case TOK_NEWLINE: return ";\n";
      default: return "";
    }
}

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

bool pop_stack(ast_node* node){
    if (stack_cursor >= stack_count) return false;
    *node = ast_stack[stack_cursor++];
    return true;
}

bool switch_rule(grammar_rules *current_rule, int *curr_option){
    ast_node node;
    if (!pop_stack(&node)) return false;
    if (node.t.kind){
        print("Found token instead of rule");
        return false;
    }
    *current_rule = node.rule;
    *curr_option = node.option;
    return true;
}

void eval_rule(grammar_rules current_rule, int curr_option){
    print("Current rule %s@%i",rule_name(current_rule),curr_option);
    for (int s = 0; s < language_rules[current_rule].options[curr_option].num_elements; s++){
        grammar_elem elem = language_rules[current_rule].options[curr_option].rules[s];
        if (elem.rule){
            grammar_rules new_rule;
            int new_opt;
            if (!switch_rule(&new_rule, &new_opt)){
                print("Rule not found %i",elem.value);
                return;
            }
            eval_rule(new_rule, new_opt);
        } else {
            print("Token@%i",s);
            if (elem.value == TOK_IDENTIFIER || elem.value == TOK_STRING || elem.value == TOK_CONST || elem.value == TOK_OPERATOR){
                if (elem.lit){
                    output_literal(elem.lit, true);
                } else {
                    ast_node node;
                    pop_stack(&node);
                    if (node.t.kind != elem.value){
                        print("Wrong token found. Expected %i, found %i (%v)",elem.value, node.t.kind, token_to_slice(node.t));
                        return;
                    }
                    output_token(node.t);
                }
            } else {
                output_literal(tok_symbol(elem.value), false);
            }
        }
    }
    print("Finished rule %s@%i",rule_name(current_rule),curr_option);
}

void gen_code(ast_node *stack, uint32_t count){
    ast_stack = stack;
    stack_count = count;
    // output_debug_rule(current_rule);
    ast_node node = {};
    grammar_rules new_rule;
    int new_opt;
    switch_rule(&new_rule,&new_opt);
    eval_rule(new_rule,new_opt);
    print(buf);
    print_stack(stack, count);
}