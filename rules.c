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
			SYMCHECK(IDENTIFIER,var),
			LITTOK(OPERATOR,"="),
			SYMRULE(expression,exp),
			TOKEN(SEMICOLON),
		},4},
	},1, sem_assign},
	[rule_funccall] = {{
		{{
			SYMCHECK(IDENTIFIER,func),
			TOKEN(LPAREN),
			SYMRULE(argument,args),
			TOKEN(RPAREN),
			TOKEN(SEMICOLON),
		},5},
	},1, sem_call},
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
			SYMRULE(expression,cond),
			TOKEN(RPAREN),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},7},
	},1, sem_cond},
	[rule_declaration] = {{
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			LITTOK(OPERATOR,"="),
			SYMRULE(expression,exp),
			TOKEN(SEMICOLON),
		},5},
	},1, sem_dec},
	[rule_jump] = {{
		{{
			LITERAL("goto"),
			SYMCHECK(IDENTIFIER,jmp),
			TOKEN(SEMICOLON),
		},3},
	},1, sem_jmp},
	[rule_label] = {{
		{{
			SYMDEC(IDENTIFIER,name),
			TOKEN(COLON),
		},2},
	},1, sem_label},
	[rule_expression] = {{
		{{
			SYMCHECK(CONST,val),
			SYMCHECK(OPERATOR,op),
			RULE(expression),
		},3},
		{{
			SYMCHECK(IDENTIFIER,var),
			SYMCHECK(OPERATOR,op),
			RULE(expression),
		},3},
		{{
			SYMCHECK(CONST,val),
		},1},
		{{
			SYMCHECK(IDENTIFIER,var),
		},1},
	},4, sem_exp},
};
