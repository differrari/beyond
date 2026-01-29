#include "data/format/tokenizer/tokenizer.h"

#include "rules.h"

typedef struct codegen { 
	void* ptr;
	int type;
	void (*register_elem)(void *instance, int type, Token elem);
	void (*register_subrule)(void *instance, int type, struct codegen child);
	void (*emit_code)(void *instance);
 } codegen;

void codegen_register_elem(codegen instance ,int type, Token elem);

void codegen_register_subrule(codegen instance ,int type, struct codegen child);

void codegen_emit_code(codegen instance);
