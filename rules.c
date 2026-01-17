#include "rules.h" 

grammar_rule language_rules[num_grammar_rules] = {
	[rule_block] = {{
		{{
			SYMRULE(statement,stat),
			SYMRULE(block,scope),
		},2},
		{{
			SYMRULE(statement,stat),
		},1},
	},2, sem_scope},
	[rule_statement] = {{
		{{
			SYMRULE(declaration,dec),
		},1},
		{{
			SYMRULE(assignment,assign),
		},1},
		{{
			SYMRULE(funccall,call),
		},1},
		{{
			SYMRULE(conditional,cond),
		},1},
		{{
			SYMRULE(jump,jmp),
		},1},
		{{
			SYMRULE(label,label),
		},1},
	},6, sem_stat},
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
			SYMRULE(expression,exp),
			TOKEN(COMMA),
			SYMRULE(argument,args),
		},3},
		{{
			SYMRULE(expression,exp),
		},1},
	},2, sem_args},
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
			SYMRULE(expression,exp),
		},3},
		{{
			SYMCHECK(IDENTIFIER,var),
			SYMCHECK(OPERATOR,op),
			SYMRULE(expression,exp),
		},3},
		{{
			SYMCHECK(CONST,val),
		},1},
		{{
			SYMCHECK(IDENTIFIER,var),
		},1},
	},4, sem_exp},
};
