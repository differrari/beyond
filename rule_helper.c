#include "rules.h"

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
        case sem_var: return "variable";
        case sem_func: return "function";
        case sem_jmp: return "jump";
        case sem_scope: return "scope";
        case sem_val: return "value";
        case sem_cond: return "condition";
        case sem_call: return "call";
        case sem_args: return "argument";
        case sem_dec: return "declaration"; 
        case sem_assign: return "assign"; 
        case sem_exp: return "expression"; 
        case sem_label: return "label"; 
        case sem_op: return "operand"; 
        case sem_stat: return "";
        case sem_param: return "parameter";
        case sem_for: return "for";
        case sem_while: return "while";
        case sem_dowhile: return "do while";
        case sem_inc: return "include";
        case sem_struct: return "struct";
        case sem_ret: return "return";
        case sem_def: return "defer";
        case sem_interf: return "interface";
        case sem_none: 
        case sem_rules_count: return "";
    }
}

char *get_sem_elem_name(semantic_elements elem){
    switch (elem){
        case sem_elem_type: return "type";
        case sem_elem_name: return "name";
        default: return "";
    }
}