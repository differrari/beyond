#include "ir/general.h"
#include "ir/arch_transformer.h"

#if defined(NOTRANSFORM) || (defined(NOCODETRANSFORM) && defined(NORULETRANSFORM))
codegen perform_transformations(codegen root){
    return root;
}

codegen blk_code_transform(codegen instance){
    return instance;
}

#endif

#if defined(NOTRANSFORM) || defined(NOCODETRANSFORM)

codegen dec_code_transform(codegen instance){
    return instance;
}

codegen ass_code_transform(codegen instance){
    return instance;
}

codegen exp_code_transform(codegen instance){
    return instance;
}

codegen call_code_transform(codegen instance){
    return instance;
}

codegen arg_code_transform(codegen instance){
    return instance;
}

codegen cond_code_transform(codegen instance){
    return instance;
}

codegen jmp_code_transform(codegen instance){
    return instance;
}

codegen label_code_transform(codegen instance){
    return instance;
}

codegen param_code_transform(codegen instance){
    return instance;
}

codegen func_code_transform(codegen instance){
    return instance;
}

codegen for_code_transform(codegen instance){
    return instance;
}

codegen while_code_transform(codegen instance){
    return instance;
}

codegen dowhile_code_transform(codegen instance){
    return instance;
}

codegen var_code_transform(codegen instance){
    return instance;
}

codegen inc_code_transform(codegen instance){
    return instance;
}

codegen struct_code_transform(codegen instance){
    return instance;
}

codegen ret_code_transform(codegen instance){
    return instance;
}

codegen def_code_transform(codegen instance){
    return instance;
}

codegen int_code_transform(codegen instance){
    return instance;
}

codegen enum_code_transform(codegen instance){
    return instance;
}

codegen enum_case_code_transform(codegen instance){
    return instance;
}

codegen else_code_transform(codegen instance){
    return instance;
}

codegen switch_code_transform(codegen instance){
    return instance;
}

codegen case_code_transform(codegen instance){
    return instance;
}

codegen prop_init_code_transform(codegen instance){
    return instance;
}

codegen struct_init_code_transform(codegen instance){
    return instance;
}

codegen cast_code_transform(codegen instance){
    return instance;
}

codegen array_init_code_transform(codegen instance){
    return instance;
}

codegen array_entry_code_transform(codegen instance){
    return instance;
}

#endif

#if defined(NOTRANSFORM) || defined(NORULETRANSFORM)

codegen rule_sequence_code_transform(codegen instance){
    return instance;
}

codegen rule_entry_code_transform(codegen instance){
    return instance;
}

#endif
