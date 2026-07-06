#include "lisp.h"
#include "syscalls/syscalls.h"
#include "ir/general.h"
#include "codegen/code_generator.h"

ast_node* next_node(stack_navigator *sn){
    ast_node *node = chunk_array_get(sn->stack, sn->cursor++);
    return node;
}

void make_ir_from_node(stack_navigator *sn, int depth, codegen code){
    ast_node *node = next_node(sn);
    while (node){
        if (node->t.kind && node->sem_value){
            codegen_register_elem(code, node->sem_value, node->t);
        }
        if (depth >= 0 && node->depth <= depth){
            // print(")");
            return;
        }
        if (node->sem_value) {
            // print("[%s] %v",sem_rule_strings[tok_node->sem_value],token_to_slice(tok_node->t));
            codegen new_atom = begin_rule(node->sem_value);
            make_ir_from_node(sn,node->depth, new_atom);
            codegen_register_subrule(code, node->sem_value, new_atom);
        }
        node = next_node(sn);
    }
    // if (depth >= 0) print(")");
}

codegen gen_ir_lisp(chunk_array_t *stack){
    codegen root = s_exp_code_init();
    root.type = sem_rule_sexp;
    stack_navigator nav = make_stack_navigator(stack);
    make_ir_from_node(&nav, -1, root);
    return root;
}