#pragma once

#include "syscalls/syscalls.h"
#include "string/string.h"
#include "semantic/sem_analysis.h"

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