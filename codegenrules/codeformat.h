#pragma once

#include "std/string_slice.h"
#include "data/tokenizer/tokenizer.h"
#include "files/buffer.h"

typedef struct {
    buffer prologue;
    int pindent;
    buffer body;
    int bindent;
    buffer epilogue;
    int eindent;
} emit_block;

typedef enum { block_section_prologue, block_section_body, block_section_epilogue } emit_block_section;

extern emit_block current_eblock;

emit_block save_and_push_block();
emit_block pop_and_restore_emit_block(emit_block block);
void switch_block_section(emit_block_section section);
void collapse_block(emit_block old_block);

void emit_const(char *lit);
void emit(char* fmt, ...);
void emit_token(Token t);
void emit_slice(string_slice slice);
void emit_newline();
void emit_newlines(int amount);
void emit_space();
void increase_indent();
void decrease_indent();

void output_code(const char *path);