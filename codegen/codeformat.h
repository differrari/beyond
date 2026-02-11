#pragma once

#include "std/string_slice.h"
#include "data/format/tokenizer/tokenizer.h"
#include "files/buffer.h"

void reset_emit_buffer();
void emit_const(char *lit);
void emit(char* fmt, ...);
void emit_token(Token t);
void emit_slice(string_slice slice);
void emit_newline();
void emit_newlines(int amount);
void emit_space();
void increase_indent();
void decrease_indent();

void output_code(const char *path, const char *extension);