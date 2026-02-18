#include "ir/general.h"
#include "ir/arch_transformer.h"

#if defined(NOTRANSFORM) || (defined(NOCODETRANSFORM) && defined(NORULETRANSFORM))
codegen perform_transformations(codegen root){
    return root;
}

codegen blk_code_transform(void *ptr, codegen this){
    return this;
}

#endif

#if defined(NOTRANSFORM) || defined(NOCODETRANSFORM)


codegen dec_code_transform(void *ptr, codegen this){
    return this;
}

codegen ass_code_transform(void *ptr, codegen this){
    return this;
}

codegen exp_code_transform(void *ptr, codegen this){
    return this;
}

codegen call_code_transform(void *ptr, codegen this){
    return this;
}

codegen arg_code_transform(void *ptr, codegen this){
    return this;
}

codegen cond_code_transform(void *ptr, codegen this){
    return this;
}

codegen jmp_code_transform(void *ptr, codegen this){
    return this;
}

codegen label_code_transform(void *ptr, codegen this){
    return this;
}

codegen param_code_transform(void *ptr, codegen this){
    return this;
}

codegen func_code_transform(void *ptr, codegen this){
    return this;
}

codegen for_code_transform(void *ptr, codegen this){
    return this;
}

codegen while_code_transform(void *ptr, codegen this){
    return this;
}

codegen dowhile_code_transform(void *ptr, codegen this){
    return this;
}

codegen var_code_transform(void *ptr, codegen this){
    return this;
}

codegen inc_code_transform(void *ptr, codegen this){
    return this;
}

codegen struct_code_transform(void *ptr, codegen this){
    return this;
}

codegen ret_code_transform(void *ptr, codegen this){
    return this;
}

codegen def_code_transform(void *ptr, codegen this){
    return this;
}

codegen int_code_transform(void *ptr, codegen this){
    return this;
}

codegen enum_code_transform(void *ptr, codegen this){
    return this;
}

codegen enum_case_code_transform(void *ptr, codegen this){
    return this;
}

codegen else_code_transform(void *ptr, codegen this){
    return this;
}

codegen switch_code_transform(void *ptr, codegen this){
    return this;
}

codegen case_code_transform(void *ptr, codegen this){
    return this;
}

codegen prop_init_code_transform(void *ptr, codegen this){
    return this;
}

codegen struct_init_code_transform(void *ptr, codegen this){
    return this;
}

codegen cast_code_transform(void *ptr, codegen this){
    return this;
}

codegen array_init_code_transform(void *ptr, codegen this){
    return this;
}

codegen array_entry_code_transform(void *ptr, codegen this){
    return this;
}

#endif

#if defined(NOTRANSFORM) || defined(NORULETRANSFORM)

codegen rule_sequence_code_transform(void *ptr, codegen this){
    return this;
}

codegen rule_entry_code_transform(void *ptr, codegen this){
    return this;
}

#endif