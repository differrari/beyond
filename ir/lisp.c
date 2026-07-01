#include "lisp.h"
#include "syscalls/syscalls.h"
#include "ir/general.h"

ast_node* next_node(stack_navigator *sn){
    ast_node *node = chunk_array_get(sn->stack, sn->stack_cursor++);
    return node;
}

extern void s_exp_code_register_elem(void *ptr, int type, Token elem);
extern void s_exp_code_register_subrule(void *ptr, int type, codegen child);

void gen_ir_lisp(stack_navigator *sn, int depth, s_exp_code *code){
    ast_node *node = next_node(sn);
    while (node){
        if (depth >= 0 && node->depth <= depth){
            print(")");
            return;
        }
        if (node->sem_value == sem_rule_sexp){
            print("(");
            codegen new_code = s_exp_code_init();
            gen_ir_lisp(sn,node->depth, new_code.ptr);
            s_exp_code_register_subrule(code, 0, new_code);
        } else if (node->sem_value) {
            print("[%s] %v",sem_rule_strings[node->sem_value],token_to_slice(node->t));
            s_exp_code_register_elem(code, 0, node->t);
        }
        node = next_node(sn);
    }
    if (depth >= 0) print(")");
}