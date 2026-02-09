#pragma once

#include "types.h"
#include "data/format/tokenizer/tokenizer.h"
#include "semantic/semantic_rules.h"

typedef struct {
    bool rule;
    int value;
    char *lit;
    sem_action action;
    int sem_value;
} grammar_elem;

typedef struct {
    grammar_elem rules[64];
    uint8_t num_elements;
} grammar_rule_opt;

typedef struct {
    grammar_rule_opt options[64];
    uint8_t num_elements;
    sem_rule semrule;
    sem_action action;
} grammar_rule;

#define TOKEN(name) { false, TOK_##name, 0, sem_action_none, 0 }
#define RULE(name) { true, rule_##name, 0, sem_action_none, 0 }
#define LITERAL(val) { false, TOK_IDENTIFIER, val, sem_action_none, 0 }
#define LITTOK(tok,val) { false, TOK_##tok, val, sem_action_none, 0 }
#define LITTAG(val,type) { false, TOK_IDENTIFIER, val, sem_action_check, sem_rule_##type }

#define SYMDEC(name,type) { false, TOK_##name, 0, sem_action_declare, sem_elem_##type }
#define SYMCHECK(name,type) { false, TOK_##name, 0, sem_action_check, sem_rule_##type }
#define SYMRULE(name,type) { true, rule_##name, 0, sem_action_check, sem_rule_##type }
#define SYMTOK(name,tag,action,type,val) { false, TOK_##name, val, sem_action_##action, type##_##tag}
extern grammar_rule language_rules[];

extern char* rule_names[];