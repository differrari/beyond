#include "rules.h"
#include "general.h"

#define CODEGEN_DEC(name) \
codegen_t name##_init(){\
    return (codegen_t){\
        .ptr = zalloc(sizeof(name)),\
        .register_elem = name##_register_elem,\
        .register_subrule = name##_register_subrule,\
        .emit_code = name##_emit_code,\
    };\
}

typedef struct {
    codegen_t stat;
    codegen_t chain;
} blk_code;

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

char *blk_code_emit_code(void* ptr){
    blk_code *code = (blk_code*)ptr;
    if (code->chain.ptr){
        return string_format("%s\n%s",emit_code(code->stat),emit_code(code->chain)).data;
    } else {
        return string_format("%s",emit_code(code->stat)).data;
    }
}

CODEGEN_DEC(blk_code)

typedef struct {
    codegen_t statement;
    int type;
} stat_code;

void stat_code_register_elem(void* ptr, int type, Token elem){
    
}

void stat_code_register_subrule(void* ptr, int type, codegen_t child){
    stat_code *code = (stat_code*)ptr;
    code->type = type;
    code->statement = child;
}

char *stat_code_emit_code(void* ptr){
    stat_code *code = (stat_code*)ptr;
    if (!code->statement.ptr){ print("Statement error"); return ""; }
    return string_format("%s",emit_code(code->statement)).data;
}

CODEGEN_DEC(stat_code)

typedef struct {
    Token type;
    Token name;
    codegen_t initial_value;
} dec_code;

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

char *dec_code_emit_code(void* ptr){
    dec_code *code = (dec_code*)ptr;
    return string_format("%v %v = %s;",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->initial_value)).data;
}

CODEGEN_DEC(dec_code)

typedef struct {
    Token name;
    codegen_t expression;
} ass_code;

void ass_code_register_elem(void* ptr, int type, Token elem){
    ass_code *code = (ass_code*)ptr;
    code->name = elem;
}

void ass_code_register_subrule(void* ptr, int type, codegen_t child){
    ass_code *code = (ass_code*)ptr;
    code->expression = child;
}

char* ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    return string_format("%v = %s;",token_to_slice(code->name),emit_code(code->expression)).data;
}

CODEGEN_DEC(ass_code)

typedef struct {
    Token name;
    codegen_t args;
} call_code;

void call_code_register_elem(void* ptr, int type, Token elem){
    call_code *code = (call_code*)ptr;
    code->name = elem;
}

void call_code_register_subrule(void* ptr, int type, codegen_t child){
    call_code *code = (call_code*)ptr;
    code->args = child;
}

char* call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    return string_format("%v(%s);",token_to_slice(code->name),emit_code(code->args)).data;
}

CODEGEN_DEC(call_code)

typedef struct {
    codegen_t cond;
    codegen_t scope;
} cond_code;

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

char* cond_code_emit_code(void *ptr){
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    char *c = emit_code(code->cond);
    char *s = emit_code(code->scope);
    // print("Condition code emit end");
    return string_format("if (%s) { %s }",c,s).data;
}

CODEGEN_DEC(cond_code)

typedef struct {
    Token jump;
} jmp_code;

void jmp_code_register_elem(void* ptr, int type, Token elem){
    jmp_code *code = (jmp_code*)ptr;
    code->jump = elem;
}

void jmp_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

char* jmp_code_emit_code(void *ptr){
    jmp_code *code = (jmp_code*)ptr;
    return string_format("goto %v;",token_to_slice(code->jump)).data;
}

CODEGEN_DEC(jmp_code)

typedef struct {
    Token name;
} label_code;

void label_code_register_elem(void* ptr, int type, Token elem){
    label_code *code = (label_code*)ptr;
    code->name = elem;
}

void label_code_register_subrule(void* ptr, int type, codegen_t child){
    
}

char* label_code_emit_code(void *ptr){
    label_code *code = (label_code*)ptr;
    return string_format("%v:",token_to_slice(code->name)).data;
}

CODEGEN_DEC(label_code)

typedef struct {
    Token val;
    Token operand;
    codegen_t exp;
} exp_code;

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

char* exp_code_emit_code(void *ptr){
    exp_code *code = (exp_code*)ptr;
    if (code->exp.ptr){
        char *exp = emit_code(code->exp);
        return string_format("%v %v %s",token_to_slice(code->val),token_to_slice(code->operand),exp).data;
    } else {
        return string_format("%v",token_to_slice(code->val)).data;
    }
}

CODEGEN_DEC(exp_code)

typedef struct {
    codegen_t exp;
    codegen_t chain;
} arg_code;

void arg_code_register_elem(void* ptr, int type, Token elem){
   
}

void arg_code_register_subrule(void* ptr, int type, codegen_t child){
    arg_code *code = (arg_code*)ptr;
    if (type == sem_args)
        code->chain = child;
    if (type == sem_exp)
        code->exp = child;
}

char* arg_code_emit_code(void *ptr){
    arg_code *code = (arg_code*)ptr;
    if (code->chain.ptr){
        return string_format("%s, %s",emit_code(code->exp),emit_code(code->chain)).data;
    } else {
        return string_format("%s",emit_code(code->exp)).data;
    }
}

CODEGEN_DEC(arg_code)

typedef struct {
    Token type;
    Token name;
    codegen_t chain;
} param_code;

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

char* param_code_emit_code(void *ptr){
    param_code *code = (param_code*)ptr;
    if (code->chain.ptr){
        return string_format("%v %v, %s",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->chain)).data;
    } else {
        return string_format("%v %v",token_to_slice(code->type),token_to_slice(code->name)).data;
    }
}

CODEGEN_DEC(param_code)

typedef struct {
    Token type;
    Token name;
    codegen_t args;
    codegen_t body;
} func_code;

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

char* func_code_emit_code(void *ptr){
    func_code *code = (func_code*)ptr;
    if (!code->body.ptr) return "";
    if (code->args.ptr){
        return string_format("%v %v(%s){ %s }",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->args),emit_code(code->body)).data;
    } else {
        return string_format("%v %v(){ %s }",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->body)).data;
    }
}

CODEGEN_DEC(func_code)