#pragma once

#include "rules.h"
#include "data/helpers/token_stream.h"
#include "ast.h"

typedef struct {
    grammar_rules current_rule;
    int option;
    int sequence;
    uint32_t scanner_pos;
    Scanner *scan;
    uint32_t tree_pos;
} parser_sm;

typedef struct {
    bool result;
    uint32_t furthest_parse_pos;
    ast_node* ast_stack;
    uint32_t ast_count;
} parse_result;

parse_result parse(const char *content, TokenStream *ts, parser_sm *parser);