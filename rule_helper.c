#include "rules.h"

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

char *sem_action_name(semantic_action kind){
    switch (kind) {
        case sem_action_none: return "none";
        case sem_action_declare: return "declare";
        case sem_action_check: return "check";
        default: return "";
    }
}

char *sem_rule_name(semantic_rules rule){
    switch (rule){
        case sem_var: return "var";
        case sem_func: return "func";
        case sem_jmp: return "jmp";
        case sem_scope: return "scope";
        case sem_val: return "val";
        case sem_cond: return "condition";
        case sem_call: return "call";
        case sem_args: return "arg";
        default: return "";
    }
}

char *get_sem_elem_name(semantic_elements elem){
    switch (elem){
        case sem_elem_type: return "type";
        case sem_elem_name: return "name";
        default: return "";
    }
}