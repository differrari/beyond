#include "codeformat.h"
#include "std/string.h"
#include "common.h"
#include "syscalls/syscalls.h"
#include "files/buffer.h"

#define MAX_BUF 0x10000

static buffer code_buf = {};
int depth = 0;

void new_buffer(){
    code_buf = (buffer){
        .buffer = zalloc(0x10000),
        .limit = 0x10000,
        .can_grow = true,
        .buffer_size = 0,
        .cursor = 0,
        .circular = false,
    };
}

void emit_const(char *lit){
    if (!code_buf.buffer){
        new_buffer();
    }
    buffer_write_const(&code_buf,lit);
}

void emit(char* fmt, ...){
    if (!code_buf.buffer){
        new_buffer();
    }
    __attribute__((aligned(16))) va_list args;
    va_start(args, fmt); 
    buffer_write_va(&code_buf, fmt, args);
    va_end(args);
}

void emit_token(Token t){
    if (!code_buf.buffer){
        new_buffer();
    }
    buffer_write_lim(&code_buf, (char*)t.start, t.length);
}

void emit_slice(string_slice slice){
    if (!code_buf.buffer){
        new_buffer();
    }
    buffer_write_lim(&code_buf, (char*)slice.data, slice.length);
}

void emit_space(){
    if (!code_buf.buffer){
        new_buffer();
    }
    emit_const(" ");
}

void emit_newline(){
    if (!code_buf.buffer){
        new_buffer();
    }
    emit("\n\%s",indent_by(depth));
}

void emit_newlines(int amount){
    if (!code_buf.buffer){
        new_buffer();
    }
    for (int i = 0; i < amount; i++){
        emit_newline();
    }
}

void increase_indent(){
    if (!code_buf.buffer){
        new_buffer();
    }
    depth++;
}

void decrease_indent(){
    if (!code_buf.buffer){
        new_buffer();
    }
    depth--;
}

void output_code(const char *path){
    write_full_file(path,code_buf.buffer,code_buf.buffer_size);
}