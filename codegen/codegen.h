#pragma once

#include "data/format/tokenizer/tokenizer.h"
#include "rules.h"
typedef struct codegen codegen;
struct codegen { 
	void* ptr;
	int type;
	void (*register_elem)(codegen instance, int type, Token elem);
	void (*register_subrule)(codegen instance, int type, codegen child);
	void (*debug_print)(codegen instance, codegen this, int depth);
	bool (*emit_code)(codegen instance);
	codegen (*transform)(codegen instance, codegen this);
	codegen (*get_subscope)(codegen instance);
 };

void codegen_register_elem(codegen instance, int type, Token elem);
void codegen_register_subrule(codegen instance, int type, codegen child);
void codegen_debug_print(codegen instance, codegen this, int depth);
bool codegen_emit_code(codegen instance);
codegen codegen_transform(codegen instance, codegen this);
codegen codegen_get_subscope(codegen instance);