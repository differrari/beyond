#pragma once

#include "string/string.h"
#include "semantic/sem_analysis.h"
#include "syscalls/syscalls.h"

#define FIND_SYM(rule, symname) symbol_t *sym = find_symbol(rule, token_to_slice(symname));\
if (!sym && rule == sem_rule_dec) sym = find_symbol(sem_rule_param, token_to_slice(symname));\
if (!sym) { print("Symbol not found: %v",token_to_slice(symname)); sym = zalloc(sizeof(symbol_t)); sym->name = token_to_slice(symname); }

#define FIND_SLICE(rule, name_slice) symbol_t *sym = find_symbol(rule, name_slice);\
if (!sym) { print("Symbol not found: %v",name_slice); sym = zalloc(sizeof(symbol_t)); sym->name = name_slice; }

static inline string type_name(symbol_t *sym, bool sub, bool extra){
    string base = {};
    switch ((sub ? sym->resolved_subtype : sym->resolved_type)) {
    case semantic_types_none: base = string_from_literal("void"); break;
    case semantic_types_literal: base = string_from_literal("char*"); break;
    case semantic_types_int64: base = string_from_literal("int64_t"); break;
    case semantic_types_int32: base = string_from_literal("int32_t"); break;
    default:
    case semantic_types_passthrough: base = string_from_slice(token_to_slice(sub ? sym->subtype : sym->type)); break;
    }
    string chain = {};
    if (extra){
        if ((sub ? sym->resolved_subtype : sym->reference_type)){
            chain = string_from_literal("_t*");
        } 
    }
    if (!chain.data)
        return base;
    string s = string_concat(base, chain);
    string_free(base);
    string_free(chain);
    return s;
}