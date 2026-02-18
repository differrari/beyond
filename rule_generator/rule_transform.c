#ifdef RULETRANSFORM

#include "ir/general.h"
#include "ir/arch_transformer.h"
#include "ir/manual_gen.h"
#include "syscalls/syscalls.h"

codegen additional_rules = {};
codegen literals = {};

codegen perform_transformations(codegen root){
    root = codegen_transform(root,root);
    rule_entry_code *code = root.ptr;
    while (code->chain.ptr) code = code->chain.ptr;
    code->chain = additional_rules;
    return root;
}

bool is_token(string_slice slice){
    for (int i = 0; i < slice.length; i++)
        if (slice.data[i] <= 'A' || slice.data[i] >= 'Z') return false;
    return true;
}


codegen blk_code_transform(void *ptr, codegen this){
    blk_code *code = ptr;
    TRANSFORM(stat);
    TRANSFORM(chain);
    return this;
}

codegen get_inserted_rule(string_slice name){
    codegen rule = additional_rules;
    do {
        rule_entry_code *code = rule.ptr;
        if (!code) return (codegen){};
        if (slices_equal(code->name, name, true)){
            return rule;
        }
        rule = code->chain;
    } while(rule.ptr);
    return (codegen){};
}

codegen get_inserted_option(codegen options, string_slice name){
    codegen rule = options;
    do {
        blk_code *code = rule.ptr;
        if (!code) return (codegen){};
        if (code->stat.ptr){
            rule_sequence_code *seq = code->stat.ptr;
            if (slices_equal(seq->name, name, true)){
                return rule;
            }
        }
        rule = code->chain;
    } while(rule.ptr);
    return (codegen){};
}

void insert_new_rule(string_slice name, string_slice tag, bool declaration, string_slice opt_value){
    codegen rule = get_inserted_rule(name);
    codegen seq = make_rule_sequence(opt_value, (string_slice){}, (string_slice){}, (string_slice){}, false, (codegen){});
    if (!rule.ptr) additional_rules = make_rule(name, tag, declaration, wrap_in_block(seq,(codegen){},false), additional_rules);
    else {
        rule_entry_code *code = rule.ptr;
        codegen list = get_inserted_option(code->list, opt_value);
        if (!list.ptr) code->list = wrap_in_block(seq, code->list, true);
    }
}

codegen rule_sequence_code_transform(void *ptr, codegen this){
    rule_sequence_code *code = this.ptr;
    if (code->tag.length && *code->tag.data != '"' && is_token(code->name)){
        insert_new_rule(code->tag, (string_slice){}, false, code->name);
        code->name = code->tag;
    }
    TRANSFORM(chain);
    return this;
}

codegen rule_entry_code_transform(void *ptr, codegen this){
    rule_entry_code *code = ptr;
    TRANSFORM(list);
    TRANSFORM(chain);
    return this;
}

#endif