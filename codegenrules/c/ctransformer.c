#include "../general.h"
#include "ir/arch_transformer.h"
#include "syscalls/syscalls.h"
#include "emit_context.h"
#include "string/slice.h"

#ifdef CCODEGEN

#define TRANSFORM(name) if (code->name.ptr) code->name = codegen_transform(code->name, code->name)

codegen blk_code_transform(void *ptr, codegen this){
    blk_code *code = (blk_code*)ptr;
    TRANSFORM(stat);
    TRANSFORM(chain);
    return this;
}

codegen dec_code_transform(void *ptr, codegen this){
    dec_code *code = (dec_code*)ptr;
    return this;
}

codegen ass_code_transform(void *ptr, codegen this){
    ass_code *code = (ass_code*)ptr;
    return this;
}

codegen exp_code_transform(void *ptr, codegen this){
    exp_code *code = (exp_code*)ptr;
    if (code->lambda.ptr){
        print("Need to transform");
    }
    TRANSFORM(var);
    TRANSFORM(exp);
    return this;
}

codegen call_code_transform(void *ptr, codegen this){
    call_code *code = (call_code*)ptr;
    TRANSFORM(args);
    return this;
}

codegen arg_code_transform(void *ptr, codegen this){
    arg_code *code = (arg_code*)ptr;
    TRANSFORM(chain);
    TRANSFORM(exp);
    return this;
}

codegen cond_code_transform(void *ptr, codegen this){
    cond_code *code = (cond_code*)ptr;
    TRANSFORM(cond);
    TRANSFORM(chain);
    TRANSFORM(scope);
    return this;
}

codegen jmp_code_transform(void *ptr, codegen this){
    jmp_code *code = (jmp_code*)ptr;
    return this;
}

codegen label_code_transform(void *ptr, codegen this){
    label_code *code = (label_code*)ptr;
    return this;
}

codegen param_code_transform(void *ptr, codegen this){
    param_code *code = (param_code*)ptr;
    TRANSFORM(chain);
    return this;
}

codegen wrap_in_block(codegen statement, codegen chain, bool tail){
    codegen blk_ret_value = blk_code_init();
    blk_code *code = (blk_code*)blk_ret_value.ptr;
    code->stat = statement;
    if (tail && chain.ptr){
        blk_code *block = chain.ptr;
        while (block->chain.ptr){
            block = block->chain.ptr;
        }
        block->chain = blk_ret_value;
        return chain;
    } else code->chain = chain;
    return blk_ret_value;
}

void replace_returns(codegen body){
    blk_code *code = body.ptr;
    
    if (code->stat.type == sem_rule_ret){
        ret_code *ret = code->stat.ptr;
        
        codegen goto_code = jmp_code_init();
        jmp_code *go = goto_code.ptr;
        go->jump = slice_from_literal("defer");
        
        codegen assign_code = ass_code_init();
        ass_code *ass = assign_code.ptr;
        ass->name = slice_from_literal("__return_val");
        ass->expression = ret->expression;
        
        codegen b = wrap_in_block(goto_code, (codegen){}, false);
        b = wrap_in_block(assign_code, b, false);
        
        code->stat = b;
        
    }
    
    if (code->chain.ptr) replace_returns(code->chain);
    
}

codegen make_const_exp(string_slice exp){
    codegen exp_codegen = exp_code_init();
    exp_code *code = exp_codegen.ptr;
    code->val = exp;
    return exp_codegen;
}

codegen func_code_transform(void *ptr, codegen this){
    func_code *code = (func_code*)ptr;
    
    if (code->signature.ptr){
        func_code *sig = code->signature.ptr;
        code->type = sig->type;
        code->name = sig->name;
        code->args = sig->args;
        release(sig);
        code->signature = (codegen){};
    }
    
    TRANSFORM(args);
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_func });
    TRANSFORM(body);
    if (ctx.defer_statements.ptr){
        codegen def_ret_value = dec_code_init();
        dec_code *declaration = (dec_code*)def_ret_value.ptr;
        declaration->name = slice_from_literal("__return_val");
        declaration->type = code->type;
        code->body = wrap_in_block(def_ret_value, code->body, false);
        
        codegen def_label = label_code_init();
        label_code *label = (label_code*)def_label.ptr;
        label->name = slice_from_literal("defer");
        code->body = wrap_in_block(def_label, code->body, true);
        
        code->body = wrap_in_block(ctx.defer_statements, code->body, true);
        
        replace_returns(code->body);
        
        codegen ret_ret_value = ret_code_init();
        ret_code *retstat = (ret_code*)ret_ret_value.ptr;
        retstat->expression = make_const_exp(slice_from_literal("__return_val"));
        code->body = wrap_in_block(ret_ret_value, code->body, true);
    }
    pop_and_restore_context(orig);
    
    return this;
}

codegen for_code_transform(void *ptr, codegen this){
    for_code *code = (for_code*)ptr;
    TRANSFORM(initial);
    TRANSFORM(increment);
    TRANSFORM(condition);
    TRANSFORM(body);
    return this;
}

codegen while_code_transform(void *ptr, codegen this){
    while_code *code = (while_code*)ptr;
    TRANSFORM(condition);
    TRANSFORM(body);
    return this;
}

codegen dowhile_code_transform(void *ptr, codegen this){
    dowhile_code *code = (dowhile_code*)ptr;
    TRANSFORM(condition);
    TRANSFORM(body);
    return this;
}

codegen var_code_transform(void *ptr, codegen this){
    var_code *code = (var_code*)ptr;
    TRANSFORM(var);
    TRANSFORM(expression);
    return this;
}

codegen inc_code_transform(void *ptr, codegen this){
    inc_code *code = (inc_code*)ptr;
    return this;
}

codegen struct_code_transform(void *ptr, codegen this){
    struct_code *code = (struct_code*)ptr;
    TRANSFORM(contents);
    return this;
}

codegen ret_code_transform(void *ptr, codegen this){
    ret_code *code = (ret_code*)ptr;
    TRANSFORM(expression);
    return this;
}

codegen def_code_transform(void *ptr, codegen this){
    def_code *code = (def_code*)ptr;
    TRANSFORM(expression);
    ctx.defer_statements = wrap_in_block(code->expression, ctx.defer_statements, true);
    return (codegen){};
}

codegen int_code_transform(void *ptr, codegen this){
    int_code *code = (int_code*)ptr;
    TRANSFORM(contents);
    return this;
}

codegen enum_code_transform(void *ptr, codegen this){
    enum_code *code = (enum_code*)ptr;
    TRANSFORM(contents);
    return this;
}

codegen enum_case_code_transform(void *ptr, codegen this){
    enum_case_code *code = (enum_case_code*)ptr;
    TRANSFORM(chain);
    return this;
}

codegen else_code_transform(void *ptr, codegen this){
    else_code *code = (else_code*)ptr;
    TRANSFORM(block);
    return this;
}

codegen switch_code_transform(void *ptr, codegen this){
    switch_code *code = (switch_code*)ptr;
    TRANSFORM(condition);
    TRANSFORM(cases);
    return this;
}

codegen case_code_transform(void *ptr, codegen this){
    case_code *code = (case_code*)ptr;
    TRANSFORM(match);
    TRANSFORM(body);
    TRANSFORM(chain);
    return this;
}

codegen perform_transformations(codegen root){
    return codegen_transform(root,root);
}

#endif