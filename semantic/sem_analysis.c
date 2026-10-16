#include "ir/general.h"
#include "sem_analysis.h"
#define MOG_MODULE "SEM_ANAL"
#define MOG_LEVEL MOG_VERBOSE
#include "utils/mog.h"

symbol_table *global;

//TODO: rethink this whole thing
int analyze_type(string_slice v){
    if (slice_lit_match(v, "void", false)){
        return semantic_types_none;
    }
    if (slice_lit_match(v, "literal", false)){
        return semantic_types_literal;
    }
    return semantic_types_passthrough;
}

int analyze_subtype(string_slice v, symbol_t *sym){
    if (slice_lit_match(v, "void", false)){
        return semantic_types_none;
    }
    if (slice_lit_match(v, "literal", false)){
        return semantic_types_literal;
    }
    return semantic_types_passthrough;
}

symbol_t * find_symbol_in_table(symbol_table *table,sem_rule type, string_slice name){
    for (int i = 0; i < table->symbol_count; i++){
        symbol_t *sym = &table->symbol_table[i];
        if (sym->sym_type == type && slices_equal(name, sym->name, false) || slices_equal(name, sym->type, false)){
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

symbol_table* new_table(sem_rule type){
    symbol_table *table = (symbol_table*)zalloc(sizeof(symbol_table) + (sizeof(symbol_t) * 256));//TODO: won't be enough
    table->table_type = type;
    table->symbol_table = (symbol_t*)((uintptr_t)table + sizeof(symbol_table));
    return table;
}

bool insert_symbol(symbol_table *table, symbol_t sym){
    if (table->symbol_count >= 256) return false;
    table->symbol_table[table->symbol_count++] = sym;
    return true;
}

bool symbol_exists(symbol_table *table, string_slice name){
    return true;
    // for (uint16_t i = 0; i < symbol_count; i++){
    //     if (slices_equal(token_to_slice(t), token_to_slice(symbol_table[i].name), false)) return true;
    // }
    // return false;
}

bool blk_code_analyze_semantics(codegen instance, symbol_table *table){
    blk_code *code = instance.ptr;
    return codegen_analyze_semantics(code->stat, table) &&
           codegen_analyze_semantics(code->chain, table);
}

bool dec_code_analyze_semantics(codegen instance, symbol_table *table){
    dec_code *code = instance.ptr;
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_dec,
        .type = code->type,
        .resolved_type = analyze_type(code->type),
        .name = code->name,
        .reference_type = code->is_ref
    })) return false;
    return codegen_analyze_semantics(code->initial_value, table);
}

//TODO: remove the lowercase macro and add the uppercase one everywhere + restore its return false
#define analyze(name, content) bool name##_analyze_semantics(codegen instance, symbol_table *table){ name *code = instance.ptr; content; return true; }
#define ANALYZE(s,t) codegen_analyze_semantics(s,t)
// if (s.ptr && codegen_analyze_semantics(s,t)) return false;

analyze(ass_code,{
    ANALYZE(code->var, table);
    ANALYZE(code->expression, table);
    return true;
})

analyze(call_code,{
    return symbol_exists(table,code->name) && codegen_analyze_semantics(code->args, table);
})

analyze(cond_code,{
    if (!codegen_analyze_semantics(code->cond, table)) return false;
    symbol_table *subtable = new_table(sem_rule_scope);
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_scope,
        .child = subtable
    })) return false;
    codegen_analyze_semantics(code->scope, subtable);
    return codegen_analyze_semantics(code->chain, table);
})

analyze(jmp_code, {
    return symbol_exists(table, code->jump);
})

analyze(label_code, {
    return insert_symbol(table, (symbol_t){
        .name = code->name,
        .sym_type = sem_rule_label
    });
})

analyze(exp_code, {
    return 
    codegen_analyze_semantics(code->exp, table) &&
    codegen_analyze_semantics(code->var, table) &&
    codegen_analyze_semantics(code->lambda, table);
})

analyze(arg_code,{
    if (!codegen_analyze_semantics(code->exp, table)) return false;
    return codegen_analyze_semantics(code->chain, table);
});

analyze(param_code, {
    print("Inserting symbol %v",code->name);
    if (!insert_symbol(table, (symbol_t){ 
        .sym_type = sem_rule_param,
        .name = code->name,
        .type = code->type,
        .resolved_type = analyze_type(code->type)
    })) return false;
    return codegen_analyze_semantics(code->chain,table);
})

analyze(func_code, {
    if (code->signature.ptr){
        return codegen_analyze_semantics(code->signature,table);
    } else {
        symbol_table *subtable = new_table(sem_rule_func);
        if (!code->name.length) code->name = make_temp_name(sem_rule_func);
        ANALYZE(code->args,subtable);
        ANALYZE(code->body,subtable);
        if (!insert_symbol(table, (symbol_t){
            .sym_type = sem_rule_func,
            .name = code->name,
            .type = code->type,
            .resolved_type = analyze_type(code->type),
            .child = subtable
        })) return false;
    }
})

analyze(for_code,{
    symbol_table *subtable = new_table(sem_rule_scope);
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_scope,
        .child = subtable
    })) return false;
    if (!codegen_analyze_semantics(code->initial,subtable) &&
    codegen_analyze_semantics(code->condition,subtable) &&
    codegen_analyze_semantics(code->increment,subtable)) return false;
    codegen_analyze_semantics(code->body,subtable);
})

analyze(while_code,{
    if (!codegen_analyze_semantics(code->condition, table)) return false;
    symbol_table *subtable = new_table(sem_rule_scope);
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_scope,
        .child = subtable
    })) return false;
    return codegen_analyze_semantics(code->body,subtable);
})

analyze(dowhile_code,{
    if (!codegen_analyze_semantics(code->condition, table)) return false;
    symbol_table *subtable = new_table(sem_rule_scope);
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_scope,
        .child = subtable
    })) return false;
    return codegen_analyze_semantics(code->body,subtable);
})

analyze(var_code,{
    return codegen_analyze_semantics(code->expression,table) && 
    codegen_analyze_semantics(code->var,table);
})

analyze(inc_code,{
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_inc,
        .name = code->value
    })){
        mog(ERROR, "Failed registering include %v",code->value);
        return false;
    }
    mog(VERBOSE, "Registered include %v",code->value);
    return true;
})

analyze(struct_code,{
    //TODO: analyze code->parent and conformance to parent
    symbol_table *subtable = new_table(sem_rule_struct);
    if (!insert_symbol(table, (symbol_t){
        .type = code->name,
        .resolved_type = semantic_types_passthrough,
        .sym_type = sem_rule_struct,
        .child = subtable
    })){
        mog(ERROR,"Failed to insert struct definition %v",code->name);
        return false;
    }
    return codegen_analyze_semantics(code->contents, subtable);
})

analyze(ret_code,{
    return codegen_analyze_semantics(code->expression, table);
})

analyze(def_code,{
    return codegen_analyze_semantics(code->expression, table);
})

analyze(int_code,{
    symbol_table *subtable = new_table(sem_rule_interf);
    if (!insert_symbol(table, (symbol_t){
        .type = code->name,
        .sym_type = sem_rule_interf,
        .child = subtable
    })) return false;
    return codegen_analyze_semantics(code->contents, subtable);
})

analyze(enum_code,{
    symbol_table *subtable = new_table(sem_rule_enum);
    if (!insert_symbol(table, (symbol_t){
        .type = code->name,
        .sym_type = sem_rule_enum,
        .child = subtable
    })) return false;
    return codegen_analyze_semantics(code->contents, subtable);
})

analyze(enum_case_code,{
    if (!insert_symbol(table, (symbol_t){
        .name = code->name,
        .sym_type = sem_rule_enum_case,
    })) return false;
    return codegen_analyze_semantics(code->chain, table);
})

analyze(else_code,{
    return codegen_analyze_semantics(code->block, table);
})

analyze(switch_code,{
    return codegen_analyze_semantics(code->condition, table) && codegen_analyze_semantics(code->cases, table);
})

analyze(case_code,{
    codegen_analyze_semantics(code->match, table);
    symbol_table *subtable = new_table(sem_rule_scope);
    if (!insert_symbol(table, (symbol_t){
        .sym_type = sem_rule_scope,
        .child = subtable
    })) return false;
    if (!codegen_analyze_semantics(code->body, subtable)) return false;
    return codegen_analyze_semantics(code->chain, table);
})

analyze(prop_init_code,{
    // code->name; // TODO: Check against the type to see if it exists
    return codegen_analyze_semantics(code->expression, table) && codegen_analyze_semantics(code->chain, table);
})

analyze(struct_init_code,{
    // code->name // TODO: check this against types to find it
    return codegen_analyze_semantics(code->content, table);
})

analyze(cast_code,{
    //TODO: type checking
    return true;
})

analyze(array_init_code,{
    return codegen_analyze_semantics(code->entries, table);
})

analyze(array_entry_code,{
    // code->index // TODO: Can this be checked for correctness?
    return codegen_analyze_semantics(code->exp, table) && codegen_analyze_semantics(code->chain, table);
})

analyze(rule_sequence_code,{
    return true;
})

analyze(rule_entry_code,{
    return true;
})

analyze(atom_code,{
    return true;
})

analyze(s_exp_code,{
    return true;
})

symbol_table* analyze_semantics(codegen ir){
    symbol_table *root_table = new_table(sem_rule_scope);
    global = root_table;
    if (!codegen_analyze_semantics(ir,root_table)){
        mog(ERROR, "Semantic analysis failed");
        return 0;
    }
    mog(VERBOSE,"Successfully analyzed");
    return root_table;
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