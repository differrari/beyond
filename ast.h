#pragma once

#include "data/tokenizer/tokenizer.h"
#include "rules.h"

typedef struct {
    Token t;
    grammar_rules rule;
    int option;
    int depth;
    int sequence;
} ast_node;