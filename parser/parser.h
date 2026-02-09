#pragma once

#include "data/format/helpers/token_stream.h"
#include "ast.h"

typedef struct {
    int current_rule;
    int option;
    int sequence;
    uint32_t scanner_pos;
    Scanner *scan;
    uint32_t tree_pos;
} parser_sm;

typedef struct {
    grammar_elem expected;
    Token found;
    int rule;
} tok_fail;

typedef struct {
    bool result;
    ast_node* ast_stack;
    uint32_t ast_count;
    tok_fail fail_info;
} parse_result;

parse_result parse(const char *content, TokenStream *ts, parser_sm *parser);