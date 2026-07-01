#pragma once

#include "data/format/tokenizer/tokenizer.h"
#include "semantic/semantic_rules.h"
#include "data/struct/chunk_array.h"

typedef struct {
    Token t;
    int rule;
    int option;
    int depth;
    int sequence;
    sem_action action;
    int sem_value;
} ast_node;

typedef struct {
    uintptr_t cursor;
    chunk_array_t *stack;
    size_t size;
} stack_navigator;

void push_node(chunk_array_t *stack, ast_node node);

bool pop_stack(stack_navigator *sn, ast_node* node);

tern switch_rule(stack_navigator *sn, int *current_rule, int *curr_option);

chunk_array_t* init_ast();

size_t tree_count(chunk_array_t *stack);

void tree_reset(chunk_array_t *tree, size_t to);

void print_stack(chunk_array_t *stack);

static inline stack_navigator make_stack_navigator(chunk_array_t *stack){
    return (stack_navigator){
        .stack = stack,
        .size = tree_count(stack),
        .cursor = 0,
    };
}