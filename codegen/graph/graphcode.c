#ifdef GRAPHCODEGEN

#include "ir/general.h"
#include "../codeformat.h"
#include "../c/emit_context.h"

bool has_emitted = false;

void generate_code(const char *name, codegen cg){
    emit_const("digraph Code {");
    increase_indent(true);
    codegen_emit_code(cg);
    decrease_indent(true);
    emit_const("}");
    output_code(name,"graph");
}

bool blk_code_emit_code(void* ptr){
    blk_code *code = ptr;
    codegen_emit_code(code->stat);
    codegen_emit_code(code->chain);
    return true;
}

bool dec_code_emit_code(void* ptr){
    dec_code *code = ptr;
    codegen_emit_code(code->initial_value);
    return true;
}

bool ass_code_emit_code(void *ptr){
    ass_code *code = ptr;
    codegen_emit_code(code->var);
    codegen_emit_code(code->expression);
    return true;
}

bool call_code_emit_code(void *ptr){
    call_code *code = ptr;
    if (has_emitted){
        emit_const(",");
        emit_newline();
    }
    emit("%v -> %v",ctx.context_prefix,code->name);
    has_emitted = true;
    return true;
}

bool cond_code_emit_code(void *ptr){
    cond_code *code = ptr;
    codegen_emit_code(code->cond);
    codegen_emit_code(code->scope);
    codegen_emit_code(code->chain);
    return true;
}

bool jmp_code_emit_code(void *ptr){
    return false;
}

bool label_code_emit_code(void *ptr){
    return false;
}

bool exp_code_emit_code(void *ptr){
    exp_code *code = ptr;
    codegen_emit_code(code->var);
    codegen_emit_code(code->exp);
    codegen_emit_code(code->lambda);
    return true;
}

bool arg_code_emit_code(void *ptr){
    arg_code *code = ptr;
    codegen_emit_code(code->exp);
    codegen_emit_code(code->chain);
    return true;
}

bool param_code_emit_code(void *ptr){
    param_code *code = ptr;
    return true;
}

bool func_code_emit_code(void *ptr){
    func_code *code = ptr;
    emit_context orig = save_and_push_context((emit_context){.context_prefix = code->name });
    codegen_emit_code(code->body);
    pop_and_restore_context(orig);
    return true;
}

bool for_code_emit_code(void* ptr){
    for_code *code = ptr;
    codegen_emit_code(code->body);
    return true;
}

bool while_code_emit_code(void* ptr){
    while_code *code = ptr;
    codegen_emit_code(code->condition);
    codegen_emit_code(code->body);
    return true;
}

bool dowhile_code_emit_code(void* ptr){
    dowhile_code *code = ptr;
    codegen_emit_code(code->condition);
    codegen_emit_code(code->body);
    return true;
}

bool var_code_emit_code(void* ptr){
    var_code *code = ptr;
    codegen_emit_code(code->var);
    codegen_emit_code(code->expression);
    return true;
}

bool inc_code_emit_code(void *ptr){
    return false;
}

bool struct_code_emit_code(void *ptr){
    struct_code *code = ptr;
    codegen_emit_code(code->contents);
    return true;
}

bool ret_code_emit_code(void *ptr){
    ret_code *code = ptr;
    codegen_emit_code(code->expression);
    return true;
}

bool def_code_emit_code(void *ptr){
    def_code *code = ptr;
    codegen_emit_code(code->expression);
    return true;
}

bool int_code_emit_code(void *ptr){
    return false;
}

bool enum_code_emit_code(void *ptr){
    return false;
}

bool enum_case_code_emit_code(void *ptr){
    return false;
}

bool else_code_emit_code(void *ptr){
    else_code *code = ptr;
    codegen_emit_code(code->block);
    return true;
}

bool switch_code_emit_code(void *ptr){
    switch_code *code = ptr;
    codegen_emit_code(code->condition);
    codegen_emit_code(code->cases);
    return true;
}

bool case_code_emit_code(void *ptr){
    case_code *code = ptr;
    codegen_emit_code(code->match);
    codegen_emit_code(code->body);
    codegen_emit_code(code->chain);
    return true;
}

bool prop_init_code_emit_code(void *ptr){
    prop_init_code *code = ptr;
    codegen_emit_code(code->expression);
    codegen_emit_code(code->chain);
    return true;
}

bool struct_init_code_emit_code(void *ptr){
    struct_init_code *code = ptr;
    codegen_emit_code(code->content);
    return true;
}

bool cast_code_emit_code(void *ptr){
    return false;
}

bool array_init_code_emit_code(void *ptr){
    array_init_code *code = ptr;
    codegen_emit_code(code->entries);
    return true;
}

bool array_entry_code_emit_code(void *ptr){
    array_entry_code *code = ptr;
    codegen_emit_code(code->exp);
    codegen_emit_code(code->chain);
    return true;
}

bool rule_sequence_code_emit_code(void *ptr){
    return false;
}

bool rule_entry_code_emit_code(void *ptr){
    return false;
}

#endif