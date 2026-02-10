#pragma once

#include "codegen/codegen.h"

codegen wrap_in_block(codegen statement, codegen chain, bool tail);
codegen make_const_exp(string_slice exp);
codegen make_label(literal name);
codegen make_declaration(literal name, string_slice type, codegen initial);
codegen make_return(codegen expression);
codegen make_argument(codegen expression, codegen chain);
codegen make_literal_expression(string_slice slice);
codegen lambda_to_func(codegen lambda, string_slice name);
codegen var_to_exp(codegen var);