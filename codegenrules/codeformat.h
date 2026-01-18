#pragma once

#include "std/string_slice.h"
#include "data/tokenizer/tokenizer.h"

void emit_const(char *lit);
void emit(char* fmt, ...);
void emit_token(Token t);
void emit_slice(string_slice slice);
void emit_newline();
void emit_space();
void increase_indent();
void decrease_indent();

void output_code();