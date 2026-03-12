#include "ast.h"
#include "syscalls/syscalls.h"
#include "rules.h"
#include "common.h"

void push_node(chunk_array_t *stack, ast_node node){
    if (node.t.kind)
        print("%sToken %s to depth %i",indent_by(node.depth),token_name(node.t.kind),node.depth);
    else
        print("%sNode %s to depth %i",indent_by(node.depth),rule_names[node.rule],node.depth);
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

void debug_ast(chunk_array_t *stack){
    for (int i = 0; i < chunk_array_count(stack); i++){
        ast_node *node = (ast_node*)chunk_array_get(stack, i);
        if (node->terminator)
            print("%sTerminator to %s",indent_by(node->depth),rule_names[node->rule]);
        else if (node->t.kind)
            print("%sToken %s to depth %i",indent_by(node->depth),token_name(node->t.kind),node->depth);
        else
            print("%sNode %s to depth %i",indent_by(node->depth),rule_names[node->rule],node->depth);
    }
}

void tree_reset(chunk_array_t *tree, size_t to){
    if (tree->chunk_capacity <= to){
        if (tree->next)
            tree_reset(tree->next,to-tree->count);
    } else tree->count = to;
}