#include "codegen/codegen.h"




void codegen_t_register_elem(codegen_t instance ,int type, Token elem){
	if (instance.register_elem) instance.register_elem(instance.ptr, type, elem);
}

void codegen_t_register_subrule(codegen_t instance ,int type, struct codegen_t child){
	if (instance.register_subrule) instance.register_subrule(instance.ptr, type, child);
}

void codegen_t_emit_code(codegen_t instance){
	if (instance.emit_code) instance.emit_code(instance.ptr);
}
