#pragma once

#include "parser/ast.h"
#include "ir/general.h"

void gen_ir_lisp(stack_navigator *sn, int depth, s_exp_code *code);