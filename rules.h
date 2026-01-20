#pragma once

#include "types.h"
#include "data/tokenizer/tokenizer.h"

typedef enum { sem_none, sem_dec, sem_stat, sem_var, sem_assign, sem_exp, sem_func, sem_label, sem_jmp, sem_scope, sem_param, sem_val, sem_op, sem_cond, sem_call, sem_args, sem_for, sem_while, sem_dowhile, sem_inc, sem_struct, sem_rules_count, sem_ret, sem_def, sem_interf, sem_enum, sem_enum_case, sem_parent } semantic_rules;

typedef enum { sem_elem_none, sem_elem_type, sem_elem_name, sem_elem_parent, sem_elem_count } semantic_elements;

typedef enum { sem_action_none, sem_action_declare, sem_action_check } semantic_action;

typedef struct {
    bool rule;
    int value;
    char *lit;
    semantic_action action;
    int sem_value;
} grammar_elem;

typedef struct {
    grammar_elem rules[64];
    uint8_t num_elements;
} grammar_rule_opt;

typedef struct {
    grammar_rule_opt options[64];
    uint8_t num_elements;
    semantic_rules action;
} grammar_rule;

#define TOKEN(name) { false, TOK_##name, 0, sem_action_none, 0 }
#define RULE(name) { true, rule_##name, 0, sem_action_none, 0 }
#define LITERAL(val) { false, TOK_IDENTIFIER, val, sem_action_none, 0 }
#define LITTOK(tok,val) { false, TOK_##tok, val, sem_action_none, 0 }

#define SYMDEC(name,type) { false, TOK_##name, 0, sem_action_declare, sem_elem_##type }
#define SYMCHECK(name,type) { false, TOK_##name, 0, sem_action_check, sem_##type }
#define SYMRULE(name,type) { true, rule_##name, 0, sem_action_check, sem_##type }

extern grammar_rule language_rules[];

extern char* rule_names[];

char *tok_symbol(TokenKind kind);
char *sem_action_name(semantic_action act);
char *sem_rule_name(semantic_rules rule);
char *get_sem_elem_name(semantic_elements elem);