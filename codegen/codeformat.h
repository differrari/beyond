#pragma once

#include "data/format/tokenizer/tokenizer.h"

void reset_emit_buffer();
void emit_const(char *lit);
void emit(char* fmt, ...);
void emit_token(Token t);
void emit_slice(string_slice slice);
void emit_newline();
void emit_newlines(int amount);
void emit_space();
void increase_indent(bool newline);
void decrease_indent(bool newline);

void output_code(const char *path, const char *extension);