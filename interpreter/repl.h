#pragma once

#include "codegen/codegen.h"
#include "files/buffer.h"
#include "ir/arch_transformer.h"
#include "imaginal.h"
#include "shell/shell.h"

typedef struct {
    codegen environment;
    codegen output;
    bool should_print;
    imaginal_fallback fallback;
} imaginal_repl_ctx;

codegen parse_lisp(string_slice code);

bool repl_run(string_slice code, void *ctx);

void shell_embed_repl(shell_handle *handle);