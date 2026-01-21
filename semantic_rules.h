typedef enum {
	sem_rule_none,
	sem_rule_dec,
	sem_rule_stat,
	sem_rule_var,
	sem_rule_assign,
	sem_rule_exp,
	sem_rule_func,
	sem_rule_label,
	sem_rule_jmp,
	sem_rule_scope,
	sem_rule_param,
	sem_rule_val,
	sem_rule_op,
	sem_rule_cond,
	sem_rule_call,
	sem_rule_args,
	sem_rule_for,
	sem_rule_while,
	sem_rule_dowhile,
	sem_rule_inc,
	sem_rule_struct,
	sem_rule_rules_count,
	sem_rule_ret,
	sem_rule_def,
	sem_rule_interf,
	sem_rule_enum,
	sem_rule_enum_case,
	sem_rule_parent,
} sem_rule;

static inline char* sem_rule_to_string(sem_rule val){
	switch (val) {
		case sem_rule_none: return "none";
		case sem_rule_dec: return "dec";
		case sem_rule_stat: return "stat";
		case sem_rule_var: return "var";
		case sem_rule_assign: return "assign";
		case sem_rule_exp: return "exp";
		case sem_rule_func: return "func";
		case sem_rule_label: return "label";
		case sem_rule_jmp: return "jmp";
		case sem_rule_scope: return "scope";
		case sem_rule_param: return "param";
		case sem_rule_val: return "val";
		case sem_rule_op: return "op";
		case sem_rule_cond: return "cond";
		case sem_rule_call: return "call";
		case sem_rule_args: return "args";
		case sem_rule_for: return "for";
		case sem_rule_while: return "while";
		case sem_rule_dowhile: return "dowhile";
		case sem_rule_inc: return "inc";
		case sem_rule_struct: return "struct";
		case sem_rule_rules_count: return "rules_count";
		case sem_rule_ret: return "ret";
		case sem_rule_def: return "def";
		case sem_rule_interf: return "interf";
		case sem_rule_enum: return "enum";
		case sem_rule_enum_case: return "enum_case";
		case sem_rule_parent: return "parent";
	} 
} 

typedef enum {
	sem_elem_none,
	sem_elem_type,
	sem_elem_name,
	sem_elem_parent,
	sem_elem_count,
} sem_elem;

static inline char* sem_elem_to_string(sem_elem val){
	switch (val) {
		case sem_elem_none: return "none";
		case sem_elem_type: return "type";
		case sem_elem_name: return "name";
		case sem_elem_parent: return "parent";
		case sem_elem_count: return "count";
	} 
} 

typedef enum {
	sem_action_none,
	sem_action_declare,
	sem_action_check,
} sem_action;

static inline char* sem_action_to_string(sem_action val){
	switch (val) {
		case sem_action_none: return "none";
		case sem_action_declare: return "declare";
		case sem_action_check: return "check";
	} 
} 
