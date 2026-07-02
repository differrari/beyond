#pragma once

#include "data/struct/chunk_array.h"
#include "codegen/codegen.h"

codegen gen_code(chunk_array_t *stack);

void debug_print_ir(codegen root);