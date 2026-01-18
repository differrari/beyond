#include "general.h"

#ifdef CCODEGEN

//TODO: code emit should actually emit, not return a string. Can also do a formatter with this
char *blk_code_emit_code(void* ptr){
    blk_code *code = (blk_code*)ptr;
    if (code->chain.ptr){
        return string_format("%s\n%s",emit_code(code->stat),emit_code(code->chain)).data;
    } else {
        return string_format("%s",emit_code(code->stat)).data;
    }
}

char *dec_code_emit_code(void* ptr){
    dec_code *code = (dec_code*)ptr;
    return string_format("%v %v = %s;",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->initial_value)).data;
}

char* ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    return string_format("%v = %s;",token_to_slice(code->name),emit_code(code->expression)).data;
}

char* call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    return string_format("%v(%s);",token_to_slice(code->name),emit_code(code->args)).data;
}

char* cond_code_emit_code(void *ptr){
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    char *c = emit_code(code->cond);
    char *s = emit_code(code->scope);
    // print("Condition code emit end");
    return string_format("if (%s) { %s }",c,s).data;
}

char* jmp_code_emit_code(void *ptr){
    jmp_code *code = (jmp_code*)ptr;
    return string_format("goto %v;",token_to_slice(code->jump)).data;
}

char* label_code_emit_code(void *ptr){
    label_code *code = (label_code*)ptr;
    return string_format("%v:",token_to_slice(code->name)).data;
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

char* arg_code_emit_code(void *ptr){
    arg_code *code = (arg_code*)ptr;
    if (code->chain.ptr){
        return string_format("%s, %s",emit_code(code->exp),emit_code(code->chain)).data;
    } else {
        return string_format("%s",emit_code(code->exp)).data;
    }
}

char* param_code_emit_code(void *ptr){
    param_code *code = (param_code*)ptr;
    if (code->chain.ptr){
        return string_format("%v %v, %s",token_to_slice(code->type),token_to_slice(code->name),emit_code(code->chain)).data;
    } else {
        return string_format("%v %v",token_to_slice(code->type),token_to_slice(code->name)).data;
    }
}

char* func_code_emit_code(void *ptr){
    func_code *code = (func_code*)ptr;
    string_slice type = code->type.kind ? token_to_slice(code->type) : slice_from_lit("void");//TODO: this should be handled by the symbol table and we should look up into it
    return string_format("%v %v(%s){ %s }",type,token_to_slice(code->name),emit_code(code->args),emit_code(code->body)).data;
}

#endif