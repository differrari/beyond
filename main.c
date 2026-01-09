#include "syscalls/syscalls.h" 
#include "data/scanner/scanner.h"
#include "rules.h"
#include "data/helpers/token_stream.h"
#include "std/string_slice.h"

Scanner scan;

char *indent = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

typedef struct {
    grammar_rules current_rule;
    int option;
    int sequence;
    uint32_t scanner_pos;
} parser_sm;

#define MAX_PARSE_DEPTH 16

parser_sm parser_stack[MAX_PARSE_DEPTH];
int parser_depth;

#define parser_debug(...) print(__VA_ARGS__)

#define curr_indent (indent + (MAX_PARSE_DEPTH-parser_depth))

#define current_parser_rule(parser) language_rules[parser->current_rule].options[parser->option].rules[parser->sequence]

size_t furthest_pos = 0;

bool parser_advance_to_token(parser_sm *parser, Token t){
    while (current_parser_rule(parser).rule){
        parser_debug("%s[r:%i,o:%i,s:%i] subrule %i",curr_indent, parser->current_rule, parser->option, parser->sequence, current_parser_rule(parser).value);
        if (parser_depth == MAX_PARSE_DEPTH - 1){
            parser_debug("Maximum depth reached, too many nested statements, shame on you");
            return false;
        }
        // parser_debug("Push state");
        parser_stack[parser_depth++] = *parser;
        parser->scanner_pos = t.pos;
        parser->current_rule = current_parser_rule(parser).value;
        parser->sequence = 0;
        parser->option = 0;
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
        if (scan.pos > furthest_pos) furthest_pos = scan.pos;
        scan.pos = parser->scanner_pos;
    }
    parser_depth--;
    return true;
}

bool parser_advance_option_sm(parser_sm *parser){
    if (parser->option + 1 == language_rules[parser->current_rule].num_elements){
        parser_debug("%sRule %i failed",curr_indent,parser->current_rule);
        if (!pop_parser_stack(parser, true)) return false;
        return parser_advance_option_sm(parser);
    } else {
        parser->option++;
        parser->sequence = 0;
        parser_debug("%sRule %i option %i failed",curr_indent,parser->current_rule, parser->option);
        if (scan.pos > furthest_pos) furthest_pos = scan.pos;
        scan.pos = parser->scanner_pos;
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

bool parse_token(char *content, Token t, parser_sm *parser){
    parser_advance_to_token(parser, t);
    // parser_debug("Evaluating token at %i",t.pos);
    grammar_elem element = current_parser_rule(parser);
    if (t.kind == element.value && (!element.lit || slice_lit_match(token_to_slice(t), element.lit))){
        parser_debug("%sParsed token %v [%i] = %i",curr_indent, make_string_slice(content, t.pos, t.length), t.kind, element.value);
        return parser_advance_sequence(parser);
    } else {
        parser_debug("%sFailed to match token %i, found %i. Skipping",curr_indent, element.value, t.kind);
        return parser_advance_option_sm(parser);
    }
}

int main(int argc, char *argv[]){
    
    char *content = read_full_file("street.cred");
    
    scan = scanner_make(content,strlen(content));
    
    Tokenizer tk = tokenizer_make(&scan);
    tk.skip_type_check = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_SLASH;
    // tk.parse_newline = true;
    TokenStream ts;
    ts_init(&ts,&tk);
    
    parser_sm parser = (parser_sm){
        .current_rule = rule_block,
        .option = 0,
        .sequence = 0,
        .scanner_pos = scan.pos,
    };
    
    bool result;
    Token t;
    while (ts_next(&ts, &t)) {
        if (!t.kind)
            break;
        result = parse_token(content, t, &parser);
        if (!result)
            break;
        // msleep(1000);
    }
    
    if (tk.err){
        printf("Tokenizer failed %i",tk.err);
        result = false;
    }
    
    printf("String scanned %i",result);
    if (!result)
        printf("Found syntax error at position %i",furthest_pos);
    
    return 0;
}