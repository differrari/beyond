#include "common.h"
#include "parser.h"

#define MAX_TREE_COUNT 1024

ast_node tree_stack[MAX_TREE_COUNT];
uint32_t tree_count = 0;

parser_sm parser_stack[MAX_DEPTH];
int parser_depth;

// #define PRINT_PARSE
#ifdef PRINT_PARSE
#define parser_debug(...) print(__VA_ARGS__)
#else
#define parser_debug(...)
#endif

#define curr_indent indent_by(parser_depth)

#define current_parser_rule(parser) language_rules[parser->current_rule].options[parser->option].rules[parser->sequence]

bool emit_token(Token t, grammar_rules rule, int option, int sequence, grammar_elem element){
    if (element.lit || (t.kind != TOK_IDENTIFIER && t.kind != TOK_STRING && t.kind != TOK_CONST && t.kind != TOK_OPERATOR)) return true;
    if (tree_count >= MAX_TREE_COUNT-1){
        print("Too much code");
        return false;
    }
    tree_stack[tree_count++] = (ast_node){
        .t = t,
        .rule = rule,
        .option = option,
        .depth = parser_depth,
        .sequence = sequence,
    };
    return true;
}

bool emit_rule(grammar_rules rule, int option, int sequence){
    if (tree_count >= MAX_TREE_COUNT-1){
        print("Too much code");
        return false;
    }
    tree_stack[tree_count++] = (ast_node){
        .t = {},
        .rule = rule,
        .option = option,
        .depth = parser_depth,
        .sequence = sequence,
    };
    return true;
}

size_t furthest_pos = 0;
uint32_t tok_pos = 0;

bool parser_advance_to_token(parser_sm *parser, Token t){
    while (current_parser_rule(parser).rule){
        parser_debug("%s[%i] %s[o:%i] subrule %s @%i",curr_indent, parser->sequence, rule_name(parser->current_rule), parser->option, rule_name(current_parser_rule(parser).value),parser->scan->pos);
        if (parser_depth == MAX_DEPTH - 1){
            parser_debug("Maximum depth reached, too many nested statements, shame on you");
            return false;
        }
        // parser_debug("Push state with pos %i",tok_pos);
        parser_stack[parser_depth++] = *parser;
        parser->scanner_pos = tok_pos;
        parser->tree_pos = tree_count;
        parser->current_rule = current_parser_rule(parser).value;
        parser->sequence = 0;
        parser->option = 0;
        if (!emit_rule(parser->current_rule,parser->option, parser->sequence)) return false;
    }
    // parser_debug("%sCurrent rule %i. Option %i. Sequence %i",curr_indent, parser->current_rule, parser->option, parser->sequence);
    return true;
}

bool pop_parser_stack(parser_sm *parser, bool backtrack){
    if (parser_depth == 0){
        // parser_debug("Cannot pop root state");
        return false;
    }
    // parser_debug("Pop state");
    *parser = parser_stack[parser_depth-1];
    if (backtrack){
        parser_debug("%sSubrule failed, backtrack to %i",curr_indent, parser->scanner_pos);
        if (parser->scan->pos > furthest_pos) furthest_pos = parser->scan->pos;
        parser->scan->pos = parser->scanner_pos;
        tok_pos = parser->scanner_pos;
        tree_count = parser->tree_pos;
    }
    parser_depth--;
    return true;
}

bool parser_advance_option_sm(parser_sm *parser){
    if (parser->option + 1 == language_rules[parser->current_rule].num_elements){
        parser_debug("%sRule %s failed",curr_indent,rule_name(parser->current_rule));
        if (!pop_parser_stack(parser, true)) return false;
        return parser_advance_option_sm(parser);
    } else {
        parser_debug("%sRule %s option %i failed",curr_indent,rule_name(parser->current_rule), parser->option);
        parser->option++;
        parser->sequence = 0;
        parser_debug("%soption failed, backtrack to %i",curr_indent, parser->scanner_pos);
        parser_debug("%s%s [o:%i]",curr_indent,rule_name(parser->current_rule), parser->option);
        if (parser->scan->pos > furthest_pos) furthest_pos = parser->scan->pos;
        parser->scan->pos = parser->scanner_pos;
        tok_pos = parser->scanner_pos;
        tree_count = parser->tree_pos;
        if (!emit_rule(parser->current_rule,parser->option, parser->sequence)) return false;
        return true;
    }
}

bool parser_advance_sequence(parser_sm *parser){
    if (parser->sequence + 1 == language_rules[parser->current_rule].options[parser->option].num_elements){
        // parser_debug("Successfully parsed rule %i with option %i",parser->current_rule, parser->option);
        if (!pop_parser_stack(parser, false)) return true;
        return parser_advance_sequence(parser);
    }
    else parser->sequence++;
    return true;
}

bool parse_token(const char *content, Token t, parser_sm *parser){
    parser_advance_to_token(parser, t);
    // parser_debug("Evaluating token at %i",t.pos);
    grammar_elem element = current_parser_rule(parser);
    if (t.kind == element.value && (!element.lit || slice_lit_match(token_to_slice(t), element.lit, false))){
        parser_debug("%s[%i] Parsed token %v [%s@%i]",curr_indent, parser->sequence, make_string_slice(content, t.pos, t.length), token_name(t.kind), parser->scan->pos);
        tok_pos = parser->scan->pos;
        if (!emit_token(t, parser->current_rule, parser->option, parser->sequence, element)) return false;
        return parser_advance_sequence(parser);
    } else {
        parser_debug("%s[%i] Failed to match token %s, found %s@%i. Skipping",curr_indent, parser->sequence, token_name(element.value), token_name(t.kind),parser->scan->pos);
        return parser_advance_option_sm(parser);
    }
}

parse_result parse(const char *content, TokenStream *ts, parser_sm *parser){
    Token t;
    bool result;
    if (!emit_rule(parser->current_rule,parser->option, parser->sequence)) return (parse_result){ .result = false, .furthest_parse_pos = furthest_pos };
    while (ts_next(ts, &t)) {
        // print("Read token@%i",parser->scan->pos);
        if (!t.kind)
            break;
        result = parse_token(content, t, parser);
        if (!result) break;
        // msleep(1000);
    }
    
    if (ts->tz->err || !result)
        return (parse_result){ .result = false, .furthest_parse_pos = furthest_pos };
    
    return (parse_result){ .result = true, .ast_stack = tree_stack, .ast_count = tree_count };
}