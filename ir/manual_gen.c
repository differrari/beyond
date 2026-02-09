#include "manual_gen.h"
#include "general.h"

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

codegen make_const_exp(string_slice exp){
    codegen exp_codegen = exp_code_init();
    exp_code *code = exp_codegen.ptr;
    code->val = exp;
    return exp_codegen;
}

codegen make_label(literal name){
    codegen def_label = label_code_init();
    label_code *label = (label_code*)def_label.ptr;
    label->name = slice_from_literal(name);
    return def_label;
}

codegen make_declaration(literal name, string_slice type, codegen initial){
    codegen def_ret_value = dec_code_init();
    dec_code *declaration = (dec_code*)def_ret_value.ptr;
    declaration->name = slice_from_literal("__return_val");
    declaration->type = type;
    declaration->initial_value = initial;
    return def_ret_value;
}

codegen make_return(codegen expression){
    codegen ret_value = ret_code_init();
    ret_code *retstat = (ret_code*)ret_value.ptr;
    retstat->expression = expression;
    return ret_value;
}