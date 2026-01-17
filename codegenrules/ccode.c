#include "general.h"
#include "codeformat.h"

#ifdef CCODEGEN

void blk_code_emit_code(void* ptr){
    blk_code *code = (blk_code*)ptr;
    emit_code(code->stat);
    emit_const(";");//TODO: not fully correct, lots of unnecessary semicolons, but better than duplicated ones that can break code
    if (code->chain.ptr){ 
        emit_newline(); 
        emit_code(code->chain); 
    }
}

void dec_code_emit_code(void* ptr){
    dec_code *code = (dec_code*)ptr;
    emit_token(code->type);//TODO: fetch from symbol table
    emit_space();
    emit_token(code->name);//TODO: fetch from symbol table
    emit_const(" = ");
    emit_code(code->initial_value);
}

void ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    emit_token(code->name);//TODO: fetch from symbol table
    emit_const(" = ");
    emit_code(code->expression);
}

void call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    emit_token(code->name);
    emit_const("(");
    emit_code(code->args);
    emit_const(")");
}

void cond_code_emit_code(void *ptr){
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    emit_const("if (");
    emit_code(code->cond);
    emit_const("){");
    increase_indent();
    emit_newline();
    emit_code(code->scope);
    decrease_indent();
    emit_newline();
    emit_const("}");
    emit_newline();
}

void jmp_code_emit_code(void *ptr){
    jmp_code *code = (jmp_code*)ptr;
    emit_const("goto ");
    emit_token(code->jump);
}

void label_code_emit_code(void *ptr){
    label_code *code = (label_code*)ptr;
    emit_const("\r");
    emit_token(code->name);
    emit_const(":");
}

void exp_code_emit_code(void *ptr){
    exp_code *code = (exp_code*)ptr;
    //TODO: fetch from symbol table
    // emit_const("(");
    if (code->var.ptr) emit_code(code->var);
    else emit_token(code->val);
    // emit_const(",");
    if (code->exp.ptr){
        if (code->val.kind || code->var.ptr) emit_space();
        emit_token(code->operand);
        emit_space();
        emit_code(code->exp);
    }
    // emit_const(")");
}

void arg_code_emit_code(void *ptr){
    arg_code *code = (arg_code*)ptr;
    emit_code(code->exp);
    if (code->chain.ptr){
        emit_const(", ");
        emit_code(code->chain);
    }
}

void param_code_emit_code(void *ptr){
    param_code *code = (param_code*)ptr;
    emit_token(code->type);//TODO: fetch from symbol table
    emit_space();
    emit_token(code->name);//TODO: fetch from symbol table
    if (code->chain.ptr){
        emit_const(", ");
        emit_code(code->chain);
    }
}

void func_code_emit_code(void *ptr){
    func_code *code = (func_code*)ptr;
    if (code->type.kind){
        emit_token(code->type);//TODO: fetch from symbol table
        emit_const(" ");
    } else 
        emit_const("void ");
    emit_token(code->name);//TODO: fetch from symbol table
    emit_const("(");
    emit_code(code->args);
    emit_const("){");
    increase_indent();
    emit_newline();
    emit_code(code->body);
    decrease_indent();
    emit_newline();
    emit_const("}");
    emit_newline();
}

void for_code_emit_code(void* ptr){
    for_code *code = (for_code*)ptr;
    emit_const("for (");
    emit_code(code->initial);
    emit_const("; ");
    emit_code(code->condition);
    emit_const("; ");
    emit_code(code->increment);
    emit_const("){");
    increase_indent();
    if (code->body.ptr) emit_code(code->body);
    decrease_indent();
    emit_const("}");
}
void while_code_emit_code(void* ptr){
    while_code *code = (while_code*)ptr;
    emit_const("while (");
    emit_code(code->condition);
    emit_const("){");
    increase_indent();
    if (code->body.ptr) emit_code(code->body);
    decrease_indent();
    emit_const("}");
}
void dowhile_code_emit_code(void* ptr){
    dowhile_code *code = (dowhile_code*)ptr;
    emit_const("do {");
    increase_indent();
    if (code->body.ptr) emit_code(code->body);
    decrease_indent();
    emit_const("}");
    emit_const("while (");
    emit_code(code->condition);
    emit_const(")");
}

void var_code_emit_code(void* ptr, int type, Token elem){
    var_code *code = (var_code*)ptr;
    if (code->var.ptr) emit_code(code->var); else emit_token(code->name);
    if (code->operation.kind){
        if (*code->operation.start == '['){
            emit_const("[");
            emit_code(code->expression);
            emit_const("]");
        } else if (*code->operation.start == '.'){
            emit_const(".");//TODO: could be pointer
            emit_code(code->expression);
        } else print("UNKNOWN OPERATION %v",token_to_slice(code->operation));
    } else if (code->expression.ptr) emit_code(code->expression);
}

#endif