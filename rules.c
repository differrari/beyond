#include "rules.h" 

typedef enum { 
	rule_block,
	rule_statement,
	rule_separator,
	rule_assignment,
	rule_funccall,
	rule_funcdec,
	rule_funcarguments,
	rule_argdec,
	rule_argument,
	rule_argname,
	rule_conditional,
	rule_condition,
	rule_declaration,
	rule_jump,
	rule_label,
	rule_expression,
	rule_chain,
	rule_math,
	rule_variable,
	rule_forloop,
	rule_whileloop,
	rule_dowhile,
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
			RULE(forloop),
		},1},
		{{
			RULE(whileloop),
		},1},
		{{
			RULE(dowhile),
		},1},
		{{
			SYMRULE(declaration,dec),
			TOKEN(SEMICOLON),
		},2},
		{{
			SYMRULE(assignment,assign),
			TOKEN(SEMICOLON),
		},2},
		{{
			SYMRULE(funccall,call),
			TOKEN(SEMICOLON),
		},2},
		{{
			SYMRULE(conditional,cond),
		},1},
		{{
			SYMRULE(jump,jmp),
			TOKEN(SEMICOLON),
		},2},
		{{
			SYMRULE(label,label),
		},1},
	},10, 0},
	[rule_separator] = {{
		{{
			TOKEN(SEMICOLON),
		},1},
		{{
			TOKEN(NEWLINE),
		},1},
	},2, 0},
	[rule_assignment] = {{
		{{
			SYMCHECK(IDENTIFIER,var),
			LITTOK(OPERATOR,"="),
			SYMRULE(expression,exp),
		},3},
	},1, sem_assign},
	[rule_funccall] = {{
		{{
			SYMCHECK(IDENTIFIER,func),
			TOKEN(LPAREN),
			SYMRULE(argument,args),
			TOKEN(RPAREN),
		},4},
		{{
			SYMCHECK(IDENTIFIER,func),
			TOKEN(LPAREN),
			TOKEN(RPAREN),
		},3},
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
			RULE(argname),
			SYMRULE(expression,exp),
			TOKEN(COMMA),
			SYMRULE(argument,args),
		},4},
		{{
			SYMRULE(expression,exp),
			TOKEN(COMMA),
			SYMRULE(argument,args),
		},3},
		{{
			RULE(argname),
			SYMRULE(expression,exp),
		},2},
		{{
			SYMRULE(expression,exp),
		},1},
	},4, sem_args},
	[rule_argname] = {{
		{{
			TOKEN(IDENTIFIER),
			TOKEN(COLON),
		},2},
	},1, 0},
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
		},4},
	},1, sem_dec},
	[rule_jump] = {{
		{{
			LITERAL("goto"),
			SYMCHECK(IDENTIFIER,jmp),
		},2},
	},1, sem_jmp},
	[rule_label] = {{
		{{
			SYMDEC(IDENTIFIER,name),
			TOKEN(COLON),
		},2},
	},1, sem_label},
	[rule_expression] = {{
		{{
			SYMRULE(funccall,exp),
		},1},
		{{
			SYMRULE(chain,exp),
		},1},
		{{
			TOKEN(LPAREN),
			SYMRULE(expression,exp),
			TOKEN(RPAREN),
		},3},
		{{
			SYMRULE(math,exp),
		},1},
	},4, sem_exp},
	[rule_chain] = {{
		{{
			SYMRULE(variable,var),
			TOKEN(DOT),
			SYMRULE(funccall,exp),
		},3},
		{{
			SYMRULE(variable,var),
			TOKEN(DOT),
			SYMCHECK(IDENTIFIER,exp),
		},3},
	},2, sem_exp},
	[rule_math] = {{
		{{
			SYMCHECK(CONST,val),
			SYMCHECK(OPERATOR,op),
			SYMRULE(expression,exp),
		},3},
		{{
			SYMRULE(variable,var),
			SYMCHECK(OPERATOR,op),
			SYMRULE(expression,exp),
		},3},
		{{
			SYMCHECK(CONST,val),
		},1},
		{{
			SYMRULE(variable,var),
		},1},
	},4, sem_exp},
	[rule_variable] = {{
		{{
			SYMCHECK(IDENTIFIER,var),
			TOKEN(LBRACKET),
			SYMRULE(math,exp),
			TOKEN(RBRACKET),
		},4},
		{{
			SYMCHECK(IDENTIFIER,var),
		},1},
	},2, sem_var},
	[rule_forloop] = {{
		{{
			LITERAL("for"),
			TOKEN(LPAREN),
			SYMRULE(declaration,dec),
			TOKEN(SEMICOLON),
			SYMRULE(condition,cond),
			TOKEN(SEMICOLON),
			SYMRULE(assignment,assign),
			TOKEN(RPAREN),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},11},
		{{
			LITERAL("for"),
			TOKEN(LPAREN),
			SYMRULE(declaration,dec),
			TOKEN(SEMICOLON),
			SYMRULE(condition,cond),
			TOKEN(SEMICOLON),
			SYMRULE(assignment,assign),
			TOKEN(RPAREN),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},10},
	},2, 0},
	[rule_whileloop] = {{
		{{
			LITERAL("while"),
			SYMRULE(condition,cond),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
		},5},
		{{
			LITERAL("while"),
			SYMRULE(condition,cond),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
		},4},
	},2, 0},
	[rule_dowhile] = {{
		{{
			LITERAL("do"),
			TOKEN(LBRACE),
			SYMRULE(block,scope),
			TOKEN(RBRACE),
			LITERAL("while"),
			SYMRULE(condition,cond),
		},6},
		{{
			LITERAL("do"),
			TOKEN(LBRACE),
			TOKEN(RBRACE),
			LITERAL("while"),
			SYMRULE(condition,cond),
		},5},
	},2, 0},
};

char* rule_names[num_grammar_rules] = {
	[rule_block] = "block",
	[rule_statement] = "statement",
	[rule_separator] = "separator",
	[rule_assignment] = "assignment",
	[rule_funccall] = "funccall",
	[rule_funcdec] = "funcdec",
	[rule_funcarguments] = "funcarguments",
	[rule_argdec] = "argdec",
	[rule_argument] = "argument",
	[rule_argname] = "argname",
	[rule_conditional] = "conditional",
	[rule_condition] = "condition",
	[rule_declaration] = "declaration",
	[rule_jump] = "jump",
	[rule_label] = "label",
	[rule_expression] = "expression",
	[rule_chain] = "chain",
	[rule_math] = "math",
	[rule_variable] = "variable",
	[rule_forloop] = "forloop",
	[rule_whileloop] = "whileloop",
	[rule_dowhile] = "dowhile",
};
