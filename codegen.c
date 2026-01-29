#include "codegen.h"
#include "codegenrules/general.h"
#include "rules.h"

codegen begin_rule(sem_rule type){
    switch (type){
        case sem_rule_scope: return blk_code_init();
        case sem_rule_dec: return dec_code_init();
        case sem_rule_assign: return ass_code_init();
        case sem_rule_exp: return exp_code_init();
        case sem_rule_call: return call_code_init();
        case sem_rule_args: return arg_code_init(); break;
        case sem_rule_cond: return cond_code_init();
        case sem_rule_jmp: return jmp_code_init();
        case sem_rule_label: return label_code_init();
        case sem_rule_param: return param_code_init();
        case sem_rule_func: return func_code_init();
        case sem_rule_for: return for_code_init();
        case sem_rule_while: return while_code_init();
        case sem_rule_dowhile: return dowhile_code_init();
        case sem_rule_var: return var_code_init();
        case sem_rule_inc: return inc_code_init();
        case sem_rule_struct: return struct_code_init();
        case sem_rule_ret: return ret_code_init();
        case sem_rule_def: return def_code_init();
        case sem_rule_interf: return int_code_init();
        case sem_rule_enum: return enum_code_init();
        case sem_rule_enum_case: return enum_case_code_init();
        case sem_rule_else: return else_code_init();
        case sem_rule_syn:
        case sem_rule_parent:
        case sem_rule_val: 
        case sem_rule_op: 
        case sem_rule_none:
        case sem_rule_stat:
        case sem_rule_rules_count:
            return (codegen){};
    }
}