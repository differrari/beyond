int a();
void draw(int x, int y, int color, int overwrite);
void c(int x);
void print();
extern int t;
typedef enum {
	test_enum_one,
	test_enum_two,
	test_enum_three,
} test_enum;

char* test_enum_to_string(test_enum val);

typedef struct hello { 
	void* ptr;
	void (*func)(void *instance, int rule, Token elem);
	void (*test)(void *instance);
 } hello;

void hello_func(hello instance ,int rule, Token elem);

void hello_test(hello instance);

typedef struct testType { 
	
	int prop1;
	int prop2;
 } testType;

void testType_func(void *parent, int rule, Token elem);
void testType_test(void *parent);
hello testType_init();

int main();