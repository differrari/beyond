#include "output.h"



char* heyo(){
	return "Value";
}

bool credbuild(){
	redb_mod module = new_module("cred");
	set_name(&module, "credgen");
	set_target(&module, target_native);
	redb_mod_source_all(&module, ".cred");
	return redb_mod_cred_compile(module);
	switch (hello){
		case 1: {print("1");} break;
		case 2: {print("22");} break;
		case 3: {print("3");} break;
	}
}

int a(){
	chunk_array_t* testarr;
	char* ly = "like";
	int test = 0;
	testarr = chunk_array_create(12, sizeof(int));
	int x = *(int*)chunk_array_get(testarr, 0);
	chunk_array_push(testarr, 0);
}

void draw(int x, int y, int color, int overwrite){
	int b = 0;
}

void c(int x){
	int l;
	if (x){
		return 0;
	}
	
}

void print(){
	
}

int t = u + i;


char* test_enum_to_string(test_enum val){
	switch (val) {
		case test_enum_one: return "one";
		case test_enum_two: return "two";
		case test_enum_three: return "three";
	} 
} 


int hello_func(hello instance, int rule, Token elem){
	if (instance.func){
		return instance.func(instance.ptr, rule, elem);
	}
	
	return (int){};
}

void hello_test(hello instance){
	if (instance.test){
		return instance.test(instance.ptr);
	}
	
	return (){};
}



hello testType_init(){
	return (hello){
		.ptr = zalloc(sizeof(testType)),
		.func = testType_func,
		.test = testType_test,
	};
}
void testType_func(testType* instance, int rule, Token elem){
	int a = 0;
}

void testType_test(testType* instance){
	print("Test succeeded %i", instance->prop1);
}

void testType_helper(testType* instance){
	
}

int _temp_func_0(char* directory, char* name){
	print("Inside lambda");
}

int main(){
	int __return_val;
	int y = a();
	int x = 1 - (0 - 2);
	int z = int_get(x, 0);
	print("This is democracy manifest");
	for (int i = 0; i < 10; i = i + 1){}
	while (0){print();}
	__return_val = 0;
	goto defer;
	do {print();}while (0);
	draw(x, y, 0xFF00FF00, 0);
	x = x + 1;
	x = int_run(y);
	if (x < 100){
		
	} else {
		print("Done with x");
	}
	y = y + 1;
	if (y < 100){
		
	} else if (y == 100){
		print("Exactly 100 y");
	}
	
	print();
	traverse_directory(s.data, true, _temp_func_0);
	defer:
	cleanup();
	x = 0;
	return __return_val;
}
