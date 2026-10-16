#include "lisp.h"
#include "syscalls/syscalls.h"
#include "ir/general.h"
#include "codegen/code_generator.h"
#define MOG_LEVEL MOG_VERBOSE
#define MOG_MODULE "IRGEN"
#include "utils/mog.h"

ast_node* next_node(stack_navigator *sn){
    ast_node *node = chunk_array_get(sn->stack, sn->cursor++);
    return node;
}

codegen make_ir_from_node(stack_navigator *sn, int depth, codegen code){
    codegen ret = code;
    ast_node *node = next_node(sn);
    while (node){
        if (node->terminate) {
            if (code.type == node->sem_value){
                mog(VERBOSE,"Terminating %s",sem_rule_strings[code.type]);
                return ret;
            }
            if (node->sem_value){
                mog(ERROR,"Wrong terminator for %s expected terminator for %s",sem_rule_strings[node->sem_value],sem_rule_strings[code.type]);
                return (codegen){};
            }
        }
        if (node->t.kind && node->sem_value && node->depth == depth){
            mog(VERBOSE,"Found token %v for rule %s",token_to_slice(node->t),sem_rule_strings[code.type]);
            codegen_register_elem(code, node->sem_value, node->t);
        }
        else if (node->sem_value) {
            mog(VERBOSE,"Found subrule %s for rule %s",sem_rule_strings[node->sem_value],sem_rule_strings[code.type]);
            codegen new_subrule = begin_rule(node->sem_value);
            make_ir_from_node(sn,node->depth, new_subrule);
            if (ret.type)
                codegen_register_subrule(code, node->sem_value, new_subrule);
            else ret = new_subrule;
        }
        node = next_node(sn);
    }
    return ret;
}

codegen gen_ir_lisp(chunk_array_t *stack){
    codegen root = s_exp_code_init();
    root.type = sem_rule_sexp;
    stack_navigator nav = make_stack_navigator(stack);
    make_ir_from_node(&nav, -1, root);
    return root;
}