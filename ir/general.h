#pragma once

#include "codegen/codegen.h"
#include "string/slice.h"

#define CCODEGEN
// #define DEBUG_CODEGEN

typedef struct {
    codegen stat;
    codegen chain;
} blk_code;

codegen blk_code_init();

typedef struct {
    string_slice type;
    string_slice name;
    codegen initial_value;
} dec_code;

codegen dec_code_init();

typedef struct {
    string_slice name;
    codegen expression;
} ass_code;

codegen ass_code_init();

typedef struct {
    codegen exp;
    codegen chain;
} arg_code;

codegen arg_code_init();

typedef struct {
    string_slice val;
    codegen var;
    Token operand;
    codegen exp;
    codegen lambda;
    bool paren;
    bool invert;
} exp_code;

codegen exp_code_init();

typedef struct {
    Token name;
    codegen args;
} call_code;

codegen call_code_init();

typedef struct {
    codegen cond;
    codegen scope;
    codegen chain;
} cond_code;

codegen cond_code_init();

typedef struct {
    string_slice jump;
} jmp_code;

codegen jmp_code_init();

typedef struct {
    string_slice name;
} label_code;

codegen label_code_init();

typedef struct {
    Token type;
    Token name;
    codegen chain;
} param_code;

codegen param_code_init();

typedef struct {
    Token type;
    Token name;
    codegen signature;
    codegen args;
    codegen body;
} func_code;

codegen func_code_init();

typedef struct {
    codegen initial;
    codegen increment;
    codegen condition;
    codegen body;
} for_code;

codegen for_code_init();
typedef struct {
    codegen condition;
    codegen body;
} while_code;

codegen while_code_init();
typedef struct {
    codegen condition;
    codegen body;
} dowhile_code;

codegen dowhile_code_init();

typedef struct {
    Token name;
    codegen var;
    codegen expression;
    Token operation;
    enum { var_none, var_deref, var_addr } transform;
} var_code;

codegen var_code_init();

typedef struct {
    Token value;
} inc_code;

codegen inc_code_init();

typedef struct {
    Token name;
    codegen contents;
    Token parent;
} struct_code;

codegen struct_code_init();

typedef struct {
    codegen expression;
} ret_code;

codegen ret_code_init();

typedef struct {
    codegen expression;
} def_code;

codegen def_code_init();

typedef struct {
    Token name;
    codegen contents;
} int_code;

codegen int_code_init();

typedef struct {
    Token name;
    codegen contents;
} enum_code;

codegen enum_code_init();

typedef struct {
    Token name;
    codegen chain;
} enum_case_code;

codegen enum_case_code_init();

typedef struct {
    codegen block;
} else_code;

codegen else_code_init();

typedef struct {
    codegen condition;
    codegen cases;
} switch_code;

codegen switch_code_init();

typedef struct {
    codegen match;
    codegen body;
    codegen chain;
} case_code;

codegen case_code_init();