#include "ir/general.h"
#include "ir/arch_transformer.h"
#include "syscalls/syscalls.h"
#include "emit_context.h"
#include "string/slice.h"
#include "ir/manual_gen.h"
#include "semantic/sem_analysis.h"
#include "c_syms.h"

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
    TRANSFORM(initial_value);
    return this;
}

codegen ass_code_transform(void *ptr, codegen this){
    ass_code *code = (ass_code*)ptr;
    TRANSFORM(expression);
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
        code->body = wrap_in_block(make_declaration("__return_val", token_to_slice(code->type), (codegen){}), code->body, false);
        code->body = wrap_in_block(make_label("defer"), code->body, true);
        code->body = wrap_in_block(ctx.defer_statements, code->body, true);
        replace_returns(code->body);
        code->body = wrap_in_block(make_return(make_const_exp(slice_from_literal("__return_val"))), code->body, true);
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
    if (code->operation.length){
        if (*code->operation.data == '.' && code->expression.type == sem_rule_call){
            call_code *function = code->expression.ptr;
            function->args = make_argument(code->name.length ? make_literal_expression(code->name) : var_to_exp(code->var), function->args);
            string_slice symname = ((var_code*)code->var.ptr)->name;
            symbol_t *sym = find_symbol(sem_rule_dec, symname);
            if (sym){
                string s = type_name(sym, false, false);
                function->name = slice_from_string(string_format("%S_%v", s, function->name));
            }
            return code->expression;
        }
        else if (*code->operation.data == '['){
            codegen call = call_code_init();
            call_code *function = call.ptr;
            string_slice symname = code->name.length ? code->name : ((var_code*)code->var.ptr)->name;
            symbol_t *sym = find_symbol(sem_rule_dec, symname);
            if (sym){
                string s = type_name(sym, false, false);
                string cast = string_format("*(%S*)",type_name(sym, true, false));//TODO: the cast is hacked into the function name, is there a better way without explicit casts?
                function->name = slice_from_string(string_format("%s%S_get",sym->resolved_subtype ? cast.data : "",s));
            }
            function->args = make_argument(code->name.length ? make_literal_expression(code->name) : var_to_exp(code->var), make_argument(code->expression,function->args));
            return call;
        }
    }
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