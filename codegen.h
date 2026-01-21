#pragma once

#include "data/tokenizer/tokenizer.h"
#include "rules.h"

typedef struct codegen_t {
    int type;
    void (*register_elem)(void* ptr, int type, Token elem);
    void (*register_subrule)(void* ptr, int type, struct codegen_t child);
    void (*emit_code)(void* ptr);
    void *ptr;
} codegen_t;

codegen_t begin_rule(sem_rule type);
void register_elem(codegen_t gen, int type, Token elem);
void register_subrule(codegen_t gen, int type, codegen_t child);
void emit_code(codegen_t gen);