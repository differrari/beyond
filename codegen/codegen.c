#include "codegen/codegen.h"




void codegen_register_elem(codegen instance, int type, Token elem){
	if (instance.register_elem){
		return instance.register_elem(instance.ptr, type, elem);
	}
	
}

void codegen_register_subrule(codegen instance, int type, codegen child){
	if (instance.register_subrule){
		return instance.register_subrule(instance.ptr, type, child);
	}
	
}

void codegen_emit_code(codegen instance){
	if (instance.emit_code){
		return instance.emit_code(instance.ptr);
	}
	
}

codegen codegen_transform(codegen instance, codegen this){
	if (instance.transform){
		return instance.transform(instance.ptr, this);
	}
	
	return (codegen){};
}
