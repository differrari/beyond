#pragma once

#include "parser/ast.h"
#include "ir/general.h"

codegen gen_ir_lisp(chunk_array_t *stack);