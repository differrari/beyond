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

char* sem_rule_to_string(sem_rule val);

typedef enum {
	sem_elem_none,
	sem_elem_type,
	sem_elem_name,
	sem_elem_parent,
	sem_elem_count,
} sem_elem;

char* sem_elem_to_string(sem_elem val);

typedef enum {
	sem_action_none,
	sem_action_declare,
	sem_action_check,
} sem_action;

char* sem_action_to_string(sem_action val);
