#include "codegen.h"
#include "codegenrules/general.h"
#include "rules.h"

codegen_t begin_rule(semantic_rules type){
    switch (type){
        case sem_scope: return blk_code_init();
        case sem_dec: return dec_code_init();
        case sem_assign: return ass_code_init();
        case sem_exp: return exp_code_init();
        case sem_call: return call_code_init();
        case sem_args: return arg_code_init(); break;
        case sem_cond: return cond_code_init();
        case sem_jmp: return jmp_code_init();
        case sem_label: return label_code_init();
        case sem_param: return param_code_init();
        case sem_func: return func_code_init();
        case sem_for: return for_code_init();
        case sem_while: return while_code_init();
        case sem_dowhile: return dowhile_code_init();
        case sem_var: return var_code_init();
        case sem_inc: return inc_code_init();
        case sem_struct: return struct_code_init();
        case sem_ret: return ret_code_init();
        case sem_def: return def_code_init();
        case sem_val: 
        case sem_op: 
        case sem_none:
        case sem_stat:
        case sem_rules_count:
            return (codegen_t){};
    }
}

void register_elem(codegen_t gen, int type, Token elem){
   if (gen.register_elem) gen.register_elem(gen.ptr, type, elem);
}

void end_rule(int type){
    
}

void register_subrule(codegen_t gen, int type, codegen_t child){
    if (gen.register_subrule) gen.register_subrule(gen.ptr, type, child);
}

void emit_code(codegen_t gen){
    if (gen.emit_code) gen.emit_code(gen.ptr);
}