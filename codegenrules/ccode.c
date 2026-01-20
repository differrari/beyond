#include "general.h"
#include "codeformat.h"

#ifdef CCODEGEN

typedef struct {
    int context_rule;
    string_slice context_prefix;
} emit_context;

emit_context ctx;

emit_context save_and_push_context(int context_rule, string_slice context_prefix){
    emit_context orig = ctx;
    ctx = (emit_context){
        .context_rule = context_rule,
        .context_prefix = context_prefix
    };
    return orig;
}

emit_context pop_and_restore_context(emit_context restore){
    emit_context orig = ctx;
    ctx = restore;
    return orig;
}

void blk_code_emit_code(void* ptr){
    blk_code *code = (blk_code*)ptr;
    emit_code(code->stat);
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
    if (code->initial_value.ptr){
        emit_const(" = ");
        emit_code(code->initial_value);
    }
    if (ctx.context_rule != sem_for) emit_const(";");
}

void ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    emit_token(code->name);//TODO: fetch from symbol table
    emit_const(" = ");
    emit_code(code->expression);
    if (ctx.context_rule != sem_for) emit_const(";");
}

void call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    emit_token(code->name);
    emit_const("(");
    emit_code(code->args);
    emit_const(")");
    if (ctx.context_rule != sem_exp) emit_const(";");
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
    emit_const(";");
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
    emit_context orig = save_and_push_context(sem_exp, make_string_slice(0, 0, 0));
    if (code->var.ptr) emit_code(code->var);
    else emit_token(code->val);
    // emit_const(",");
    if (code->exp.ptr){
        if (code->val.kind || code->var.ptr) emit_space();
        emit_token(code->operand);
        emit_space();
        emit_code(code->exp);
    }
    pop_and_restore_context(orig);
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
    if (ctx.context_rule == sem_struct){
        switch_block_section(block_section_epilogue);
    }
    if (code->type.kind){
        emit_token(code->type);//TODO: fetch from symbol table
        emit_const(" ");
    } else 
        emit_const("void ");
    if (ctx.context_rule == sem_struct){
        emit_slice(ctx.context_prefix);
        emit_const("_");
    }
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
    if (ctx.context_rule == sem_struct){
        switch_block_section(block_section_body);
    }
}

void for_code_emit_code(void* ptr){
    for_code *code = (for_code*)ptr;
    emit_context orig = save_and_push_context(sem_for, make_string_slice(0, 0, 0));
    emit_const("for (");
    emit_code(code->initial);
    emit_const("; ");
    emit_code(code->condition);
    emit_const("; ");
    emit_code(code->increment);
    emit_const(")");
    pop_and_restore_context(orig);
    emit_const("{");
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
    emit_const(");");
}

void var_code_emit_code(void* ptr){
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

void inc_code_emit_code(void *ptr){
    inc_code *code = (inc_code*)ptr;
    emit_const("#include ");
    emit_token(code->value);
}

void struct_code_emit_code(void *ptr){
    struct_code *code = (struct_code*)ptr;
    emit_block original = save_and_push_block();
    emit_context orig = save_and_push_context(sem_struct, token_to_slice(code->name));
    // switch_block_section(block_section_prologue);
    emit_const("typedef struct { ");
    increase_indent();
    emit_code(code->contents);
    decrease_indent();
    emit_newline();
    emit(" } %v;",token_to_slice(code->name));
    emit_newline();
    emit_block new_b = pop_and_restore_emit_block(original);
    pop_and_restore_context(orig);
    collapse_block(new_b);
    // switch_block_section(block_section_body);
}

#endif