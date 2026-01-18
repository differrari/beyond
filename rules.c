#include "rules.h" 

typedef enum { 
	rule_block,
	rule_statement,
	rule_assignment,
	rule_funccall,
	rule_funcdec,
	rule_funcarguments,
	rule_argdec,
	rule_argument,
	rule_conditional,
	rule_condition,
	rule_declaration,
	rule_jump,
	rule_label,
	rule_expression,
 	num_grammar_rules
} grammar_rules;

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
			SYMRULE(funcdec,func),
		},1},
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
	},7, 0},
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
		{{
			SYMCHECK(IDENTIFIER,func),
			TOKEN(LPAREN),
			TOKEN(RPAREN),
			TOKEN(SEMICOLON),
		},4},
	},2, sem_call},
	[rule_funcdec] = {{
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			SYMRULE(funcarguments,param),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},6},
		{{
			SYMDEC(IDENTIFIER,name),
			SYMRULE(funcarguments,param),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},5},
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},5},
		{{
			SYMDEC(IDENTIFIER,name),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},4},
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			SYMRULE(funcarguments,param),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},5},
		{{
			SYMDEC(IDENTIFIER,name),
			SYMRULE(funcarguments,param),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},4},
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},4},
		{{
			SYMDEC(IDENTIFIER,name),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},3},
	},8, sem_func},
	[rule_funcarguments] = {{
		{{
			TOKEN(LPAREN),
			SYMRULE(argdec,param),
			TOKEN(RPAREN),
		},3},
		{{
			TOKEN(LPAREN),
			TOKEN(RPAREN),
		},2},
	},2, 0},
	[rule_argdec] = {{
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
			TOKEN(COMMA),
			SYMRULE(argdec,param),
		},4},
		{{
			SYMDEC(IDENTIFIER,type),
			SYMDEC(IDENTIFIER,name),
		},2},
	},2, sem_param},
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
			SYMRULE(condition,cond),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},5},
		{{
			LITERAL("if"),
			SYMRULE(condition,cond),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},4},
	},2, sem_cond},
	[rule_condition] = {{
		{{
			TOKEN(LPAREN),
			SYMRULE(expression,cond),
			TOKEN(RPAREN),
		},3},
		{{
			SYMRULE(expression,cond),
		},1},
	},2, 0},
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

char* rule_names[num_grammar_rules] = {
	[rule_block] = "block",
	[rule_statement] = "statement",
	[rule_assignment] = "assignment",
	[rule_funccall] = "funccall",
	[rule_funcdec] = "funcdec",
	[rule_funcarguments] = "funcarguments",
	[rule_argdec] = "argdec",
	[rule_argument] = "argument",
	[rule_conditional] = "conditional",
	[rule_condition] = "condition",
	[rule_declaration] = "declaration",
	[rule_jump] = "jump",
	[rule_label] = "label",
	[rule_expression] = "expression",
};
