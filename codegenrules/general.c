#include "rules.h"
#include "general.h"

#define CODEGEN_DEC(name,t) \
extern void name##_emit_code(void*ptr);\
codegen_t name##_init(){\
    return (codegen_t){\
        .ptr = zalloc(sizeof(name)),\
        .register_elem = name##_register_elem,\
        .register_subrule = name##_register_subrule,\
        .emit_code = name##_emit_code,\
        .type = t,\
    };\
}

void blk_code_register_elem(void* ptr, int type, Token elem){
    
}

void blk_code_register_subrule(void* ptr, int type, codegen_t child){
    blk_code *code = (blk_code*)ptr;
    if (type == sem_scope){
        code->chain = child;
    } else
        code->stat = child;
}

CODEGEN_DEC(blk_code, sem_scope)

void dec_code_register_elem(void* ptr, int type, Token elem){
    dec_code *code = (dec_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void dec_code_register_subrule(void* ptr, int type, codegen_t child){
    dec_code *code = (dec_code*)ptr;
    code->initial_value = child;
}

CODEGEN_DEC(dec_code, sem_dec)

void ass_code_register_elem(void* ptr, int type, Token elem){
    ass_code *code = (ass_code*)ptr;
    code->name = elem;
}

void ass_code_register_subrule(void* ptr, int type, codegen_t child){
    ass_code *code = (ass_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(ass_code, sem_assign)

void call_code_register_elem(void* ptr, int type, Token elem){
    call_code *code = (call_code*)ptr;
    code->name = elem;
}

void call_code_register_subrule(void* ptr, int type, codegen_t child){
    call_code *code = (call_code*)ptr;
    code->args = child;
}

CODEGEN_DEC(call_code, sem_call)

void cond_code_register_elem(void* ptr, int type, Token elem){
    
}

void cond_code_register_subrule(void* ptr, int type, codegen_t child){
    cond_code *code = (cond_code*)ptr;
    // print("REG CONDITION %i",type);
    switch (type){
        case sem_cond: code->cond = child; break;
        case sem_scope: code->scope = child; break;
    }
}

CODEGEN_DEC(cond_code, sem_cond)

void jmp_code_register_elem(void* ptr, int type, Token elem){
    jmp_code *code = (jmp_code*)ptr;
    code->jump = elem;
}

void jmp_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

CODEGEN_DEC(jmp_code, sem_jmp)

void label_code_register_elem(void* ptr, int type, Token elem){
    label_code *code = (label_code*)ptr;
    code->name = elem;
}

void label_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

CODEGEN_DEC(label_code, sem_label)

void exp_code_register_elem(void* ptr, int type, Token elem){
    exp_code *code = (exp_code*)ptr;
    if (type == sem_val)
        code->val = elem;
    else code->operand = elem;
}

void exp_code_register_subrule(void* ptr, int type,  codegen_t child){
    exp_code *code = (exp_code*)ptr;
    if (type == sem_var) code->var = child;
    if (type == sem_exp) code->exp = child;
}

CODEGEN_DEC(exp_code, sem_exp)

void arg_code_register_elem(void* ptr, int type, Token elem){
   
}

void arg_code_register_subrule(void* ptr, int type, codegen_t child){
    arg_code *code = (arg_code*)ptr;
    if (type == sem_args)
        code->chain = child;
    if (type == sem_exp)
        code->exp = child;
}

CODEGEN_DEC(arg_code, sem_args)

void param_code_register_elem(void* ptr, int type, Token elem){
    param_code *code = (param_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void param_code_register_subrule(void* ptr, int type, codegen_t child){
    param_code *code = (param_code*)ptr;
    if (type == sem_param)
        code->chain = child;
}

CODEGEN_DEC(param_code, sem_param)

void func_code_register_elem(void* ptr, int type, Token elem){
    func_code *code = (func_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; break; 
        case sem_elem_name: code->name = elem; break;
    }
}

void func_code_register_subrule(void* ptr, int type, codegen_t child){
    func_code *code = (func_code*)ptr;
    if (type == sem_func) code->signature = child;
    if (type == sem_param) code->args = child;
    if (type == sem_scope) code->body = child;
}

CODEGEN_DEC(func_code, sem_func)

void for_code_register_elem(void* ptr, int type, Token elem){

}
    
void for_code_register_subrule(void* ptr, int type, codegen_t child){
    for_code *code = (for_code*)ptr;
    switch (type) {
        case sem_assign: code->increment = child; break;
        case sem_dec: code->initial = child; break;
        case sem_cond: code->condition = child; break;
        case sem_scope: code->body = child; break;
    }
}

CODEGEN_DEC(for_code, sem_for)

void while_code_register_elem(void* ptr, int type, Token elem){

}

void while_code_register_subrule(void* ptr, int type, codegen_t child){
    while_code *code = (while_code*)ptr;
    switch (type) {
        case sem_cond: code->condition = child; break;
        case sem_scope: code->body = child; break;
    }
}

CODEGEN_DEC(while_code, sem_while)

void dowhile_code_register_elem(void* ptr, int type, Token elem){

}

void dowhile_code_register_subrule(void* ptr, int type, codegen_t child){
    dowhile_code *code = (dowhile_code*)ptr;
    switch (type) {
        case sem_cond: code->condition = child; break;
        case sem_scope: code->body = child; break;
    }
}

CODEGEN_DEC(dowhile_code, sem_dowhile)

void var_code_register_elem(void* ptr, int type, Token elem){
    var_code *code = (var_code*)ptr;
    switch (type) {
        case sem_var: code->name = elem; break;
        case sem_op: code->operation = elem; break;
    }
}

void var_code_register_subrule(void* ptr, int type, codegen_t child){
    var_code *code = (var_code*)ptr;
    if (type == sem_exp)
        code->expression = child;
    else if (type == sem_var)
        code->var = child;
}

CODEGEN_DEC(var_code, sem_var)

void inc_code_register_elem(void *ptr, int type, Token elem){
    inc_code *code = (inc_code*)ptr;
    if (type == sem_inc && *elem.start == '"') code->value = elem;
}

void inc_code_register_subrule(void *ptr, int type, codegen_t child){
    
}

CODEGEN_DEC(inc_code, sem_inc)

void struct_code_register_elem(void *ptr, int type, Token elem){
    struct_code *code = (struct_code*)ptr;
    if (type == sem_elem_parent) code->parent = elem;
    if (type == sem_elem_name) code->name = elem;
}

void struct_code_register_subrule(void *ptr, int type, codegen_t child){
    struct_code *code = (struct_code*)ptr;
    code->contents = child;
}

CODEGEN_DEC(struct_code, sem_struct)

void ret_code_register_elem(void *ptr, int type, Token elem){
    
}

void ret_code_register_subrule(void *ptr, int type, codegen_t child){
    ret_code *code = (ret_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(ret_code, sem_ret)

void def_code_register_elem(void *ptr, int type, Token elem){
    
}

void def_code_register_subrule(void *ptr, int type, codegen_t child){
    def_code *code = (def_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(def_code, sem_def)

void int_code_register_elem(void *ptr, int type, Token elem){
    struct_code *code = (struct_code*)ptr;
    if (type == sem_elem_name) code->name = elem;
}

void int_code_register_subrule(void *ptr, int type, codegen_t child){
    struct_code *code = (struct_code*)ptr;
    code->contents = child;
}

CODEGEN_DEC(int_code, sem_interf)