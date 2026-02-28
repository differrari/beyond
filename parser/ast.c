#include "ast.h"
#include "syscalls/syscalls.h"

void push_node(chunk_array_t *stack, ast_node node){
    chunk_array_push(stack, &node);
}

bool pop_stack(stack_navigator *sn, ast_node* node){
    if (sn->stack_cursor >= chunk_array_count(sn->stack)){
        return false;
    }
    *node = *(ast_node*)chunk_array_get(sn->stack, sn->stack_cursor++);
    return true;
}

tern switch_rule(stack_navigator *sn, int *current_rule, int *curr_option){
    ast_node node;
    if (!pop_stack(sn, &node)) return -1;
    if (node.t.kind){
        print("Found token %s instead of rule %v",token_name(node.t.kind),token_to_slice(node.t));
        return false;
    }
    *current_rule = node.rule;
    *curr_option = node.option;
    return true;
}

chunk_array_t* init_ast(){
    return chunk_array_create(sizeof(ast_node), 1024);
}

size_t tree_count(chunk_array_t *stack){
    return chunk_array_count(stack);
}

void tree_reset(chunk_array_t *tree, size_t to){
    if (tree->chunk_capacity <= to){
        if (tree->next)
            tree_reset(tree->next,to-tree->count);
    } else tree->count = to;
}