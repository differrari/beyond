#include "rules.h"
#include "general.h"

#define CODEGEN_DEC(name) \
extern void name##_emit_code(void*ptr);\
codegen_t name##_init(){\
    return (codegen_t){\
        .ptr = zalloc(sizeof(name)),\
        .register_elem = name##_register_elem,\
        .register_subrule = name##_register_subrule,\
        .emit_code = name##_emit_code,\
    };\
}

void blk_code_register_elem(void* ptr, int type, Token elem){
    
}

void blk_code_register_subrule(void* ptr, int type, codegen_t child){
    blk_code *code = (blk_code*)ptr;
    if (type == sem_stat){
        code->stat = child;
    }
    if (type == sem_scope){
        code->chain = child;
    }
}

CODEGEN_DEC(blk_code)

void dec_code_register_elem(void* ptr, int type, Token elem){
    dec_code *code = (dec_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; 
        case sem_elem_name: code->name = elem;
    }
}

void dec_code_register_subrule(void* ptr, int type, codegen_t child){
    dec_code *code = (dec_code*)ptr;
    code->initial_value = child;
}

CODEGEN_DEC(dec_code)

void ass_code_register_elem(void* ptr, int type, Token elem){
    ass_code *code = (ass_code*)ptr;
    code->name = elem;
}

void ass_code_register_subrule(void* ptr, int type, codegen_t child){
    ass_code *code = (ass_code*)ptr;
    code->expression = child;
}

CODEGEN_DEC(ass_code)

void call_code_register_elem(void* ptr, int type, Token elem){
    call_code *code = (call_code*)ptr;
    code->name = elem;
}

void call_code_register_subrule(void* ptr, int type, codegen_t child){
    call_code *code = (call_code*)ptr;
    code->args = child;
}

CODEGEN_DEC(call_code)

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

CODEGEN_DEC(cond_code)

void jmp_code_register_elem(void* ptr, int type, Token elem){
    jmp_code *code = (jmp_code*)ptr;
    code->jump = elem;
}

void jmp_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

CODEGEN_DEC(jmp_code)

void label_code_register_elem(void* ptr, int type, Token elem){
    label_code *code = (label_code*)ptr;
    code->name = elem;
}

void label_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

CODEGEN_DEC(label_code)

void exp_code_register_elem(void* ptr, int type, Token elem){
    exp_code *code = (exp_code*)ptr;
    if (type == sem_var || type == sem_val)
        code->val = elem;
    else code->operand = elem;
}

void exp_code_register_subrule(void* ptr, int type,  codegen_t child){
    exp_code *code = (exp_code*)ptr;
    code->exp = child;
}

CODEGEN_DEC(exp_code)

void arg_code_register_elem(void* ptr, int type, Token elem){
   
}

void arg_code_register_subrule(void* ptr, int type, codegen_t child){
    arg_code *code = (arg_code*)ptr;
    if (type == sem_args)
        code->chain = child;
    if (type == sem_exp)
        code->exp = child;
}

CODEGEN_DEC(arg_code)

void param_code_register_elem(void* ptr, int type, Token elem){
    param_code *code = (param_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; 
        case sem_elem_name: code->name = elem;
    }
}

void param_code_register_subrule(void* ptr, int type, codegen_t child){
    param_code *code = (param_code*)ptr;
    if (type == sem_param)
        code->chain = child;
}

CODEGEN_DEC(param_code)

void func_code_register_elem(void* ptr, int type, Token elem){
    func_code *code = (func_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; 
        case sem_elem_name: code->name = elem;
    }
}

void func_code_register_subrule(void* ptr, int type, codegen_t child){
    func_code *code = (func_code*)ptr;
    if (type == sem_param)
        code->args = child;
    if (type == sem_scope)
        code->body = child;
}

CODEGEN_DEC(func_code)