#pragma once

#include "parser/ast.h"
#include "codegen/codegen.h"

codegen gen_code(chunk_array_t *stack, const char *out_name);