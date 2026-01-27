#include "rules.h"
typedef enum { rule_block,rule_statement,rule_def,rule_include,rule_separator,rule_assignment,rule_funccall,rule_funcdec,rule_funcsign,rule_funcarguments,rule_argdec,rule_argument,rule_argname,rule_conditional,rule_else,rule_condition,rule_declaration,rule_jump,rule_label,rule_expression,rule_lambda,rule_chain,rule_math,rule_variable,rule_forloop,rule_whileloop,rule_dowhile,rule_enum,rule_enumcase,rule_struct,rule_decblock,rule_return,rule_interface,rule_intblock, num_grammar_rules } grammar_rules;

grammar_rule language_rules[num_grammar_rules] = {
	[rule_block] = {{
		{{ 
			SYMRULE(statement,stat), SYMRULE(block,scope), 
		 },2},
		{{ 
			SYMRULE(statement,stat), 
		 },1},
	},2, sem_rule_scope, sem_action_none},
	[rule_statement] = {{
		{{ 
			SYMRULE(interface,interf), 
		 },1},
		{{ 
			SYMRULE(enum,enum), 
		 },1},
		{{ 
			SYMRULE(def,def), 
		 },1},
		{{ 
			SYMRULE(struct,struct), 
		 },1},
		{{ 
			SYMRULE(include,inc), 
		 },1},
		{{ 
			SYMRULE(dowhile,dowhile), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(whileloop,while), 
		 },1},
		{{ 
			SYMRULE(conditional,cond), 
		 },1},
		{{ 
			SYMRULE(funcdec,func), 
		 },1},
		{{ 
			TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },3},
		{{ 
			TOKEN(LBRACE), TOKEN(RBRACE), 
		 },2},
		{{ 
			SYMRULE(forloop,for), 
		 },1},
		{{ 
			SYMRULE(jump,jmp), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(return,ret), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(chain,exp), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(assignment,assign), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(declaration,dec), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(label,label), 
		 },1},
		{{ 
			SYMRULE(funccall,call), TOKEN(SEMICOLON), 
		 },2},
	},19, 0},
	[rule_def] = {{
		{{ 
			LITERAL("defer"), SYMRULE(statement,exp), 
		 },2},
	},1, sem_rule_def, sem_action_none},
	[rule_include] = {{
		{{ 
			LITTOK(SYMBOL,"@"), LITERAL("includeC"), TOKEN(LPAREN), SYMCHECK(CONST,inc), TOKEN(RPAREN), 
		 },5},
	},1, sem_rule_inc, sem_action_none},
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
			SYMCHECK(IDENTIFIER,var), LITTOK(OPERATOR,"="), SYMRULE(expression,exp), 
		 },3},
	},1, sem_rule_assign, sem_action_none},
	[rule_funccall] = {{
		{{ 
			SYMCHECK(IDENTIFIER,func), TOKEN(LPAREN), SYMRULE(argument,args), TOKEN(RPAREN), 
		 },4},
		{{ 
			SYMCHECK(IDENTIFIER,func), TOKEN(LPAREN), TOKEN(RPAREN), 
		 },3},
	},2, sem_rule_call, sem_action_none},
	[rule_funcdec] = {{
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },6},
		{{ 
			SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },3},
	},8, sem_rule_func, sem_action_declare},
	[rule_funcsign] = {{
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,name), SYMRULE(funcarguments,param), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,name), 
		 },1},
	},4, sem_rule_func, sem_action_declare},
	[rule_funcarguments] = {{
		{{ 
			TOKEN(LPAREN), SYMRULE(argdec,param), TOKEN(RPAREN), 
		 },3},
		{{ 
			TOKEN(LPAREN), TOKEN(RPAREN), 
		 },2},
	},2, 0},
	[rule_argdec] = {{
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), TOKEN(COMMA), SYMRULE(argdec,param), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), 
		 },2},
	},2, sem_rule_param, sem_action_declare},
	[rule_argument] = {{
		{{ 
			RULE(argname), SYMRULE(expression,exp), TOKEN(COMMA), SYMRULE(argument,args), 
		 },4},
		{{ 
			SYMRULE(expression,exp), TOKEN(COMMA), SYMRULE(argument,args), 
		 },3},
		{{ 
			RULE(argname), SYMRULE(expression,exp), 
		 },2},
		{{ 
			SYMRULE(expression,exp), 
		 },1},
	},4, sem_rule_args, sem_action_none},
	[rule_argname] = {{
		{{ 
			TOKEN(IDENTIFIER), TOKEN(COLON), 
		 },2},
	},1, 0},
	[rule_conditional] = {{
		{{ 
			LITERAL("if"), SYMRULE(condition,cond), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), SYMRULE(else,else), 
		 },6},
		{{ 
			LITERAL("if"), SYMRULE(condition,cond), TOKEN(LBRACE), TOKEN(RBRACE), SYMRULE(else,else), 
		 },5},
		{{ 
			LITERAL("if"), SYMRULE(condition,cond), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			LITERAL("if"), SYMRULE(condition,cond), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
	},4, sem_rule_cond, sem_action_none},
	[rule_else] = {{
		{{ 
			LITERAL("else"), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },4},
		{{ 
			LITERAL("else"), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },3},
		{{ 
			LITERAL("else"), SYMRULE(conditional,cond), 
		 },2},
	},3, sem_rule_else, sem_action_none},
	[rule_condition] = {{
		{{ 
			TOKEN(LPAREN), SYMRULE(expression,cond), TOKEN(RPAREN), 
		 },3},
		{{ 
			SYMRULE(expression,cond), 
		 },1},
	},2, 0},
	[rule_declaration] = {{
		{{ 
			SYMDEC(IDENTIFIER,type), TOKEN(COLON), SYMDEC(IDENTIFIER,subtype), SYMDEC(IDENTIFIER,name), LITTOK(OPERATOR,"="), SYMRULE(expression,exp), 
		 },6},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), LITTOK(OPERATOR,"="), SYMRULE(expression,exp), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type), TOKEN(COLON), SYMDEC(IDENTIFIER,subtype), SYMDEC(IDENTIFIER,name), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,subtype), SYMDEC(IDENTIFIER,name), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type), TOKEN(COLON), SYMDEC(IDENTIFIER,name), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,type), SYMDEC(IDENTIFIER,name), 
		 },2},
	},7, sem_rule_dec, sem_action_declare},
	[rule_jump] = {{
		{{ 
			LITERAL("goto"), SYMCHECK(IDENTIFIER,jmp), 
		 },2},
	},1, sem_rule_jmp, sem_action_none},
	[rule_label] = {{
		{{ 
			SYMDEC(IDENTIFIER,name), TOKEN(COLON), 
		 },2},
	},1, sem_rule_label, sem_action_declare},
	[rule_expression] = {{
		{{ 
			LITTAG("not",op), SYMRULE(expression,exp), 
		 },2},
		{{ 
			SYMCHECK(LPAREN,syn), SYMRULE(expression,exp), TOKEN(RPAREN), 
		 },3},
		{{ 
			SYMRULE(math,exp), 
		 },1},
		{{ 
			SYMRULE(lambda,func), 
		 },1},
		{{ 
			SYMRULE(funccall,exp), 
		 },1},
		{{ 
			SYMRULE(chain,exp), 
		 },1},
		{{ 
			SYMCHECK(CONST,val), 
		 },1},
		{{ 
			SYMRULE(variable,var), 
		 },1},
	},8, sem_rule_exp, sem_action_none},
	[rule_lambda] = {{
		{{ 
			TOKEN(LPAREN), SYMRULE(argdec,param), TOKEN(RPAREN), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },6},
		{{ 
			TOKEN(LPAREN), SYMRULE(argdec,param), TOKEN(RPAREN), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },5},
	},2, sem_rule_func, sem_action_declare},
	[rule_chain] = {{
		{{ 
			SYMRULE(variable,var), SYMCHECK(DOT,op), SYMRULE(funccall,exp), 
		 },3},
		{{ 
			SYMRULE(variable,var), SYMCHECK(DOT,op), SYMRULE(variable,exp), 
		 },3},
	},2, sem_rule_var, sem_action_none},
	[rule_math] = {{
		{{ 
			SYMRULE(funccall,var), SYMCHECK(OPERATOR,op), SYMRULE(expression,exp), 
		 },3},
		{{ 
			SYMCHECK(CONST,val), SYMCHECK(OPERATOR,op), SYMRULE(expression,exp), 
		 },3},
		{{ 
			SYMRULE(variable,var), SYMCHECK(OPERATOR,op), SYMRULE(expression,exp), 
		 },3},
	},3, sem_rule_exp, sem_action_none},
	[rule_variable] = {{
		{{ 
			SYMCHECK(IDENTIFIER,var), SYMCHECK(LBRACKET,op), SYMRULE(expression,exp), TOKEN(RBRACKET), 
		 },4},
		{{ 
			SYMCHECK(IDENTIFIER,var), 
		 },1},
	},2, sem_rule_var, sem_action_none},
	[rule_forloop] = {{
		{{ 
			LITERAL("for"), TOKEN(LPAREN), SYMRULE(declaration,dec), TOKEN(SEMICOLON), SYMRULE(condition,cond), TOKEN(SEMICOLON), SYMRULE(assignment,assign), TOKEN(RPAREN), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },11},
		{{ 
			LITERAL("for"), TOKEN(LPAREN), SYMRULE(declaration,dec), TOKEN(SEMICOLON), SYMRULE(condition,cond), TOKEN(SEMICOLON), SYMRULE(assignment,assign), TOKEN(RPAREN), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },10},
	},2, sem_rule_for, sem_action_none},
	[rule_whileloop] = {{
		{{ 
			LITERAL("while"), SYMRULE(condition,cond), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			LITERAL("while"), SYMRULE(condition,cond), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
	},2, sem_rule_while, sem_action_none},
	[rule_dowhile] = {{
		{{ 
			LITERAL("do"), TOKEN(LBRACE), SYMRULE(block,scope), TOKEN(RBRACE), LITERAL("while"), SYMRULE(condition,cond), 
		 },6},
		{{ 
			LITERAL("do"), TOKEN(LBRACE), TOKEN(RBRACE), LITERAL("while"), SYMRULE(condition,cond), 
		 },5},
	},2, sem_rule_dowhile, sem_action_none},
	[rule_enum] = {{
		{{ 
			LITERAL("enum"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), SYMRULE(enumcase,enum_case), TOKEN(RBRACE), 
		 },5},
		{{ 
			LITERAL("enum"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
	},2, sem_rule_enum, sem_action_declare},
	[rule_enumcase] = {{
		{{ 
			SYMDEC(IDENTIFIER,name), TOKEN(COMMA), SYMRULE(enumcase,enum_case), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,name), TOKEN(COMMA), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,name), 
		 },1},
	},3, sem_rule_enum_case, sem_action_declare},
	[rule_struct] = {{
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), SYMRULE(decblock,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(COLON), SYMDEC(IDENTIFIER,parent), TOKEN(LBRACE), SYMRULE(decblock,scope), TOKEN(RBRACE), 
		 },7},
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(COLON), TOKEN(LBRACE), SYMRULE(decblock,scope), TOKEN(RBRACE), 
		 },6},
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(COLON), SYMDEC(IDENTIFIER,parent), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },6},
		{{ 
			LITERAL("struct"), SYMDEC(IDENTIFIER,name), TOKEN(COLON), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },5},
	},6, sem_rule_struct, sem_action_declare},
	[rule_decblock] = {{
		{{ 
			SYMRULE(declaration,dec), TOKEN(SEMICOLON), SYMRULE(decblock,scope), 
		 },3},
		{{ 
			SYMRULE(declaration,dec), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(funcdec,func), SYMRULE(decblock,scope), 
		 },2},
		{{ 
			SYMRULE(funcdec,func), 
		 },1},
	},4, sem_rule_scope, sem_action_none},
	[rule_return] = {{
		{{ 
			LITERAL("return"), SYMRULE(expression,exp), 
		 },2},
	},1, sem_rule_ret, sem_action_none},
	[rule_interface] = {{
		{{ 
			LITERAL("interface"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), SYMRULE(intblock,scope), TOKEN(RBRACE), 
		 },5},
		{{ 
			LITERAL("interface"), SYMDEC(IDENTIFIER,name), TOKEN(LBRACE), TOKEN(RBRACE), 
		 },4},
	},2, sem_rule_interf, sem_action_declare},
	[rule_intblock] = {{
		{{ 
			SYMRULE(declaration,dec), TOKEN(SEMICOLON), SYMRULE(intblock,scope), 
		 },3},
		{{ 
			SYMRULE(declaration,dec), TOKEN(SEMICOLON), 
		 },2},
		{{ 
			SYMRULE(funcsign,func), TOKEN(SEMICOLON), SYMRULE(intblock,scope), 
		 },3},
		{{ 
			SYMRULE(funcsign,func), TOKEN(SEMICOLON), 
		 },2},
	},4, sem_rule_scope, sem_action_none},
};

char* rule_names[num_grammar_rules] = {
		[rule_block] = "block",
		[rule_statement] = "statement",
		[rule_def] = "def",
		[rule_include] = "include",
		[rule_separator] = "separator",
		[rule_assignment] = "assignment",
		[rule_funccall] = "funccall",
		[rule_funcdec] = "funcdec",
		[rule_funcsign] = "funcsign",
		[rule_funcarguments] = "funcarguments",
		[rule_argdec] = "argdec",
		[rule_argument] = "argument",
		[rule_argname] = "argname",
		[rule_conditional] = "conditional",
		[rule_else] = "else",
		[rule_condition] = "condition",
		[rule_declaration] = "declaration",
		[rule_jump] = "jump",
		[rule_label] = "label",
		[rule_expression] = "expression",
		[rule_lambda] = "lambda",
		[rule_chain] = "chain",
		[rule_math] = "math",
		[rule_variable] = "variable",
		[rule_forloop] = "forloop",
		[rule_whileloop] = "whileloop",
		[rule_dowhile] = "dowhile",
		[rule_enum] = "enum",
		[rule_enumcase] = "enumcase",
		[rule_struct] = "struct",
		[rule_decblock] = "decblock",
		[rule_return] = "return",
		[rule_interface] = "interface",
		[rule_intblock] = "intblock",
	
};