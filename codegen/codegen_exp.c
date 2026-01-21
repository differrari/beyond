#include "data/tokenizer/tokenizer.h"

#include "rules.h"

typedef struct codegen_t { 
	void* ptr;
	int type;
	void (*register_elem)(void *instance, int type, Token elem);
	void (*register_subrule)(void *instance, int type, struct codegen_t child);
	void (*emit_code)(void *instance);
 } codegen_t;


void codegen_t_register_elem(codegen_t instance ,int type, Token elem){
	if (instance.register_elem) instance.register_elem(instance.ptr, type, elem);
}

void codegen_t_register_subrule(codegen_t instance ,int type, struct codegen_t child){
	if (instance.register_subrule) instance.register_subrule(instance.ptr, type, child);
}

void codegen_t_emit_code(codegen_t instance){
	if (instance.emit_code) instance.emit_code(instance.ptr);
}

codegen_t begin_rule(sem_rule type){
	
}
