#pragma once

#include "ir/general.h"
#include "data/struct/hashmap.h"
#include "data/struct/stack.h"

typedef arr_stack_t *imaginal_env;

codegen apply(codegen fn_exp, codegen a, codegen *env);
codegen eval(codegen exp, codegen *env);
bool is_atom(codegen exp);

void imaginal_print(codegen exp);
