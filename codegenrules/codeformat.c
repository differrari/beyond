#include "codeformat.h"
#include "std/string.h"
#include "common.h"
#include "syscalls/syscalls.h"

#define MAX_BUF 0x10000

int depth;
static char code_buf[MAX_BUF];
uintptr_t code_cursor;

void emit_const(char *lit){
    for (int i = 0; lit[i]; i++){
        if (code_cursor >= MAX_BUF) return;
        code_buf[code_cursor++] = lit[i];
    }
}

void emit(char* fmt, ...){
    __attribute__((aligned(16))) va_list args;
    va_start(args, fmt); 
    size_t n = string_format_va_buf(fmt, code_buf+code_cursor, MAX_BUF-code_cursor, args);
    code_cursor += n;
    va_end(args);
}

void emit_token(Token t){
    for (int i = 0; i < t.length; i++){
        if (code_cursor >= MAX_BUF) return;
        code_buf[code_cursor++] = *(t.start + i);
    }
}

void emit_slice(string_slice slice){
    for (int i = 0; i < slice.length; i++){
        if (code_cursor >= MAX_BUF) return;
        code_buf[code_cursor++] = slice.data[i];
    }
}

void emit_space(){
    emit_const(" ");
}

void emit_newline(){
    emit("\n\%s",indent_by(depth));
}

void emit_newlines(int amount){
    for (int i = 0; i < amount; i++){
        emit_newline();
    }
}

void increase_indent(){
    depth++;
}

void decrease_indent(){
    depth--;
}

void output_code(const char *path){
    write_full_file(path,code_buf,code_cursor);
}