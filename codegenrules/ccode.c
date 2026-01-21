#include "general.h"
#include "codeformat.h"

#ifdef CCODEGEN

typedef enum { convenience_type_none, convenience_type_to_string, convenience_type_from_string, convenience_type_to_int, convenience_type_from_int} convenience_type;

typedef struct {
    int context_rule;
    string_slice context_prefix;
    string_slice context_parent;
    bool ignore_semicolon;
    bool has_defer;
    convenience_type convenience;
} emit_context;

emit_context ctx;

emit_context save_and_push_context(emit_context nctx){
    emit_context orig = ctx;
    ctx = nctx;
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
        emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
        emit_const(" = ");
        emit_code(code->initial_value);
        pop_and_restore_context(orig);
    }
    if (!ctx.ignore_semicolon) emit_const(";");
}

void ass_code_emit_code(void *ptr){
    ass_code *code = (ass_code*)ptr;
    emit_token(code->name);//TODO: fetch from symbol table
    emit_const(" = ");
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
    emit_code(code->expression);
    pop_and_restore_context(orig);
    if (!ctx.ignore_semicolon) emit_const(";");
}

void call_code_emit_code(void *ptr){
    call_code *code = (call_code*)ptr;
    emit_token(code->name);
    emit_const("(");
    emit_code(code->args);
    emit_const(")");
    if (!ctx.ignore_semicolon) emit_const(";");
}

void cond_code_emit_code(void *ptr){
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    emit_const("if (");
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
    emit_code(code->cond);
    pop_and_restore_context(orig);
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
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
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
    if ((ctx.context_rule == sem_rule_interf || ctx.context_rule == sem_rule_struct) && slices_equal(token_to_slice(code->type), ctx.context_prefix, false))
        emit_const("struct ");
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
    if (ctx.context_rule == sem_rule_struct){
        switch_block_section(block_section_epilogue);
    }
    func_code *sig;
    if (code->signature.ptr){
        sig = code->signature.ptr;
    } else sig = code;
    if (sig->type.kind){
        emit_token(sig->type);//TODO: fetch from symbol table
        emit_const(" ");
    } else 
        emit_const("void ");
    if (ctx.context_rule == sem_rule_struct){
        emit_slice(ctx.context_prefix);
        emit_const("_");
    }
    if (ctx.context_rule == sem_rule_interf){
        emit_const("(*");
    }
    emit_token(sig->name);//TODO: fetch from symbol table
    if (ctx.context_rule == sem_rule_interf){
        emit_const(")");
    }
    emit_const("(");
    if (ctx.context_rule == sem_rule_struct || ctx.context_rule == sem_rule_interf){
        if (ctx.context_parent.length){
            emit_const("void *parent");
        } else {
            if (ctx.context_rule == sem_rule_interf)
                emit_const("void");
            else 
                emit_slice(ctx.context_prefix);
            emit_const(" *instance");
        }
        if (sig->args.ptr)
            emit_const(", ");
    }
    emit_code(sig->args);
    if (ctx.context_rule == sem_rule_interf){
        emit_const(");");
    } else {
        emit_const("){");
        if (ctx.context_parent.length){
            emit_newline();
            emit_slice(ctx.context_prefix);
            emit_const(" *instance = (");
            emit_slice(ctx.context_prefix);
            emit_const("*)parent;");
            emit_newline();
        }
        emit_block original = save_and_push_block();
        emit_context orig = save_and_push_context((emit_context){ .context_prefix = token_to_slice(code->name), .ignore_semicolon = false, .has_defer = false });
        increase_indent();
        emit_newline();
        emit_code(code->body);
        switch_block_section(block_section_epilogue);
        decrease_indent();
        if (ctx.has_defer){
            switch_block_section(block_section_prologue);
            emit_newline();
            increase_indent();
            emit_const("int _return_val = 0;");
            switch_block_section(block_section_epilogue);
            emit_const("return _return_val;");
            emit_newline();
        }
        emit_const("}");
        emit_newline();
        switch_block_section(block_section_body);
        emit_block fnblock = pop_and_restore_emit_block(original);
        if (orig.context_rule == sem_rule_struct){
            switch_block_section(block_section_epilogue);
        }
        collapse_block(fnblock);
        if (orig.context_rule == sem_rule_struct){
            switch_block_section(block_section_body);
        }
        pop_and_restore_context(orig);
    }
}

void for_code_emit_code(void* ptr){
    for_code *code = (for_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
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
    emit_newline();
}

void struct_code_emit_code(void *ptr){
    struct_code *code = (struct_code*)ptr;
    emit_block original = save_and_push_block();
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_struct, .ignore_semicolon = false, .context_prefix = token_to_slice(code->name), .context_parent = token_to_slice(code->parent) });
    // switch_block_section(block_section_prologue);
    emit_const("typedef struct ");
    emit_token(code->name);
    emit_const(" { ");
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
    if (code->parent.kind){
        emit("/* Parent conformance here. Requires semantic analysis \n\
%v %v_init(){\n\
    return (%v){\n\
        .ptr = zalloc(sizeof(%v)),\n\
        .fn1 = %v_fn1,\n\
        .fn2 = %v_fn2,\n\
        .fn3 = %v_fn3,\n\
    };\n\
}\n\
*/",token_to_slice(code->parent),token_to_slice(code->name),token_to_slice(code->parent),token_to_slice(code->name),token_to_slice(code->name),token_to_slice(code->name),token_to_slice(code->name));
    }
}

void ret_code_emit_code(void *ptr){
    ret_code *code = (ret_code*)ptr;
    if (ctx.has_defer){//TODO: can't know if it has defer until one has been encountered, but maybe that's logically ok?
        emit_const("_return_val = ");
        emit_code(code->expression);
        emit_const(";");
        emit_newline();
        emit_const("goto ");
        emit_slice(ctx.context_prefix);
        emit_const("_defer"); 
    } else {
        emit_const("return ");
        emit_code(code->expression);
    }
    emit_const(";");
}

void def_code_emit_code(void *ptr){
    def_code *code = (def_code*)ptr;
    switch_block_section(block_section_epilogue);
    if (!ctx.has_defer){ 
        emit_slice(ctx.context_prefix);
        emit_const("_defer:"); 
        emit_newline();
    }
    increase_indent();
    emit_code(code->expression);
    decrease_indent();
    emit_newline();
    ctx.has_defer = true;
    switch_block_section(block_section_body);
}

void gen_invoke_param(codegen_t contents){
    param_code *code = (param_code*)contents.ptr;
    if (code->name.length) emit(", %v",token_to_slice(code->name));
    if (code->chain.ptr) gen_invoke_param(code->chain);
}

void gen_int_stubs(codegen_t contents, string_slice name){
    blk_code *scope = (blk_code*)contents.ptr;
    if (scope->stat.ptr && scope->stat.type == sem_rule_func){
        func_code *func = scope->stat.ptr;
        if (func->type.length) emit("%v ",token_to_slice(func->type));
        else emit_const("void ");
        
        emit_slice(name);
        emit_const("_");
        emit_token(func->name);
        emit("(%v instance",name);
        if (func->args.ptr) emit_const(" ,");
        emit_code(func->args);
        emit_const("){");
        increase_indent();
        emit_newline();
        emit("if (instance.%v) instance.%v(instance.ptr",token_to_slice(func->name),token_to_slice(func->name));
        if (func->args.ptr) gen_invoke_param(func->args);
        emit_const(");");
        decrease_indent();
        emit_newline();
        emit_const("}");
        emit_newline();
        
    }
    if (scope->chain.ptr){
        emit_newline();
        gen_int_stubs(scope->chain,name);
    }
}
// void codegen_t_register_subrule(codegen_t gen, int type, codegen_t child){
//     if (gen.register_subrule) gen.register_subrule(gen.ptr, type, child);
// }

void int_code_emit_code(void *ptr){
    int_code *code = (int_code*)ptr;
    emit_block original = save_and_push_block();
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_interf, .ignore_semicolon = false, .context_prefix = token_to_slice(code->name) });
    emit_const("typedef struct ");
    emit_token(code->name);
    emit_const(" { ");
    increase_indent();
    emit_newline();
    emit_const("void* ptr;");
    emit_newline();
    emit_code(code->contents);
    decrease_indent();
    emit_newline();
    emit(" } %v;",token_to_slice(code->name));
    emit_newline();
    emit_block new_b = pop_and_restore_emit_block(original);
    collapse_block(new_b);
    gen_int_stubs(code->contents, token_to_slice(code->name));
    pop_and_restore_context(orig);
}

void enum_code_emit_code(void *ptr){
    enum_code *code = (enum_code*)ptr;
    
    emit_const("typedef enum {");
    increase_indent();
    emit_newline();
    emit_context orig = save_and_push_context((emit_context){ .convenience = convenience_type_none, .context_prefix = token_to_slice(code->name) });
    emit_code(code->contents);
    pop_and_restore_context(orig);
    decrease_indent();
    emit_newline();
    emit_const("} ");
    emit_token(code->name);
    emit_const(";");
    
    emit_newlines(2);
    
    emit_const("static inline char* ");
    emit_token(code->name);
    emit_const("_to_string(");
    emit_token(code->name);
    emit_const(" val){");
    increase_indent();
    emit_newline();
    emit_const("switch (val) {");
    increase_indent();
    emit_newline();
    orig = save_and_push_context((emit_context){ .convenience = convenience_type_to_string, .context_prefix = token_to_slice(code->name) });
    emit_code(code->contents);
    pop_and_restore_context(orig);
    decrease_indent();
    emit_newline();
    emit_const("} ");
    decrease_indent();
    emit_newline();
    emit_const("} ");
    emit_newline();
}

void enum_case_code_emit_code(void *ptr){
    enum_case_code *code = (enum_case_code*)ptr;
    
    switch (ctx.convenience){
        case convenience_type_to_string:
            emit_const("case ");
            emit_slice(ctx.context_prefix);
            emit_const("_");
            emit_token(code->name);
            emit_const(": return \"");
            emit_token(code->name);
            emit_const("\";");
        break;
        case convenience_type_from_string:
            //TODO
        break;
        case convenience_type_to_int: break;
        case convenience_type_from_int:
            //TODO
        break;
        default: 
            emit_slice(ctx.context_prefix);
            emit_const("_");
            emit_token(code->name);
            emit_const(",");
        break;
    }
    if (code->chain.ptr) {
        emit_newline();
        emit_code(code->chain);
    }
    
}

#endif