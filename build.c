#include "redbuild.h"
#include "syscalls/syscalls.h"
bool rulegen(){
	new_module("rulegen");
	set_name("rulegen");
	set_target(target_native);
	set_package_type(package_bin);
	source("common.c");
	source("codegenrules/codeformat.c");
	source("ruleparser.c");
	if (compile()){
		int i = run();
		if (i == 0){
			return 1;
		}
		
		return 0;
	}
	
	return 0;
}

bool genc(){
	if (rulegen()){
		new_module("genc");
		source("sem_enum.cred");
		set_name("semantic_rules.h");
		cred_compile();
		return true;
	}
	
	return false;
}

void compiler(){
	if (genc()){
		new_module("compiler");
		set_name("cred");
		set_target(target_native);
		set_package_type(package_bin);
		ignore_source("ruleparser.c");
		ignore_source("build.c");
		ignore_source("output.c");
		ignore_source("codegen_exp.c");
		source_all(".c");
		if (compile()){
			
		}
		
	}
	
}

int main(){
	printf("Hello world");
	compiler();
	return 0;
}
