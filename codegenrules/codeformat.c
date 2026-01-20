#include "codeformat.h"
#include "std/string.h"
#include "common.h"
#include "syscalls/syscalls.h"
#include "files/buffer.h"

#define MAX_BUF 0x10000

buffer *code_buf;
int depth = 0;

static emit_block current_eblock = {};

buffer new_buffer(){
    return (buffer){
        .buffer = zalloc(0x10000),
        .limit = 0x10000,
        .can_grow = true,
        .buffer_size = 0,
        .cursor = 0,
        .circular = false,
    };
}

void switch_block_section(emit_block_section section){
    switch (section) {
    case block_section_prologue:
        code_buf = &current_eblock.prologue;
    break;
    case block_section_body:
        code_buf = &current_eblock.body;
    break;
    case block_section_epilogue:
        code_buf = &current_eblock.epilogue;
    break;
    }
}

void new_block(){
    current_eblock = (emit_block){
        .prologue = new_buffer(),
        .body = new_buffer(),
        .epilogue = new_buffer()
    };
    code_buf = &current_eblock.body;
}

emit_block create_block(){
    new_block();
    return current_eblock;
}

emit_block save_emit_block(){
    return current_eblock;
}

void restore_emit_block(emit_block block){
    current_eblock = block;
}

void emit_const(char *lit){
    if (!code_buf){
        new_block();
    }
    buffer_write_const(code_buf,lit);
}

void emit(char* fmt, ...){
    if (!code_buf){
        new_block();
    }
    __attribute__((aligned(16))) va_list args;
    va_start(args, fmt); 
    buffer_write_va(code_buf, fmt, args);
    va_end(args);
}

void emit_token(Token t){
    if (!code_buf){
        new_block();
    }
    buffer_write_lim(code_buf, (char*)t.start, t.length);
}

void emit_slice(string_slice slice){
    if (!code_buf){
        new_block();
    }
    buffer_write_lim(code_buf, (char*)slice.data, slice.length);
}

void emit_space(){
    if (!code_buf){
        new_block();
    }
    emit_const(" ");
}

void emit_newline(){
    if (!code_buf){
        new_block();
    }
    emit("\n\%s",indent_by(depth));
}

void emit_newlines(int amount){
    if (!code_buf){
        new_block();
    }
    for (int i = 0; i < amount; i++){
        emit_newline();
    }
}

void increase_indent(){
    if (!code_buf){
        new_block();
    }
    depth++;
}

void decrease_indent(){
    if (!code_buf){
        new_block();
    }
    depth--;
}

void output_code(const char *path){
    write_full_file(path,code_buf->buffer,code_buf->buffer_size);
}