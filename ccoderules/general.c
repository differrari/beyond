#include "rules.h"
#include "general.h"

#define CODEGEN_DEC(name) \
codegen_t name##_init(){\
    return (codegen_t){\
        .ptr = malloc(sizeof(name)),\
        .register_elem = name##_register_elem,\
        .register_subrule = name##_register_subrule,\
        .emit_code = name##_emit_code,\
    };\
}

typedef struct {
    Token type;
    Token name;
    void *initial_value;
} dec_code;

void dec_code_register_elem(void* ptr, int type, Token elem){
    dec_code *code = (dec_code*)ptr;
    switch (type) {
        case sem_elem_type: code->type = elem; 
        case sem_elem_name: code->name = elem;
    }
}

void dec_code_register_subrule(void* ptr, int type, void *child){
    dec_code *code = (dec_code*)ptr;
    code->initial_value = child;
}

char *dec_code_emit_code(void* ptr){
    dec_code *code = (dec_code*)ptr;
    return string_format("%v %v = %s",token_to_slice(code->type),token_to_slice(code->name),emit_code(*(codegen_t*)code->initial_value)).data;
}

CODEGEN_DEC(dec_code)

typedef struct {
    Token name;
    void *expression;
} ass_code;

void ass_code_register_elem(void* ptr, int type, Token elem){
    ass_code *code = (ass_code*)ptr;
    code->name = elem;
}

void ass_code_register_subrule(void* ptr, int type, void *child){
    ass_code *code = (ass_code*)ptr;
    code->expression = child;
}

char* ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    return string_format("%v = %s",token_to_slice(code->name),emit_code(*(codegen_t*)code->expression)).data;
}

CODEGEN_DEC(ass_code)

typedef struct {
    Token name;
    void *args;
} call_code;

void call_code_register_elem(void* ptr, int type, Token elem){
    call_code *code = (call_code*)ptr;
    code->name = elem;
}

void call_code_register_subrule(void* ptr, int type, void *child){
    call_code *code = (call_code*)ptr;
    code->args = child;
}

char* call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    return string_format("%v(ARGS_NOT_IMPLEMENTED)",token_to_slice(code->name)).data;
}

CODEGEN_DEC(call_code)

typedef struct {
    void *cond;
    void *scope;
} cond_code;

void cond_code_register_elem(void* ptr, int type, Token elem){
    
}

void cond_code_register_subrule(void* ptr, int type, void *child){
    cond_code *code = (cond_code*)ptr;
    // print("REG CONDITION %i",type);
    switch (type){
        case sem_cond: code->cond = child;
        case sem_scope: code->scope = child;
    }
}

char* cond_code_emit_code(void *ptr){
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    char *c = emit_code(*(codegen_t*)code->cond);
    char *s = emit_code(*(codegen_t*)code->scope);
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

void jmp_code_register_subrule(void* ptr, int type, void *child){
    
}

char* jmp_code_emit_code(void *ptr){
    jmp_code *code = (jmp_code*)ptr;
    return string_format("goto %v",token_to_slice(code->jump)).data;
}

CODEGEN_DEC(jmp_code)

typedef struct {
    Token name;
} label_code;

void label_code_register_elem(void* ptr, int type, Token elem){
    label_code *code = (label_code*)ptr;
    code->name = elem;
}

void label_code_register_subrule(void* ptr, int type, void *child){
    
}

char* label_code_emit_code(void *ptr){
    label_code *code = (label_code*)ptr;
    return string_format("%v:",token_to_slice(code->name)).data;
}

CODEGEN_DEC(label_code)

typedef struct {
    Token val;
    Token operand;
    void *exp;
} exp_code;

void exp_code_register_elem(void* ptr, int type, Token elem){
    exp_code *code = (exp_code*)ptr;
    if (type == sem_var || type == sem_val)
        code->val = elem;
    else code->operand = elem;
}

void exp_code_register_subrule(void* ptr, int type, void *child){
    exp_code *code = (exp_code*)ptr;
    code->exp = child;
}

char* exp_code_emit_code(void *ptr){
    exp_code *code = (exp_code*)ptr;
    // print("EMIT EXP");
    if (code->exp){
        return string_format("%v %v %s",token_to_slice(code->val),token_to_slice(code->operand),emit_code(*(codegen_t*)code->exp)).data;
    } else {
        return string_format("%v",token_to_slice(code->val)).data;
    }
}

CODEGEN_DEC(exp_code)