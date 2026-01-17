#include "sem_analysis.h"

stack_navigator ssn;

typedef struct {
    Token type;
    Token name;
    semantic_rules sym_type;
} symbol_t;

symbol_t symbol_table[64];
uint16_t symbol_count; 

bool find_symbol(semantic_rules type, Token t){
    for (uint16_t i = 0; i < symbol_count; i++){
        if (slices_equal(token_to_slice(t), token_to_slice(symbol_table[i].name), false)) return true;
    }
    return false;
}

bool analyze_rule(int current_rule, int curr_option){
    // print("Current rule %s@%i",rule_names[current_rule],curr_option);
    // if (semantic_analysis_rules[current_rule]){
        // if (semantic_analysis_rules[current_rule] == sem_scope)
            // print("== SYMBOL TABLE FOR SCOPE ==");
    // }
    
    symbol_t sym = {};
    
    sym.sym_type = language_rules[current_rule].action;
    
    for (int s = 0; s < language_rules[current_rule].options[curr_option].num_elements; s++){
        grammar_elem elem = language_rules[current_rule].options[curr_option].rules[s];
        if (elem.rule){
            int new_rule;
            int new_opt;
            tern res = switch_rule(&ssn, &new_rule, &new_opt);
            if (res != true){
                if (!res) print("Rule not found %i", elem.value);
                return res;
            }
            if (!analyze_rule(new_rule, new_opt)) return false;
        } else if (elem.sem_value && !elem.lit){
             ast_node node;
             pop_stack(&ssn, &node);
             if (node.action == sem_action_declare) {
                 if (node.sem_value == sem_elem_type) sym.type = node.t;
                 if (node.sem_value == sem_elem_name) sym.name = node.t;
                 // print("SYMBOL: %s = %v",get_sem_elem_name(elem.sem_value),token_to_slice(node.t));
             } else if (node.action == sem_action_check && (node.sem_value == sem_var || node.sem_value == sem_jmp /*&& elem.sem_value != sem_func*/)){
                 if (!find_symbol(node.sem_value, node.t)){
                     print("%s %v does not exist",sem_rule_name(node.sem_value),token_to_slice(node.t));
                     return true;
                 }
             }
        }
    }
    if (sym.sym_type == sem_func || sym.sym_type == sem_param || sym.sym_type == sem_label || sym.sym_type == sem_dec || sym.sym_type == sem_func){
        symbol_table[symbol_count++] = sym;
        print("SYMBOL = %s TYPE = %v NAME = %v",sem_rule_name(sym.sym_type),sym.type.kind ? token_to_slice(sym.type) : make_string_slice("none", 0, 4),token_to_slice(sym.name));
    }
    //     print("== END SYMBOL TABLE SCOPE ==");
    // print("Finished rule %s@%i",rule_name(current_rule),curr_option);
    return true;
}

bool analyze_semantics(ast_node *stack, uint32_t count){
    ssn = (stack_navigator){};
    ssn.ast_stack = stack;
    ssn.stack_count = count;
    // output_debug_rule(current_rule);
    ast_node node = {};
    int new_rule;
    int new_opt;
    switch_rule(&ssn, &new_rule,&new_opt);
    return analyze_rule(new_rule,new_opt);
}