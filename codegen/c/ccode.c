#include "ir/general.h"
#include "../codeformat.h"
#include "semantic/sem_analysis.h"
#include "emit_context.h"
#include "syscalls/syscalls.h"
#include "c_syms.h"

#ifdef CCODEGEN

tern is_header = false;

void generate_code(const char *name, codegen cg){
    
    int32_t ext = str_has_char(name, 0, '.');
    if (ext >= 0){
        is_header = -1;
        codegen_emit_code(cg);
        output_code(name,"");
    } else {
        is_header = true;
        emit("#pragma once\n\n",name);
        codegen_emit_code(cg);
        output_code(name,"h");
        
        reset_emit_buffer();
        
        is_header = false;
        emit("#include \"%s.h\"\n\n",name);
        codegen_emit_code(cg);
        output_code(name,"c");
    }
}

void blk_code_emit_code(void* ptr){
    blk_code *code = (blk_code*)ptr;
    codegen_emit_code(code->stat);
    if (code->chain.ptr){ 
        if (code->stat.type) emit_newline(); 
        codegen_emit_code(code->chain); 
    }
}

void emit_type(symbol_t *sym, bool extra){
    string s = type_name(sym, false, extra);
    emit_slice(slice_from_string(s));
    string_free(s);
}

void dec_code_emit_code(void* ptr){
    dec_code *code = (dec_code*)ptr;
    if (is_header == false && (ctx.context_rule == sem_rule_struct || ctx.context_rule == sem_rule_interf)) return;
    if (is_header == true && ctx.context_rule != sem_rule_struct && ctx.context_rule != sem_rule_interf)
        emit_const("extern ");
    emit_slice(code->type);
    emit_space();
    emit_slice(code->name);
    if (is_header != true && code->initial_value.ptr){
        emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_dec, .ignore_semicolon = true });
        emit_const(" = ");
        codegen_emit_code(code->initial_value);
        pop_and_restore_context(orig);
    }
    if (!ctx.ignore_semicolon) emit_const(";");
}

void ass_code_emit_code(void *ptr){
    if (is_header == true) return;
    ass_code *code = (ass_code*)ptr;
    FIND_SLICE(sem_rule_dec, code->name);
    if (sym->table_type == sem_rule_struct){//CTRANS
        emit_const("instance->");
    }
    emit_slice(sym->name);
    emit_const(" = ");
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_assign, .ignore_semicolon = true });
    codegen_emit_code(code->expression);
    pop_and_restore_context(orig);
    if (!ctx.ignore_semicolon) emit_const(";");
}

void call_code_emit_code(void *ptr){
    if (is_header == true) return;
    call_code *code = (call_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_call, .ignore_semicolon = true });
    emit_slice(code->name);
    emit_const("(");
    codegen_emit_code(code->args);
    emit_const(")");
    pop_and_restore_context(orig);
    if (!ctx.ignore_semicolon) emit_const(";");
}

void cond_code_emit_code(void *ptr){
    if (is_header == true) return;
    cond_code *code = (cond_code*)ptr;
    // print("Condition code emit being %i",(*(codegen_t*)code->cond).ptr);
    emit_const("if (");
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
    codegen_emit_code(code->cond);
    pop_and_restore_context(orig);
    emit_const("){");
    increase_indent();
    emit_newline();
    codegen_emit_code(code->scope);
    decrease_indent();
    emit_newline();
    emit_const("}");
    if (code->chain.ptr) codegen_emit_code(code->chain);
    else emit_newline();
}

void jmp_code_emit_code(void *ptr){
    if (is_header == true) return;
    jmp_code *code = (jmp_code*)ptr;
    emit_const("goto ");
    emit_slice(code->jump);
    emit_const(";");
}

void label_code_emit_code(void *ptr){
    if (is_header == true) return;
    label_code *code = (label_code*)ptr;
    emit_const("\r");
    emit_slice(code->name);
    emit_const(":");
}

void exp_code_emit_code(void *ptr){
    if (is_header == true) return;
    exp_code *code = (exp_code*)ptr;
    if (code->paren)
        emit_const("(");
    if (code->invert)
        emit_const("!");
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
    if (code->var.ptr) codegen_emit_code(code->var);
    else emit_slice(code->val);
    if (code->exp.ptr){
        if (code->val.length || code->var.ptr) emit_space();
        if (code->operand.kind){
            emit_token(code->operand);
            emit_space();
        }
        codegen_emit_code(code->exp);
    }
    if (code->paren)
        emit_const(")");
    pop_and_restore_context(orig);
}

void arg_code_emit_code(void *ptr){
    if (is_header == true) return;
    arg_code *code = (arg_code*)ptr;
    codegen_emit_code(code->exp);
    if (code->chain.ptr){
        emit_const(", ");
        codegen_emit_code(code->chain);
    }
}

void param_code_emit_code(void *ptr){
    param_code *code = (param_code*)ptr;
    emit_slice(code->type);
    emit_space();
    emit_slice(code->name);
    if (code->chain.ptr){
        emit_const(", ");
        codegen_emit_code(code->chain);
    }
}

void func_code_emit_code(void *ptr){
    func_code *code = (func_code*)ptr;
    if (code->type.length){
        emit_slice(code->type);
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
    emit_slice(code->name);
    if (ctx.context_rule == sem_rule_interf){
        emit_const(")");
    }
    emit_const("(");
    codegen_emit_code(code->args);
    if (ctx.context_rule == sem_rule_interf || is_header == true){
        emit_const(");");
    } else {
        emit_const("){");
        emit_context orig = save_and_push_context((emit_context){ .context_prefix = code->name, .ignore_semicolon = false });
        increase_indent();
        emit_newline();
        codegen_emit_code(code->body);
        decrease_indent();
        emit_newline();
        emit_const("}");
        emit_newline();
        pop_and_restore_context(orig);
    }
}

void for_code_emit_code(void* ptr){
    if (is_header == true) return;
    for_code *code = (for_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){ .ignore_semicolon = true });
    emit_const("for (");
    codegen_emit_code(code->initial);
    emit_const("; ");
    codegen_emit_code(code->condition);
    emit_const("; ");
    codegen_emit_code(code->increment);
    emit_const(")");
    pop_and_restore_context(orig);
    emit_const("{");
    increase_indent();
    if (code->body.ptr) codegen_emit_code(code->body);
    decrease_indent();
    emit_const("}");
}

void while_code_emit_code(void* ptr){
    if (is_header == true) return;
    while_code *code = (while_code*)ptr;
    emit_const("while (");
    codegen_emit_code(code->condition);
    emit_const("){");
    increase_indent();
    if (code->body.ptr) codegen_emit_code(code->body);
    decrease_indent();
    emit_const("}");
}

void dowhile_code_emit_code(void* ptr){
    if (is_header == true) return;
    dowhile_code *code = (dowhile_code*)ptr;
    emit_const("do {");
    increase_indent();
    if (code->body.ptr) codegen_emit_code(code->body);
    decrease_indent();
    emit_const("}");
    emit_const("while (");
    codegen_emit_code(code->condition);
    emit_const(");");
}

string_slice resolve_symbol_name(var_code *code){
    if (code->var.ptr){
      var_code *inner_code = (var_code*)code->var.ptr;  
      return inner_code->name;
    } else return code->name;
}

bool emit_variable(var_code *code);

bool emit_var_type_resolution(var_code *code, string_slice access_name, bool ret_type){
    call_code *call = (call_code*)code->expression.ptr;
    
    string_slice name_slice = resolve_symbol_name(code);
    
    FIND_SLICE(sem_rule_dec, name_slice);
    
    if (ret_type && sym->subtype.kind){
        emit("*(%v*)",token_to_slice(sym->subtype));//TODO: should know if this cast is needed based on the subtype and the function's return type
    }
    
    emit_type(sym,false);
    
    emit_const("_");
    
    emit_slice(access_name.length ? access_name : call->name);
    
    emit_const("(");
    if (code->var.ptr) emit_variable(code->var.ptr); 
    else { 
        FIND_SLICE(sem_rule_dec, code->name);
        if (sym->table_type == sem_rule_struct){
            emit_const("instance->");
        }
        emit_slice(sym->name);
    }
    if (call->args.ptr){
        emit_const(", ");
        codegen_emit_code(call->args);
    }
    emit_const(")");
    return sym->reference_type;
}

bool emit_variable(var_code *code){//CTRANS
    bool is_ref = false;
    if (code->var.ptr) is_ref = emit_variable(code->var.ptr); else { 
        FIND_SLICE(sem_rule_dec, code->name);
        is_ref = sym->reference_type;
        if (code->transform == var_deref){
            emit_const("*");
            is_ref = false;
        }
        if (code->transform == var_addr){
            emit_const("&");
            is_ref = true;
        }
        if (sym->table_type == sem_rule_struct){
            emit_const("instance->");
        }
        emit_slice(sym->name);
    }
    if (code->operation.length) emit_slice(code->operation);
    if (code->expression.ptr){
        emit_context orig = save_and_push_context((emit_context){.ignore_semicolon = true});
        codegen_emit_code(code->expression);
        pop_and_restore_context(orig);
    } 
    return is_ref;
}

void var_code_emit_code(void* ptr){
    if (is_header == true) return;
    var_code *code = (var_code*)ptr;
    emit_variable(code);
    if (!ctx.ignore_semicolon) emit_const(";");
}

void inc_code_emit_code(void *ptr){
    if (is_header == false) return;
    inc_code *code = (inc_code*)ptr;
    emit_const("#include ");
    emit_token(code->value);
    emit_newline();
}

void struct_code_emit_code(void *ptr){
    struct_code *code = (struct_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_struct, .ignore_semicolon = false, .context_prefix = code->name, .context_parent = token_to_slice(code->parent) });

    if (is_header != false){
        emit_const("typedef struct ");
        emit_slice(code->name);
        emit_const(" { ");
        increase_indent();
        codegen_emit_code(code->contents);
        decrease_indent();
        emit_newline();
        emit(" } %v;",code->name);
    } else codegen_emit_code(code->contents);
    emit_newline();
    pop_and_restore_context(orig);

    if (code->parent.kind){//CTRANS
        emit("%v %v_init()",token_to_slice(code->parent),code->name);
        if (is_header == true){
            emit_const(";");
            emit_newline();
        } 
        else {
            emit_const("{");
            increase_indent();
            emit_newline();
            emit("return (%v){",token_to_slice(code->parent));
            increase_indent();
            emit_newline();
            emit(".ptr = zalloc(sizeof(%v)),",code->name);
            symbol_t *parent_sym = find_symbol(sem_rule_interf, token_to_slice(code->parent));
            if (!parent_sym || !parent_sym->child){
                print("Error: interface %v not found. Should've done better semantic analysis. smh",token_to_slice(code->parent));
                return;
            }
            symbol_table *table = parent_sym->child;
            for (int i = 0; i < table->symbol_count; i++){
                symbol_t *sym = &table->symbol_table[i];
                if (sym->name.length && sym->sym_type == sem_rule_func){
                    emit_newline();
                    emit(".%v = %v_%v,",sym->name,code->name,sym->name);
                }
            }
            decrease_indent();
            emit_newline();
            emit_const("};");
            decrease_indent();
            emit_newline();
            emit_const("}");
        }
    }
}

void ret_code_emit_code(void *ptr){
    if (is_header == true) return;
    ret_code *code = (ret_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){.context_rule = sem_rule_ret, .ignore_semicolon = true });
    emit_const("return");
    if (code->expression.ptr){
        emit_const(" ");
        codegen_emit_code(code->expression);
    }
    pop_and_restore_context(orig);
    emit_const(";");
}

void def_code_emit_code(void *ptr){
    return;
}

void gen_invoke_param(codegen contents){
    param_code *code = (param_code*)contents.ptr;
    if (code->name.length) emit(", %v",code->name);
    if (code->chain.ptr) gen_invoke_param(code->chain);
}

void int_code_emit_code(void *ptr){
    int_code *code = (int_code*)ptr;
    emit_context orig = save_and_push_context((emit_context){ .context_rule = sem_rule_interf, .ignore_semicolon = false, .context_prefix = code->name });
    if (is_header != false){
        emit_const("typedef struct ");
        emit_slice(code->name);
        emit_const(" ");
        emit_slice(code->name);
        emit_const(";");
        emit_newline();
        emit_const("typedef struct ");
        emit_slice(code->name);
        emit_const(" { ");
        increase_indent();
        emit_newline();
        emit_const("void* ptr;");
        emit_newline();
        codegen_emit_code(code->contents);
        decrease_indent();
        emit_newline();
        emit(" } %v;",code->name);
        emit_newline();
    }
    pop_and_restore_context(orig);
}

void enum_code_emit_code(void *ptr){
    enum_code *code = (enum_code*)ptr;
    //TODO: turn into array and use const char*
    if (is_header != false){
        emit_const("typedef enum {");
        increase_indent();
        emit_newline();
        emit_context orig = save_and_push_context((emit_context){ .convenience = convenience_type_none, .context_prefix = token_to_slice(code->name) });
        codegen_emit_code(code->contents);
        pop_and_restore_context(orig);
        decrease_indent();
        emit_newline();
        emit_const("} ");
        emit_token(code->name);
        emit_const(";");
    }
    
    emit_newlines(2);
    
    emit_const("char* ");
    emit_token(code->name);
    emit_const("_to_string(");//CTRANS
    emit_token(code->name);
    emit_const(" val)");
    if (is_header == true){
        emit_const(";");
        emit_newline();
    } else {
        emit_const("{");
        increase_indent();
        emit_newline();
        emit_const("switch (val) {");
        increase_indent();
        emit_newline();
        emit_context orig = save_and_push_context((emit_context){ .convenience = convenience_type_to_string, .context_prefix = token_to_slice(code->name) });
        codegen_emit_code(code->contents);
        pop_and_restore_context(orig);
        decrease_indent();
        emit_newline();
        emit_const("} ");
        decrease_indent();
        emit_newline();
        emit_const("} ");
        emit_newline();
    }
}

void enum_case_code_emit_code(void *ptr){
    enum_case_code *code = (enum_case_code*)ptr;
    
    switch (ctx.convenience){
        case convenience_type_to_string:
            if (is_header == true) break;
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
        codegen_emit_code(code->chain);
    }
    
}

void else_code_emit_code(void *ptr){
    else_code *code = (else_code*)ptr;
    emit_const(" else ");
    if (code->block.type == sem_rule_cond){
        codegen_emit_code(code->block);
    } else {
        emit_const("{");
        increase_indent();
        emit_newline();
        codegen_emit_code(code->block);
        decrease_indent();
        emit_newline();
        emit_const("}");
    }
}

void switch_code_emit_code(void *ptr){
    switch_code *code = (switch_code*)ptr;
    emit_const("switch (");
    codegen_emit_code(code->condition);
    emit_const("){");
    increase_indent();
    emit_newline();
    codegen_emit_code(code->cases);
    decrease_indent();
    emit_newline();
    emit_const("}");
}

void case_code_emit_code(void *ptr){
    case_code *code = (case_code*)ptr;
    
    emit_const("case ");
    codegen_emit_code(code->match);
    emit_const(": {");
    codegen_emit_code(code->body);
    emit_const("} break;");
    if (code->chain.ptr){
        emit_newline();
        codegen_emit_code(code->chain);
    }
}

#endif