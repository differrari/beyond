#pragma once

#include "codegen/codegen.h"

#define TRANSFORM(name) if (code->name.ptr) code->name = codegen_transform(code->name, code->name)

codegen perform_transformations(codegen root);