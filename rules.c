#include "rules.h" 

grammar_rule language_rules[num_grammar_rules] = {
	[rule_block] = {{
		{{
			RULE(statement),
			RULE(block),
		},2},
		{{
			RULE(statement),
		},1},
	},2, sem_scope},
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
	},6, 0},
	[rule_assignment] = {{
		{{
			TOKEN(IDENTIFIER),
			LITTOK(OPERATOR,"="),
			RULE(expression),
			TOKEN(SEMICOLON),
		},4},
	},1, 0},
	[rule_funccall] = {{
		{{
			SYMCHECK(IDENTIFIER,func),
			TOKEN(LPAREN),
			RULE(argument),
			TOKEN(RPAREN),
			TOKEN(SEMICOLON),
		},5},
	},1, 0},
	[rule_argument] = {{
		{{
			RULE(expression),
			TOKEN(COMMA),
			RULE(argument),
		},3},
		{{
			RULE(expression),
		},1},
	},2, 0},
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
	},1, 0},
	[rule_declaration] = {{
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			LITTOK(OPERATOR,"="),
			RULE(expression),
			TOKEN(SEMICOLON),
		},5},
	},1, sem_var},
	[rule_jump] = {{
		{{
			LITERAL("goto"),
			SYMCHECK(IDENTIFIER,jmp),
			TOKEN(SEMICOLON),
		},3},
	},1, 0},
	[rule_label] = {{
		{{
			SYMDEC(IDENTIFIER,name),
			TOKEN(COLON),
		},2},
	},1, sem_jmp},
	[rule_expression] = {{
		{{
			TOKEN(CONST),
			TOKEN(OPERATOR),
			RULE(expression),
		},3},
		{{
			SYMCHECK(IDENTIFIER,var),
			TOKEN(OPERATOR),
			RULE(expression),
		},3},
		{{
			TOKEN(CONST),
		},1},
		{{
			SYMCHECK(IDENTIFIER,var),
		},1},
	},4, 0},
};