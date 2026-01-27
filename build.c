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
		return run() == 0;
	}
	
	return 0;
}

bool genc(){
	if (rulegen()){
		new_module("genc");
		source("sem_enum.cred");
		set_name("semantic_rules");
		return cred_compile();
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
		add_linker_flag("-g", false);
		source_all(".c");
		if (compile()){
			gen_compile_commands();
			run();
		}
		
	}
	
}

int main(){
	print("Hello world");
	compiler();
	return 0;
}
