#pragma once

#include "ir/general.h"
#include "data/struct/hashmap.h"

typedef hash_map_t* imaginal_env;

codegen apply(codegen fn_exp, codegen a, imaginal_env env);
codegen eval(codegen exp, imaginal_env);
bool is_atom(codegen exp);

void imaginal_run(codegen c);

void imaginal_print_exp(codegen c);