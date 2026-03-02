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
    bool optional;
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

#define TOKEN(name,opt) { false, TOK_##name, 0, sem_action_none, 0, opt }
#define RULE(name,opt) { true, rule_##name, 0, sem_action_none, 0, opt }
#define LITERAL(val,opt) { false, TOK_IDENTIFIER, val, sem_action_none, 0, opt }
#define LITTOK(tok,val,opt) { false, TOK_##tok, val, sem_action_none, 0, opt }
#define LITTAG(val,type,opt) { false, TOK_IDENTIFIER, val, sem_action_check, sem_rule_##type, opt }

#define SYMDEC(name,type,opt) { false, TOK_##name, 0, sem_action_declare, sem_elem_##type, opt }
#define SYMCHECK(name,type,opt) { false, TOK_##name, 0, sem_action_check, sem_rule_##type, opt }
#define SYMRULE(name,type,opt) { true, rule_##name, 0, sem_action_check, sem_rule_##type, opt }
#define SYMTOK(name,tag,action,type,val,opt) { false, TOK_##name, val, sem_action_##action, type##_##tag, opt }
extern grammar_rule language_rules[];

extern char* rule_names[];