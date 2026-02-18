#include "redbuild.h"
#include "syscalls/syscalls.h"
bool rulegen(){
	new_module("rulegen");
	set_name("rulegen");
	set_target(target_native);
	set_package_type(package_bin);
	source("ir/manual_gen.c");
	source("codegen/codeformat.c");
	add_precomp_flag("NOTRANSFORM");
	add_precomp_flag("RULECODEGEN");
	source("codegen/identity_transform.c");
	source("codegen/codegen.c");
	source("ir/general.c");
	source("rule_generator/rulecodegen.c");
	source("rule_generator/ruleparser.c");
	if (compile()){
		gen_compile_commands("rule_generator/ruleparser.c");
		return run() == 0;
	}
	
	return 0;
}

bool genc(){
	if (rulegen()){
		return quick_cred("semantic/sem_enum.cred", "semantic/semantic_rules") && quick_cred("codegen/codegen.cred", "codegen/codegen") && true;
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
		add_precomp_flag("CCODEGEN");
		add_precomp_flag("CTRANS");
		debug();
		source_all(".c");
		if (compile()){
			gen_compile_commands(0);
			print("Running cred");
			run();
		}
		
	}
	
}

int main(){
	int __return_val;
	u64 start = get_time();
	print("Start at %i", start);
	rebuild_self();
	compiler();
	u64 end = get_time();
	print("Compilation finished %lims", end - start);
	__return_val = 0;
	goto defer;
	defer:
	emit_compile_commands();
	return __return_val;
}
