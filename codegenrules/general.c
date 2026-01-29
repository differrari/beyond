#include "rules.h"
#include "general.h"
#include "syscalls/syscalls.h"

#define CODEGEN_DEC(name,t) \
extern void name##_emit_code(void*ptr);\
codegen name##_init(){\
    return (codegen){\
        .ptr = zalloc(sizeof(name)),\
        .register_elem = name##_register_elem,\
        .register_subrule = name##_register_subrule,\
        .emit_code = name##_emit_code,\
        .type = t,\
    };\
}

void blk_code_register_elem(void* ptr, int type, Token elem){
    
}

void blk_code_register_subrule(void* ptr, int type, codegen child){
    blk_code *code = (blk_code*)ptr;
    if (type == sem_rule_scope){
        code->chain = child;
    } else
        code->stat = child;
}

CODEGEN_DEC(blk_code, sem_rule_scope)

void dec_code_register_elem(void* ptr, int type, Token elem){
    dec_code *code = (dec_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void dec_code_register_subrule(void* ptr, int type, codegen child){
    dec_code *code = (dec_code*)ptr;
    code->initial_value = child;
}

CODEGEN_DEC(dec_code, sem_rule_dec)

void ass_code_register_elem(void* ptr, int type, Token elem){
    ass_code *code = (ass_code*)ptr;
    code->name = elem;
}

void ass_code_register_subrule(void* ptr, int type, codegen child){
    ass_code *code = (ass_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(ass_code, sem_rule_assign)

void call_code_register_elem(void* ptr, int type, Token elem){
    call_code *code = (call_code*)ptr;
    code->name = elem;
}

void call_code_register_subrule(void* ptr, int type, codegen child){
    call_code *code = (call_code*)ptr;
    code->args = child;
}

CODEGEN_DEC(call_code, sem_rule_call)

void cond_code_register_elem(void* ptr, int type, Token elem){
    
}

void cond_code_register_subrule(void* ptr, int type, codegen child){
    cond_code *code = (cond_code*)ptr;
    switch (type){
        case sem_rule_cond: code->cond = child; break;
        case sem_rule_scope: code->scope = child; break;
        case sem_rule_else: code->chain = child; break;
    }
}

CODEGEN_DEC(cond_code, sem_rule_cond)

void jmp_code_register_elem(void* ptr, int type, Token elem){
    jmp_code *code = (jmp_code*)ptr;
    code->jump = elem;
}

void jmp_code_register_subrule(void* ptr, int type, codegen child){
    
}

CODEGEN_DEC(jmp_code, sem_rule_jmp)

void label_code_register_elem(void* ptr, int type, Token elem){
    label_code *code = (label_code*)ptr;
    code->name = elem;
}

void label_code_register_subrule(void* ptr, int type, codegen child){
    
}

CODEGEN_DEC(label_code, sem_rule_label)

void exp_code_register_elem(void* ptr, int type, Token elem){
    exp_code *code = (exp_code*)ptr;
    if (type == sem_rule_val)
        code->val = elem;
    else if (type == sem_rule_syn)
        code->paren = true;
    else if (type == sem_rule_op && slice_lit_match(token_to_slice(elem), "not", false))
        code->invert = true;
    else code->operand = elem;
}

void exp_code_register_subrule(void* ptr, int type, codegen child){
    exp_code *code = (exp_code*)ptr;
    switch (type){
        case sem_rule_var: code->var = child; break;
        case sem_rule_exp: code->exp = child; break;
        case sem_rule_func: code->lambda = child; break;
    }
}

CODEGEN_DEC(exp_code, sem_rule_exp)

void arg_code_register_elem(void* ptr, int type, Token elem){
   
}

void arg_code_register_subrule(void* ptr, int type, codegen child){
    arg_code *code = (arg_code*)ptr;
    if (type == sem_rule_args)
        code->chain = child;
    if (type == sem_rule_exp)
        code->exp = child;
}

CODEGEN_DEC(arg_code, sem_rule_args)

void param_code_register_elem(void* ptr, int type, Token elem){
    param_code *code = (param_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void param_code_register_subrule(void* ptr, int type, codegen child){
    param_code *code = (param_code*)ptr;
    if (type == sem_rule_param)
        code->chain = child;
}

CODEGEN_DEC(param_code, sem_rule_param)

void func_code_register_elem(void* ptr, int type, Token elem){
    func_code *code = (func_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void func_code_register_subrule(void* ptr, int type, codegen child){
    func_code *code = (func_code*)ptr;
    if (type == sem_rule_func) code->signature = child;
    if (type == sem_rule_param) code->args = child;
    if (type == sem_rule_scope) code->body = child;
}

CODEGEN_DEC(func_code, sem_rule_func)

void for_code_register_elem(void* ptr, int type, Token elem){

}
    
void for_code_register_subrule(void* ptr, int type, codegen child){
    for_code *code = (for_code*)ptr;
    switch (type) {
        case sem_rule_assign: code->increment = child; break;
        case sem_rule_dec: code->initial = child; break;
        case sem_rule_cond: code->condition = child; break;
        case sem_rule_scope: code->body = child; break;
    }
}

CODEGEN_DEC(for_code, sem_rule_for)

void while_code_register_elem(void* ptr, int type, Token elem){

}

void while_code_register_subrule(void* ptr, int type, codegen child){
    while_code *code = (while_code*)ptr;
    switch (type) {
        case sem_rule_cond: code->condition = child; break;
        case sem_rule_scope: code->body = child; break;
    }
}

CODEGEN_DEC(while_code, sem_rule_while)

void dowhile_code_register_elem(void* ptr, int type, Token elem){

}

void dowhile_code_register_subrule(void* ptr, int type, codegen child){
    dowhile_code *code = (dowhile_code*)ptr;
    switch (type) {
        case sem_rule_cond: code->condition = child; break;
        case sem_rule_scope: code->body = child; break;
    }
}

CODEGEN_DEC(dowhile_code, sem_rule_dowhile)

void var_code_register_elem(void* ptr, int type, Token elem){
    var_code *code = (var_code*)ptr;
    switch (type) {
        case sem_rule_var: code->name = elem; break;
        case sem_rule_op: code->operation = elem; break;
    }
}

void var_code_register_subrule(void* ptr, int type, codegen child){
    var_code *code = (var_code*)ptr;
    if (type == sem_rule_exp)
        code->expression = child;
    else if (type == sem_rule_var)
        code->var = child;
}

CODEGEN_DEC(var_code, sem_rule_var)

void inc_code_register_elem(void *ptr, int type, Token elem){
    inc_code *code = (inc_code*)ptr;
    if (type == sem_rule_inc && *elem.start == '"') code->value = elem;
}

void inc_code_register_subrule(void *ptr, int type, codegen child){
    
}

CODEGEN_DEC(inc_code, sem_rule_inc)

void struct_code_register_elem(void *ptr, int type, Token elem){
    struct_code *code = (struct_code*)ptr;
    if (type == sem_elem_parent) code->parent = elem;
    if (type == sem_elem_name) code->name = elem;
}

void struct_code_register_subrule(void *ptr, int type, codegen child){
    struct_code *code = (struct_code*)ptr;
    code->contents = child;
}

CODEGEN_DEC(struct_code, sem_rule_struct)

void ret_code_register_elem(void *ptr, int type, Token elem){
    
}

void ret_code_register_subrule(void *ptr, int type, codegen child){
    ret_code *code = (ret_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(ret_code, sem_rule_ret)

void def_code_register_elem(void *ptr, int type, Token elem){
    
}

void def_code_register_subrule(void *ptr, int type, codegen child){
    def_code *code = (def_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(def_code, sem_rule_def)

void int_code_register_elem(void *ptr, int type, Token elem){
    struct_code *code = (struct_code*)ptr;
    if (type == sem_elem_name) code->name = elem;
}

void int_code_register_subrule(void *ptr, int type, codegen child){
    struct_code *code = (struct_code*)ptr;
    code->contents = child;
}

CODEGEN_DEC(int_code, sem_rule_interf)

void enum_code_register_elem(void *ptr, int type, Token elem){
    enum_code * code = (enum_code*)ptr;
    if (type == sem_elem_name)
        code->name = elem;
}

void enum_code_register_subrule(void *ptr, int type, codegen child){
    enum_code * code = (enum_code*)ptr;
    if (type == sem_rule_enum_case)
        code->contents = child;
}

CODEGEN_DEC(enum_code, sem_rule_enum)

void enum_case_code_register_elem(void *ptr, int type, Token elem){
    enum_case_code * code = (enum_case_code*)ptr;
    if (type == sem_elem_name)
        code->name = elem;
}

void enum_case_code_register_subrule(void *ptr, int type, codegen child){
    enum_case_code * code = (enum_case_code*)ptr;
    if (type == sem_rule_enum_case)
        code->chain = child;
}

CODEGEN_DEC(enum_case_code, sem_rule_enum_case)

void else_code_register_elem(void* ptr, int type, Token elem){
    
}

void else_code_register_subrule(void* ptr, int type, codegen child){
    else_code *code = (else_code*)ptr;
    code->block = child;
}

CODEGEN_DEC(else_code, sem_rule_else)