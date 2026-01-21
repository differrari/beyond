#pragma once

#include "data/tokenizer/tokenizer.h"
#include "rules.h"

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
    uintptr_t stack_cursor;
    ast_node *ast_stack;
    uint32_t stack_count;
} stack_navigator;

bool pop_stack(stack_navigator *sn, ast_node* node);

tern switch_rule(stack_navigator *sn, int *current_rule, int *curr_option);