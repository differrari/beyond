#include "codegen/codegen.h"




void codegen_register_elem(codegen instance ,int type, Token elem){
	if (instance.register_elem) instance.register_elem(instance.ptr, type, elem);
}

void codegen_register_subrule(codegen instance ,int type, struct codegen child){
	if (instance.register_subrule) instance.register_subrule(instance.ptr, type, child);
}

void codegen_emit_code(codegen instance){
	if (instance.emit_code) instance.emit_code(instance.ptr);
}
