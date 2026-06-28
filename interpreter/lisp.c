#include "lisp.h"
#include "syscalls/syscalls.h"

ast_node* next_node(stack_navigator *sn){
    for (; sn->stack_cursor < sn->size; sn->stack_cursor++){
        ast_node *node = chunk_array_get(sn->stack, sn->stack_cursor);
        sn->stack_cursor++;
        return node;
    }
    return 0;
}

void lisp_vm(stack_navigator *sn, int depth){
    ast_node *node = next_node(sn);
    while (node){
        if (depth >= 0 && node->depth <= depth){
            print(")");
            return;
        }
        if (node->sem_value == sem_rule_exp){
            print("(");
            lisp_vm(sn,node->depth);
        } else if (node->sem_value) print("%v",token_to_slice(node->t));
        node = next_node(sn);
    }
    if (depth >= 0) print(")");
}