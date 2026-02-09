#pragma once

#include "parser/ast.h"
#include "codegen/codegen.h"

codegen gen_code(ast_node *stack, uint32_t count, const char *out_name);