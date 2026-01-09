#include "syscalls/syscalls.h" 
#include "data/scanner/scanner.h"
#include "data/tokenizer/tokenizer.h"
#include "data/helpers/token_stream.h"
#include "std/string_slice.h"

typedef enum { rule_block, rule_statement, rule_declaration, rule_assignment, rule_expression, rule_funccall, rule_argument, rule_conditional, rule_jump, rule_label, num_grammar_rules } grammar_rules;

typedef struct {
    bool rule;
    int value;
    char *lit;
} grammar_elem;

typedef struct {
    grammar_elem rules[16];
    uint8_t num_elements;
} grammar_rule_opt;

typedef struct {
    grammar_rule_opt options[8];
    uint8_t num_elements;
} grammar_rule;

#define TOKEN(name) { false, TOK_##name, 0 }
#define RULE(name) { true, rule_##name, 0 }
#define LITERAL(val) { false, TOK_IDENTIFIER, val }
#define LITTOK(tok,val) { false, TOK_##tok, val }

Scanner scan;

char *indent = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

grammar_rule language_rules[num_grammar_rules] = {
    [rule_block] = {{
	{{
		RULE(statement),
		RULE(statement),
	},2},
	{{
		RULE(statement),
	},1},
    },2},
    [rule_statement] = {{
	{{
		RULE(declaration),
	},1},
	{{
		RULE(assignment),
	},1},
	{{
		RULE(funccall),
	},1},
	{{
		RULE(conditional),
	},1},
	{{
		RULE(jump),
	},1},
	{{
		RULE(label),
	},1},
    },6},
    [rule_assignment] = {{
	{{
		TOKEN(IDENTIFIER),
		LITTOK(OPERATOR,"="),
		RULE(expression),
		TOKEN(SEMICOLON),
	},4},
    },1},
    [rule_funccall] = {{
	{{
		TOKEN(IDENTIFIER),
		TOKEN(LPAREN),
		RULE(argument),
		TOKEN(RPAREN),
		TOKEN(SEMICOLON),
	},5},
    },1},
    [rule_argument] = {{
	{{
		RULE(expression),
		TOKEN(COMMA),
		RULE(argument),
	},3},
	{{
		RULE(expression),
	},1},
    },2},
    [rule_conditional] = {{
	{{
		LITERAL("if"),
		TOKEN(LPAREN),
		RULE(expression),
		TOKEN(RPAREN),
		TOKEN(LBRACE),
		RULE(block),
		TOKEN(RBRACE),
	},7},
    },1},
    [rule_declaration] = {{
	{{
		TOKEN(IDENTIFIER),
		TOKEN(IDENTIFIER),
		LITTOK(OPERATOR,"="),
		RULE(expression),
		TOKEN(SEMICOLON),
	},5},
    },1},
    [rule_jump] = {{
	{{
		LITERAL("goto"),
		TOKEN(IDENTIFIER),
		TOKEN(SEMICOLON),
	},3},
    },1},
    [rule_label] = {{
	{{
		TOKEN(IDENTIFIER),
		TOKEN(COLON),
	},2},
    },1},
    [rule_expression] = {{
	{{
		TOKEN(CONST),
		TOKEN(OPERATOR),
		RULE(expression),
	},3},
	{{
		TOKEN(IDENTIFIER),
		TOKEN(OPERATOR),
		RULE(expression),
	},3},
	{{
		TOKEN(CONST),
	},1},
	{{
		TOKEN(IDENTIFIER),
	},1},
    },4},
};

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
    if (t.kind == element.value && (!element.lit || slice_lit_match(token_to_slice(t), element.lit){
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