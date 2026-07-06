#include "lisp.h"
#include "syscalls/syscalls.h"
#include "ir/general.h"
#include "codegen/code_generator.h"

ast_node* next_node(stack_navigator *sn){
    ast_node *node = chunk_array_get(sn->stack, sn->cursor++);
    return node;
}

int make_ir_from_node(stack_navigator *sn, int depth, codegen code){
    ast_node *node = next_node(sn);
    while (node){
        if (node->t.kind && node->sem_value && node->depth == depth){
            codegen_register_elem(code, node->sem_value, node->t);
        }
        else if (depth >= 0 && node->depth <= depth){
            return node->depth;
        }
        else if (node->sem_value) {
            codegen new_atom = begin_rule(node->sem_value);
            int d = make_ir_from_node(sn,node->depth, new_atom);
            codegen_register_subrule(code, node->sem_value, new_atom);
            if (depth >= 0 && d <= depth)
                return d;
        }
        node = next_node(sn);
    }
    return 0;
}

codegen gen_ir_lisp(chunk_array_t *stack){
    codegen root = s_exp_code_init();
    root.type = sem_rule_sexp;
    stack_navigator nav = make_stack_navigator(stack);
    make_ir_from_node(&nav, -1, root);
    return root;
}