#pragma once

#include "data/format/tokenizer/tokenizer.h"

#include "rules.h"

typedef struct codegen codegen;
typedef struct codegen { 
	void* ptr;
	int type;
	void (*register_elem)(void *instance, int type, Token elem);
	void (*register_subrule)(void *instance, int type, codegen child);
	void (*emit_code)(void *instance);
	codegen (*transform)(void *instance, codegen this);
 } codegen;

void codegen_register_elem(codegen instance ,int type, Token elem);

void codegen_register_subrule(codegen instance ,int type, codegen child);

void codegen_emit_code(codegen instance);

codegen codegen_transform(codegen instance ,codegen this);
