#pragma once

#include "codegen/codegen.h"

codegen wrap_in_block(codegen statement, codegen chain, bool tail);
codegen make_const_exp(string_slice exp);
codegen make_label(literal name);
codegen make_declaration(literal name, string_slice type, codegen initial);
codegen make_return(codegen expression);
codegen make_argument(codegen expression, codegen chain);
codegen make_param(string_slice type, string_slice name, codegen chain);
codegen make_literal_expression(string_slice slice);
codegen make_literal_var(string_slice slice);
codegen make_if(codegen condition, codegen body, codegen chain);
codegen make_var_chain(codegen lh, codegen rh, bool reference);
codegen make_return(codegen exp);
codegen param_to_arg(codegen param);
codegen make_func_call(string_slice name, codegen args);
codegen lambda_to_func(codegen lambda, string_slice name);
codegen var_to_exp(codegen var);
codegen make_function(string_slice type, string_slice name, codegen params, codegen body);
codegen make_struct_init(string_slice type, codegen prop);
codegen make_prop_init(string_slice type, codegen exp, codegen chain);