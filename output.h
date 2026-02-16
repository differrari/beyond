#pragma once

#include "random.h"
#include "ing.h"
char* heyo();
bool credbuild();
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
typedef struct hello hello;
typedef struct hello { 
	void* ptr;
	int (*func)(void* instance, int rule, Token elem);
	void (*test)(void* instance);
 } hello;

int hello_func(hello instance, int rule, Token elem);
void hello_test(hello instance);
typedef struct testType { 
	int prop1;
	int prop2;
 } testType;
void testType_func(testType* instance, int rule, Token elem);
void testType_test(testType* instance);
void testType_helper(testType* instance);
hello testType_init();
int _temp_func_0(char* directory, char* name);
int main();