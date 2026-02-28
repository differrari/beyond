#pragma once

#include "data/format/helpers/token_stream.h"
#include "rules.h"
#include "data/struct/chunk_array.h"

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
    chunk_array_t *ast_stack;
    tok_fail fail_info;
} parse_result;

parse_result parse(const char *content, TokenStream *ts, parser_sm *parser);