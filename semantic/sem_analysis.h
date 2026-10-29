#pragma once

#include "types.h"
#include "data/format/tokenizer/tokenizer.h"
#include "semantic/semantic_rules.h"
#include "data/struct/chunk_array.h"

struct symbol_t;

typedef struct {
    struct symbol_t *symbol_table;
    uint16_t symbol_count; 
    int table_type;
} symbol_table;

typedef struct symbol_t {
    string_slice type;
    string_slice subtype;
    string_slice name;
    sem_rule sym_type;
    symbol_table *child;
    semantic_types resolved_type;
    semantic_types resolved_subtype;
    bool reference_type;
    int table_type;
} symbol_t;

struct codegen;

symbol_table* new_table(sem_rule type);
symbol_table* analyze_semantics(struct codegen ir);
symbol_t * find_symbol(sem_rule type, string_slice name);
string_slice make_temp_name(sem_rule type);