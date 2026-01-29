#pragma once

#include "data/format/tokenizer/tokenizer.h"
#include "codegen/codegen.h"

codegen begin_rule(sem_rule type);
void generate_code(const char *name, codegen cg);