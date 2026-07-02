#include "lisp.h"
#include "syscalls/syscalls.h"
#include "ir/general.h"

ast_node* next_node(stack_navigator *sn){
    ast_node *node = chunk_array_get(sn->stack, sn->cursor++);
    return node;
}

extern void s_exp_code_register_elem(void *ptr, int type, Token elem);
extern void s_exp_code_register_subrule(void *ptr, int type, codegen child);

void make_ir_from_node(stack_navigator *sn, int depth, s_exp_code *code){
    ast_node *node = next_node(sn);
    while (node){
        if (depth >= 0 && node->depth <= depth){
            // print(")");
            return;
        }
        if (node->sem_value == sem_rule_sexp){
            // print("(");
            codegen new_code = s_exp_code_init();
            make_ir_from_node(sn,node->depth, new_code.ptr);
            s_exp_code_register_subrule(code, 0, new_code);
        } else if (node->sem_value) {
            // print("[%s] %v",sem_rule_strings[node->sem_value],token_to_slice(node->t));
            s_exp_code_register_elem(code, 0, node->t);
        }
        node = next_node(sn);
    }
    // if (depth >= 0) print(")");
}

codegen gen_ir_lisp(chunk_array_t *stack){
    codegen root = s_exp_code_init();
    root.type = sem_rule_sexp;
    stack_navigator nav = make_stack_navigator(stack);
    make_ir_from_node(&nav, -1, root.ptr);
    return root;
}