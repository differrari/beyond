#include "ast.h"

bool pop_stack(stack_navigator *sn, ast_node* node){
    if (sn->stack_cursor >= sn->stack_count) return false;
    *node = sn->ast_stack[sn->stack_cursor++];
    return true;
}

tern switch_rule(stack_navigator *sn, grammar_rules *current_rule, int *curr_option){
    ast_node node;
    if (!pop_stack(sn, &node)) return -1;
    if (node.t.kind){
        print("Found token instead of rule");
        return false;
    }
    *current_rule = node.rule;
    *curr_option = node.option;
    return true;
}