#pragma once

#include "rules.h"
#include "data/helpers/token_stream.h"

typedef struct {
    grammar_rules current_rule;
    int option;
    int sequence;
    uint32_t scanner_pos;
    Scanner *scan;
    uint32_t tree_pos;
} parser_sm;

typedef struct {
    Token t;
    grammar_rules rule;
    int option;
    int depth;
} ast_node;

typedef struct {
    bool result;
    uint32_t furthest_parse_pos;
    ast_node* ast_stack;
    uint32_t ast_count;
} parse_result;

parse_result parse(const char *content, TokenStream *ts, parser_sm *parser);