#include "sem_analysis.h"
#include "common.h"

stack_navigator ssn;

symbol_table* new_table(){
    symbol_table *table = (symbol_table*)zalloc(sizeof(symbol_table) + (sizeof(symbol_t) * 256));//TODO: won't be enough
    table->symbol_table = (symbol_t*)((uintptr_t)table + sizeof(symbol_table));
    return table;
}

bool symbol_exists(sem_rule type, Token t){
    return true;
    // for (uint16_t i = 0; i < symbol_count; i++){
    //     if (slices_equal(token_to_slice(t), token_to_slice(symbol_table[i].name), false)) return true;
    // }
    // return false;
}

symbol_table *global;

symbol_t * find_symbol_in_table(symbol_table *table,sem_rule type, string_slice name){
    for (int i = 0; i < table->symbol_count; i++){
        symbol_t *sym = &table->symbol_table[i];
        if (sym->sym_type == type && slices_equal(name, sym->name, false)){
            return sym;
        }
        if (sym && sym->name.length){//TODO: hack
            if (sym->child){
                symbol_t *s = find_symbol_in_table(sym->child, type, name);
                if (s) return s;
            }
        }
    }
    return 0;
}

symbol_t * find_symbol(sem_rule type, string_slice name){
    return find_symbol_in_table(global, type, name);
}

semantic_types analyze_type(Token t){
    //TODO: allow making this dynamic
    if (slice_lit_match(token_to_slice(t), "literal", false)){
        return semantic_types_literal;
    }
    return semantic_types_passthrough;
}

bool analyze_rule(int current_rule, int curr_option, symbol_table *table){
    symbol_t *sym = &table->symbol_table[table->symbol_count++];
    
    if (language_rules[current_rule].action == sem_action_declare){
        sym->sym_type = language_rules[current_rule].semrule;
    }
    
    symbol_table *current_table = table;
    
    if (language_rules[current_rule].semrule == sem_rule_func ||
        language_rules[current_rule].semrule == sem_rule_struct || 
        language_rules[current_rule].semrule == sem_rule_interf){
            current_table = new_table();
            sym->child = current_table;
        }
    
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
            if (!analyze_rule(new_rule, new_opt, current_table)) return false;
        } else if (elem.sem_value && !elem.lit){
             ast_node node;
             pop_stack(&ssn, &node);
             if (node.action == sem_action_declare) {
                 if (sym && node.sem_value == sem_elem_type){
                     sym->type = node.t;
                     sym->resolved_type = analyze_type(node.t);
                 } 
                 if (sym && node.sem_value == sem_elem_name) sym->name = token_to_slice(node.t);
                 // print("SYMBOL: %s = %v",sem_rule_to_string(elem.sem_value),token_to_slice(node.t));
             } else if (node.action == sem_action_check && (node.sem_value == sem_rule_var || node.sem_value == sem_rule_jmp /*&& elem.sem_value != sem_func*/)){
                 if (!symbol_exists(node.sem_value, node.t)){
                     print("%s %v does not exist",sem_rule_to_string(node.sem_value),token_to_slice(node.t));
                     return true;
                 }
             }
        }
    }
    return true;
}

int ind = 0;

void debug_table(symbol_table *table){
    for (int i = 0; i < table->symbol_count; i++){
        symbol_t *sym = &table->symbol_table[i];
        if (!sym){
            print("INVALID SYMBOL %i",i);
            return;
        }
        if (sym->name.length){//TODO: hack
            print("%sSYMBOL = %s TYPE = %v NAME = %v",indent_by(ind),sem_rule_to_string(sym->sym_type),sym->type.kind ? token_to_slice(sym->type) : make_string_slice("none", 0, 4),sym->name);
            if (sym->child){
                ind++;
                print("Has %i children",sym->child->symbol_count);
                debug_table(sym->child);
                ind--;
            }
        }
    }
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
    global = new_table();
    bool result = analyze_rule(new_rule,new_opt,global);
    if (!result) return false;
    // debug_table(global);
    print("Semantic analysis done");
    return true;
}