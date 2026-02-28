#pragma once

#include "parser/ast.h"

struct symbol_t;

typedef struct {
    struct symbol_t *symbol_table;
    uint16_t symbol_count; 
    int table_type;
} symbol_table;

typedef struct symbol_t {
    Token type;
    Token subtype;
    string_slice name;
    sem_rule sym_type;
    symbol_table *child;
    semantic_types resolved_type;
    semantic_types resolved_subtype;
    bool reference_type;
    int table_type;
} symbol_t;

symbol_table* new_table();
symbol_table* analyze_semantics(chunk_array_t *stack);
symbol_t * find_symbol(sem_rule type, string_slice name);
string_slice make_temp_name(sem_rule type);