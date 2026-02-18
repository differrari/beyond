#ifdef RULECODEGEN

#include "ir/general.h"
#include "codegen/codeformat.h"

void generate_code(const char *name, codegen cg){
    emit_const("export default grammar({");
    increase_indent(true);
    emit_const("name: \"cred\",");
    emit_newline();
    emit_const("rules: {");
    increase_indent(true);
    codegen_emit_code(cg);
    decrease_indent(false);
    emit_const("}");
    decrease_indent(true);
    emit_const("});");
    output_code(name,"js");
}

bool dec_code_emit_code(void* ptr){
    return false;
}

bool ass_code_emit_code(void *ptr){
    return false;
}

bool call_code_emit_code(void *ptr){
    return false;
}

bool cond_code_emit_code(void *ptr){
    return false;
}

bool jmp_code_emit_code(void *ptr){
    return false;
}

bool label_code_emit_code(void *ptr){
    return false;
}

bool exp_code_emit_code(void *ptr){
    return false;
}

bool arg_code_emit_code(void *ptr){
    return false;
}

bool param_code_emit_code(void *ptr){
    return false;
}

bool func_code_emit_code(void *ptr){
    return false;
}

bool for_code_emit_code(void* ptr){
    return false;
}

bool while_code_emit_code(void* ptr){
    return false;
}

bool dowhile_code_emit_code(void* ptr){
    return false;
}

bool var_code_emit_code(void* ptr){
    return false;
}

bool inc_code_emit_code(void *ptr){
    return false;
}

bool struct_code_emit_code(void *ptr){
    return false;
}

bool ret_code_emit_code(void *ptr){
    return false;
}

bool def_code_emit_code(void *ptr){
    return false;
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
    return false;
}

bool switch_code_emit_code(void *ptr){
    return false;
}

bool case_code_emit_code(void *ptr){
    return false;
}

bool prop_init_code_emit_code(void *ptr){
    return false;
}

bool struct_init_code_emit_code(void *ptr){
    return false;
}

bool cast_code_emit_code(void *ptr){
    return false;
}

bool array_init_code_emit_code(void *ptr){
    return false;
}

bool array_entry_code_emit_code(void *ptr){
    return false;
}

bool blk_code_emit_code(void* ptr){
    blk_code *code = ptr;
    emit_const("seq(");
    codegen_emit_code(code->stat);
    emit_const(")");
    if (code->chain.ptr){
        emit_const(",");
        codegen_emit_code(code->chain);
    }
    return true;
}

bool rule_sequence_code_emit_code(void *ptr){
    rule_sequence_code *code = ptr;
    if (code->optional){
        emit_const("optional(");
    }
    if (code->value.length){
        emit_slice(code->value);
    } else {
        if (code->name.length && *code->name.data != '"') emit_const("$.");
        emit_slice(code->name);
    }
    if (code->optional){
        emit_const(")");
    }
    if (code->chain.ptr){
        emit_const(", ");  
        codegen_emit_code(code->chain);
    }
    return false;
}

int pref = 100;

bool rule_entry_code_emit_code(void *ptr){
    rule_entry_code *code = ptr;
    emit_slice(code->name);
    emit_const(": $ => ");
    emit("prec(%i,",pref--);
    emit_const("choice(");
    codegen_emit_code(code->list);
    emit_const(")),");
    emit_newline();
    codegen_emit_code(code->chain);
    return false;
}

#endif