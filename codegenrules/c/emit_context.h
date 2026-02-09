#pragma once

#include "string/slice.h"
#include "codegen/codegen.h"

typedef enum { convenience_type_none, convenience_type_to_string, convenience_type_from_string, convenience_type_to_int, convenience_type_from_int} convenience_type;

typedef struct {
    int context_rule;
    string_slice context_prefix;
    string_slice context_parent;
    bool ignore_semicolon;
    codegen defer_statements;
    convenience_type convenience;
} emit_context;

static emit_context ctx;

static inline emit_context save_and_push_context(emit_context nctx){
    emit_context orig = ctx;
    ctx = nctx;
    return orig;
}

static inline emit_context pop_and_restore_context(emit_context restore){
    emit_context orig = ctx;
    ctx = restore;
    return orig;
}