#pragma once

#include "codegen/codegen.h"
#include "files/buffer.h"
#include "ir/arch_transformer.h"
#include "imaginal.h"

codegen parse_lisp(string_slice code);
extern codegen (*imaginal_fallback_fncall)(codegen fn_exp, codegen a, codegen *env);

codegen repl_run(string_slice code, bool p);