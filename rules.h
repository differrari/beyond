#pragma once

#include "types.h"
#include "data/tokenizer/tokenizer.h"

typedef enum { rule_block, rule_statement, rule_declaration, rule_assignment, rule_expression, rule_funccall, rule_argument, rule_conditional, rule_jump, rule_label, num_grammar_rules } grammar_rules;

typedef struct {
    bool rule;
    int value;
    char *lit;
} grammar_elem;

typedef struct {
    grammar_elem rules[16];
    uint8_t num_elements;
} grammar_rule_opt;

typedef struct {
    grammar_rule_opt options[8];
    uint8_t num_elements;
} grammar_rule;

#define TOKEN(name) { false, TOK_##name, 0 }
#define RULE(name) { true, rule_##name, 0 }
#define LITERAL(val) { false, TOK_IDENTIFIER, val }
#define LITTOK(tok,val) { false, TOK_##tok, val }

extern grammar_rule language_rules[num_grammar_rules];