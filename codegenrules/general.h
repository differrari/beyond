#pragma once

#include "codegen.h"

#define CCODEGEN
// #define DEBUG_CODEGEN

typedef struct {
    codegen_t stat;
    codegen_t chain;
} blk_code;

codegen_t blk_code_init();

typedef struct {
    Token type;
    Token name;
    codegen_t initial_value;
} dec_code;

codegen_t dec_code_init();

typedef struct {
    Token name;
    codegen_t expression;
} ass_code;

codegen_t ass_code_init();

typedef struct {
    codegen_t exp;
    codegen_t chain;
} arg_code;

codegen_t arg_code_init();

typedef struct {
    Token val;
    Token operand;
    codegen_t exp;
} exp_code;

codegen_t exp_code_init();

typedef struct {
    Token name;
    codegen_t args;
} call_code;

codegen_t call_code_init();

typedef struct {
    codegen_t cond;
    codegen_t scope;
} cond_code;

codegen_t cond_code_init();

typedef struct {
    Token jump;
} jmp_code;

codegen_t jmp_code_init();

typedef struct {
    Token name;
} label_code;

codegen_t label_code_init();

typedef struct {
    Token type;
    Token name;
    codegen_t chain;
} param_code;

codegen_t param_code_init();

typedef struct {
    Token type;
    Token name;
    codegen_t args;
    codegen_t body;
} func_code;

codegen_t func_code_init();

typedef struct {
    codegen_t initial;
    codegen_t increment;
    codegen_t condition;
    codegen_t body;
} for_code;

codegen_t for_code_init();
typedef struct {
    codegen_t condition;
    codegen_t body;
} while_code;

codegen_t while_code_init();
typedef struct {
    codegen_t condition;
    codegen_t body;
} dowhile_code;

codegen_t dowhile_code_init();