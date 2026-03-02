#include "rules.h"
typedef enum { rule_block,rule_statement,rule_def,rule_include,rule_separator,rule_assignment,rule_funccall,rule_funcdec,rule_funcsign,rule_funcarguments,rule_argdec,rule_argument,rule_argname,rule_conditional,rule_else,rule_condition,rule_declaration,rule_jump,rule_label,rule_expression,rule_lambda,rule_chain,rule_math,rule_variable,rule_forloop,rule_whileloop,rule_dowhile,rule_enum,rule_enumcase,rule_struct,rule_decblock,rule_return,rule_interface,rule_intblock,rule_switch,rule_switchbody, num_grammar_rules } grammar_rules;

grammar_rule language_rules[num_grammar_rules] = {
	[rule_block] = {{
		{{ 
			SYMRULE(statement,stat,false), SYMRULE(block,scope,true), 
		 },2},
		{{ 
			SYMRULE(statement,stat,false), 
		 },1},
	},2, sem_rule_scope, sem_action_none},
	[rule_statement] = {{
		{{ 
			SYMRULE(interface,interf,false), 
		 },1},
		{{ 
			SYMRULE(enum,enum,false), 
		 },1},
		{{ 
			SYMRULE(def,def,false), 
		 },1},
		{{ 
			SYMRULE(struct,struct,false), 
		 },1},
		{{ 
			SYMRULE(include,inc,false), 
		 },1},
		{{ 
			SYMRULE(dowhile,dowhile,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(whileloop,while,false), 
		 },1},
		{{ 
			SYMRULE(conditional,cond,false), 
		 },1},
		{{ 
			SYMRULE(switch,switch,false), 
		 },1},
		{{ 
			SYMRULE(funcdec,func,false), 
		 },1},
		{{ 
			TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },3},
		{{ 
			TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },2},
		{{ 
			SYMRULE(forloop,for,false), 
		 },1},
		{{ 
			SYMRULE(jump,jmp,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(return,ret,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(chain,exp,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(assignment,assign,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(label,label,false), 
		 },1},
		{{ 
			SYMRULE(funccall,call,false), TOKEN(SEMICOLON,false), 
		 },2},
	},20, 0},
	[rule_def] = {{
		{{ 
			LITERAL("defer",false), SYMRULE(statement,exp,false), 
		 },2},
	},1, sem_rule_def, sem_action_none},
	[rule_include] = {{
		{{ 
			LITTOK(SYMBOL,"@",false), LITERAL("includeC",false), TOKEN(LPAREN,false), SYMCHECK(CONST,inc,false), TOKEN(RPAREN,false), 
		 },5},
		{{ 
			LITTOK(SYMBOL,"#",false), LITERAL("include",false), SYMCHECK(CONST,inc,false), 
		 },3},
	},2, sem_rule_inc, sem_action_none},
	[rule_separator] = {{
		{{ 
			TOKEN(SEMICOLON,false), 
		 },1},
		{{ 
			TOKEN(NEWLINE,false), 
		 },1},
	},2, 0},
	[rule_assignment] = {{
		{{ 
			SYMRULE(variable,var,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },3},
		{{ 
			SYMRULE(chain,var,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },3},
	},2, sem_rule_assign, sem_action_none},
	[rule_funccall] = {{
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), SYMRULE(argument,args,true), TOKEN(RPAREN,false), 
		 },6},
		{{ 
			SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), SYMRULE(argument,args,true), TOKEN(RPAREN,false), 
		 },5},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), SYMRULE(argument,args,true), TOKEN(RPAREN,false), 
		 },5},
		{{ 
			SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), SYMRULE(argument,args,true), TOKEN(RPAREN,false), 
		 },4},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), TOKEN(RPAREN,false), 
		 },5},
		{{ 
			SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), TOKEN(RPAREN,false), 
		 },4},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), TOKEN(RPAREN,false), 
		 },4},
		{{ 
			SYMCHECK(IDENTIFIER,func,false), TOKEN(LPAREN,false), TOKEN(RPAREN,false), 
		 },3},
	},8, sem_rule_call, sem_action_none},
	[rule_funcdec] = {{
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },3},
	},8, sem_rule_func, sem_action_declare},
	[rule_funcsign] = {{
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,name,false), SYMRULE(funcarguments,param,true), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,type,true), SYMDEC(IDENTIFIER,name,false), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,name,false), 
		 },1},
	},4, sem_rule_func, sem_action_declare},
	[rule_funcarguments] = {{
		{{ 
			TOKEN(LPAREN,false), SYMRULE(argdec,param,true), TOKEN(RPAREN,false), 
		 },3},
		{{ 
			TOKEN(LPAREN,false), TOKEN(RPAREN,false), 
		 },2},
	},2, 0},
	[rule_argdec] = {{
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,name,false), TOKEN(COMMA,false), SYMRULE(argdec,param,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,name,false), 
		 },2},
	},2, sem_rule_param, sem_action_declare},
	[rule_argument] = {{
		{{ 
			RULE(argname,true), SYMRULE(expression,exp,false), TOKEN(COMMA,false), SYMRULE(argument,args,false), 
		 },4},
		{{ 
			SYMRULE(expression,exp,false), TOKEN(COMMA,false), SYMRULE(argument,args,false), 
		 },3},
		{{ 
			RULE(argname,true), SYMRULE(expression,exp,false), 
		 },2},
		{{ 
			SYMRULE(expression,exp,false), 
		 },1},
	},4, sem_rule_args, sem_action_none},
	[rule_argname] = {{
		{{ 
			TOKEN(IDENTIFIER,false), TOKEN(COLON,false), 
		 },2},
	},1, 0},
	[rule_conditional] = {{
		{{ 
			LITERAL("if",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), SYMRULE(else,else,true), 
		 },6},
		{{ 
			LITERAL("if",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), SYMRULE(else,else,true), 
		 },5},
		{{ 
			LITERAL("if",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("if",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},4, sem_rule_cond, sem_action_none},
	[rule_else] = {{
		{{ 
			LITERAL("else",false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },4},
		{{ 
			LITERAL("else",false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },3},
		{{ 
			LITERAL("else",false), SYMRULE(conditional,cond,false), 
		 },2},
	},3, sem_rule_else, sem_action_none},
	[rule_condition] = {{
		{{ 
			TOKEN(LPAREN,false), SYMRULE(expression,cond,false), TOKEN(RPAREN,false), 
		 },3},
		{{ 
			SYMRULE(expression,cond,false), 
		 },1},
	},2, 0},
	[rule_declaration] = {{
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,false), SYMDEC(IDENTIFIER,subtype,false), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },7},
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,false), SYMDEC(IDENTIFIER,subtype,false), SYMDEC(IDENTIFIER,name,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },6},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,name,false), LITTOK(OPERATOR,"=",false), SYMRULE(expression,exp,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,true), SYMDEC(IDENTIFIER,subtype,true), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), 
		 },5},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,subtype,true), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,true), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,true), SYMDEC(IDENTIFIER,subtype,true), SYMDEC(IDENTIFIER,name,false), 
		 },4},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,subtype,true), SYMDEC(IDENTIFIER,name,false), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type,false), TOKEN(COLON,true), SYMDEC(IDENTIFIER,name,false), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,name,false), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMTOK(OPERATOR,ref,declare,sem_elem,"*",true), SYMDEC(IDENTIFIER,name,false), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,type,false), SYMDEC(IDENTIFIER,name,false), 
		 },2},
	},14, sem_rule_dec, sem_action_declare},
	[rule_jump] = {{
		{{ 
			LITERAL("goto",false), SYMCHECK(IDENTIFIER,jmp,false), 
		 },2},
	},1, sem_rule_jmp, sem_action_none},
	[rule_label] = {{
		{{ 
			SYMDEC(IDENTIFIER,name,false), TOKEN(COLON,false), 
		 },2},
	},1, sem_rule_label, sem_action_declare},
	[rule_expression] = {{
		{{ 
			LITTAG("not",op,false), SYMRULE(expression,exp,false), 
		 },2},
		{{ 
			SYMCHECK(LPAREN,syn,false), SYMRULE(expression,exp,false), TOKEN(RPAREN,false), 
		 },3},
		{{ 
			SYMRULE(math,exp,false), 
		 },1},
		{{ 
			SYMRULE(lambda,func,false), 
		 },1},
		{{ 
			SYMRULE(funccall,exp,false), 
		 },1},
		{{ 
			SYMRULE(chain,exp,false), 
		 },1},
		{{ 
			SYMCHECK(CONST,val,false), 
		 },1},
		{{ 
			SYMRULE(variable,var,false), 
		 },1},
	},8, sem_rule_exp, sem_action_none},
	[rule_lambda] = {{
		{{ 
			SYMDEC(IDENTIFIER,type,true), TOKEN(LPAREN,false), SYMRULE(argdec,param,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },7},
		{{ 
			TOKEN(LPAREN,false), SYMRULE(argdec,param,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			SYMDEC(IDENTIFIER,type,true), TOKEN(LPAREN,false), SYMRULE(argdec,param,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			TOKEN(LPAREN,false), SYMRULE(argdec,param,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },5},
	},4, sem_rule_func, sem_action_declare},
	[rule_chain] = {{
		{{ 
			SYMRULE(variable,var,false), SYMCHECK(DOT,op,false), SYMRULE(funccall,exp,false), 
		 },3},
		{{ 
			SYMRULE(variable,var,false), SYMCHECK(DOT,op,false), SYMRULE(variable,exp,false), 
		 },3},
	},2, sem_rule_var, sem_action_none},
	[rule_math] = {{
		{{ 
			SYMRULE(funccall,var,false), SYMCHECK(OPERATOR,op,false), SYMRULE(expression,exp,false), 
		 },3},
		{{ 
			SYMCHECK(CONST,val,false), SYMCHECK(OPERATOR,op,false), SYMRULE(expression,exp,false), 
		 },3},
		{{ 
			SYMRULE(variable,var,false), SYMCHECK(OPERATOR,op,false), SYMRULE(expression,exp,false), 
		 },3},
	},3, sem_rule_exp, sem_action_none},
	[rule_variable] = {{
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,var,false), SYMCHECK(LBRACKET,op,false), SYMRULE(expression,exp,false), TOKEN(RBRACKET,false), 
		 },6},
		{{ 
			SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,var,false), SYMCHECK(LBRACKET,op,false), SYMRULE(expression,exp,false), TOKEN(RBRACKET,false), 
		 },5},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMCHECK(IDENTIFIER,var,false), SYMCHECK(LBRACKET,op,false), SYMRULE(expression,exp,false), TOKEN(RBRACKET,false), 
		 },5},
		{{ 
			SYMCHECK(IDENTIFIER,var,false), SYMCHECK(LBRACKET,op,false), SYMRULE(expression,exp,false), TOKEN(RBRACKET,false), 
		 },4},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,var,false), 
		 },3},
		{{ 
			SYMTOK(OPERATOR,addr,check,sem_rule,"&",true), SYMCHECK(IDENTIFIER,var,false), 
		 },2},
		{{ 
			SYMTOK(OPERATOR,deref,check,sem_rule,"*",true), SYMCHECK(IDENTIFIER,var,false), 
		 },2},
		{{ 
			SYMCHECK(IDENTIFIER,var,false), 
		 },1},
	},8, sem_rule_var, sem_action_none},
	[rule_forloop] = {{
		{{ 
			LITERAL("for",false), TOKEN(LPAREN,false), SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), SYMRULE(condition,cond,false), TOKEN(SEMICOLON,false), SYMRULE(assignment,assign,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },11},
		{{ 
			LITERAL("for",false), TOKEN(LPAREN,false), SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), SYMRULE(condition,cond,false), TOKEN(SEMICOLON,false), SYMRULE(assignment,assign,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },10},
	},2, sem_rule_for, sem_action_none},
	[rule_whileloop] = {{
		{{ 
			LITERAL("while",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("while",false), SYMRULE(condition,cond,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},2, sem_rule_while, sem_action_none},
	[rule_dowhile] = {{
		{{ 
			LITERAL("do",false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), LITERAL("while",false), SYMRULE(condition,cond,false), 
		 },6},
		{{ 
			LITERAL("do",false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), LITERAL("while",false), SYMRULE(condition,cond,false), 
		 },5},
	},2, sem_rule_dowhile, sem_action_none},
	[rule_enum] = {{
		{{ 
			LITERAL("enum",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), SYMRULE(enumcase,enum_case,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("enum",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},2, sem_rule_enum, sem_action_declare},
	[rule_enumcase] = {{
		{{ 
			SYMDEC(IDENTIFIER,name,false), TOKEN(COMMA,false), SYMRULE(enumcase,enum_case,true), 
		 },3},
		{{ 
			SYMDEC(IDENTIFIER,name,false), TOKEN(COMMA,false), 
		 },2},
		{{ 
			SYMDEC(IDENTIFIER,name,false), 
		 },1},
	},3, sem_rule_enum_case, sem_action_declare},
	[rule_struct] = {{
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), SYMRULE(decblock,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(COLON,false), SYMDEC(IDENTIFIER,parent,true), TOKEN(LBRACE,false), SYMRULE(decblock,scope,true), TOKEN(RBRACE,false), 
		 },7},
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(COLON,false), TOKEN(LBRACE,false), SYMRULE(decblock,scope,true), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(COLON,false), SYMDEC(IDENTIFIER,parent,true), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			LITERAL("struct",false), SYMDEC(IDENTIFIER,name,false), TOKEN(COLON,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },5},
	},6, sem_rule_struct, sem_action_declare},
	[rule_decblock] = {{
		{{ 
			SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), SYMRULE(decblock,scope,true), 
		 },3},
		{{ 
			SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(funcdec,func,false), SYMRULE(decblock,scope,true), 
		 },2},
		{{ 
			SYMRULE(funcdec,func,false), 
		 },1},
	},4, sem_rule_scope, sem_action_none},
	[rule_return] = {{
		{{ 
			LITERAL("return",false), SYMRULE(expression,exp,true), 
		 },2},
		{{ 
			LITERAL("return",false), 
		 },1},
	},2, sem_rule_ret, sem_action_none},
	[rule_interface] = {{
		{{ 
			LITERAL("interface",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), SYMRULE(intblock,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("interface",false), SYMDEC(IDENTIFIER,name,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},2, sem_rule_interf, sem_action_declare},
	[rule_intblock] = {{
		{{ 
			SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), SYMRULE(intblock,scope,true), 
		 },3},
		{{ 
			SYMRULE(declaration,dec,false), TOKEN(SEMICOLON,false), 
		 },2},
		{{ 
			SYMRULE(funcsign,func,false), TOKEN(SEMICOLON,false), SYMRULE(intblock,scope,true), 
		 },3},
		{{ 
			SYMRULE(funcsign,func,false), TOKEN(SEMICOLON,false), 
		 },2},
	},4, sem_rule_scope, sem_action_none},
	[rule_switch] = {{
		{{ 
			LITERAL("switch",false), TOKEN(LPAREN,false), SYMRULE(expression,exp,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), SYMRULE(switchbody,scope,true), TOKEN(RBRACE,false), 
		 },7},
		{{ 
			LITERAL("switch",false), TOKEN(LPAREN,false), SYMRULE(expression,exp,false), TOKEN(RPAREN,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			LITERAL("switch",false), SYMRULE(expression,exp,false), TOKEN(LBRACE,false), SYMRULE(switchbody,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("switch",false), SYMRULE(expression,exp,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},4, sem_rule_switch, sem_action_none},
	[rule_switchbody] = {{
		{{ 
			LITERAL("case",true), SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), SYMRULE(switchbody,cases,true), 
		 },7},
		{{ 
			SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), SYMRULE(switchbody,cases,true), 
		 },6},
		{{ 
			LITERAL("case",true), SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), SYMRULE(switchbody,cases,true), 
		 },6},
		{{ 
			SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), SYMRULE(switchbody,cases,true), 
		 },5},
		{{ 
			LITERAL("case",true), SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },6},
		{{ 
			SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), SYMRULE(block,scope,true), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			LITERAL("case",true), SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },5},
		{{ 
			SYMRULE(expression,exp,false), TOKEN(COLON,false), TOKEN(LBRACE,false), TOKEN(RBRACE,false), 
		 },4},
	},8, sem_rule_cases, sem_action_none},
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
		[rule_switch] = "switch",
		[rule_switchbody] = "switchbody",
	
};