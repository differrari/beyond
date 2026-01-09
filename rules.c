#include "rules.h" 

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
