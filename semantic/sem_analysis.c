#include "sem_analysis.h"
#include "common.h"
#include "alloc/allocate.h"

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
            sym->table_type = table->table_type;
            return sym;
        }
        if (sym){//TODO: hack
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

void analyze_type(Token t, symbol_t *sym){
    //TODO: allow making this dynamic
    if (slice_lit_match(token_to_slice(t), "void", false)){
        sym->resolved_type = semantic_types_none;
        return;
    }
    if (slice_lit_match(token_to_slice(t), "literal", false)){
        sym->resolved_type = semantic_types_literal;
        return;
    }
    sym->resolved_type = semantic_types_passthrough;
}

void analyze_subtype(Token t, symbol_t *sym){
    if (slice_lit_match(token_to_slice(t), "void", false)){
        sym->resolved_subtype = semantic_types_none;
        return;
    }
    if (slice_lit_match(token_to_slice(t), "literal", false)){
        sym->resolved_subtype = semantic_types_literal;
        return;
    }
    sym->resolved_subtype = semantic_types_passthrough;
}

bool analyze_rule(int current_rule, int curr_option, symbol_table *table){
    symbol_t *sym = &table->symbol_table[table->symbol_count++];
    
    if (language_rules[current_rule].action == sem_action_declare){
        sym->sym_type = language_rules[current_rule].semrule; 
    }
    
    symbol_table *current_table = table;
    
    if (language_rules[current_rule].semrule == sem_rule_func ||
        language_rules[current_rule].semrule == sem_rule_struct || 
        language_rules[current_rule].semrule == sem_rule_interf ||
        language_rules[current_rule].semrule == sem_rule_enum){
            current_table = new_table();
            current_table->table_type = language_rules[current_rule].semrule;
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
        } else if (elem.sem_value){
             ast_node node;
             pop_stack(&ssn, &node);
             if (node.action == sem_action_declare) {
                 if (sym && node.sem_value == sem_elem_type){
                     sym->type = node.t;
                     analyze_type(node.t,sym);
                 } 
                 if (sym && node.sem_value == sem_elem_subtype){
                     sym->subtype = node.t;
                     analyze_subtype(node.t,sym);
                 } 
                 if (sym && node.sem_value == sem_elem_ref){
                     sym->reference_type = true;
                 }
                 if (sym && node.sem_value == sem_elem_name) sym->name = token_to_slice(node.t);
             } else if (node.action == sem_action_check){
                 if (!symbol_exists(node.sem_value, node.t)){
                     print("%s %v does not exist",sem_rule_strings[node.sem_value],token_to_slice(node.t));
                     return true;
                 }
             }
        }
    }
    return true;
}

int ind = 0;

symbol_table* analyze_semantics(chunk_array_t *stack){
    ssn = (stack_navigator){};
    ssn.stack = stack;
    // output_debug_rule(current_rule);
    ast_node node = {};
    int new_rule;
    int new_opt;
    switch_rule(&ssn, &new_rule,&new_opt);
    global = new_table();
    bool result = analyze_rule(new_rule,new_opt,global);
    if (!result) return false;
    print("Semantic analysis done");
    return global;
}

uint64_t genid = 0;

string_slice make_temp_name(sem_rule type){
    switch (type) {
        case sem_rule_func: {
            string s = string_format("_temp_func_%i",genid++);
            return make_string_slice(s.data, 0, s.length);
        }
        default: return (string_slice){};
    }
}