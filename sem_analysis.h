#pragma once

#include "ast.h"

struct symbol_t;

typedef struct {
    struct symbol_t *symbol_table;
    uint16_t symbol_count; 
} symbol_table;

typedef struct symbol_t {
    Token type;
    string_slice name;
    sem_rule sym_type;
    symbol_table *child;
    semantic_types resolved_type;
    bool reference_type;
} symbol_t;

bool analyze_semantics(ast_node *stack, uint32_t count);
symbol_t * find_symbol(sem_rule type, string_slice name);