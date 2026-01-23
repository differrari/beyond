#include "output.h"

int a(){
	if (compile()){
		return run() == 0;
	}
	
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

void hello_func(hello instance ,int rule, Token elem){
	if (instance.func) instance.func(instance.ptr, rule, elem);
}

void hello_test(hello instance){
	if (instance.test) instance.test(instance.ptr);
}



hello testType_init(){
/* Parent conformance here. Requires semantic analysis 
    return (hello){
        .ptr = zalloc(sizeof(testType)),
        .fn1 = testType_fn1,
        .fn2 = testType_fn2,
        .fn3 = testType_fn3,
    };*/
}

int main(){
int _return_val = 0;

	int y = a();
	int x = 1 - (0 - 2);
	int z = x[0];
	print("This is democracy");
	
	
	for (int i = 0; i < 10; i = i + 1){}
	while (0){print();}
	_return_val = 0;
	goto main_defer;
	do {print();}while (0);
	y_loop:
	x_loop:
	draw(x, y, 0xFF00FF00, 0);
	x = x + 1;
	x = y.run();
	if (x < 100){
		goto x_loop;
	} else {
		print("Done with x");
	}
	y = y + 1;
	if (y < 100){
		goto y_loop;
	} else if (y == 100){
		print("Exactly 100 y");
	}
	
	print();
main_defer:
cleanup();
x = 0;
return _return_val;
}
