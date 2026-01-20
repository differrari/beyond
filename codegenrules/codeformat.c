#include "codeformat.h"
#include "common.h"
#include "syscalls/syscalls.h"
#include "files/buffer.h"

#define MAX_BUF 0x10000

buffer *code_buf;
int *current_indent;

emit_block current_eblock = {};

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
        current_indent = &current_eblock.pindent;
    break;
    case block_section_body:
        code_buf = &current_eblock.body;
        current_indent = &current_eblock.bindent;
    break;
    case block_section_epilogue:
        code_buf = &current_eblock.epilogue;
        current_indent = &current_eblock.eindent;
    break;
    }
}

void new_block(){
    current_eblock = (emit_block){
        .prologue = new_buffer(),
        .pindent = current_indent ? *current_indent : 0,
        .body = new_buffer(),
        .bindent = current_indent ? *current_indent : 0,
        .epilogue = new_buffer(),
        .eindent = current_indent ? *current_indent : 0,
    };
    code_buf = &current_eblock.body;
    current_indent = &current_eblock.bindent;
}

emit_block save_and_push_block(){
    if (!code_buf){
        new_block();
    }
    emit_block orig = current_eblock;
    new_block();
    return orig;
}

emit_block pop_and_restore_emit_block(emit_block block){
    emit_block orig = current_eblock;
    current_eblock = block;
    return orig;
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
    emit("\n\%s",indent_by(max(*current_indent,0)));
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
    *current_indent = *current_indent + 1;
}

void decrease_indent(){
    if (!code_buf){
        new_block();
    }
    *current_indent = *current_indent - 1;
}

void collapse_block(emit_block old_block){
    if (old_block.prologue.buffer_size){
        emit_const(old_block.prologue.buffer);
        emit_newline();
    }
    if (old_block.body.buffer_size){
        emit_const(old_block.body.buffer);
        emit_newline();
    }
    if (old_block.epilogue.buffer_size)
        emit_const(old_block.epilogue.buffer);
}

void output_code(const char *path){
    buffer joined = new_buffer();
    if (current_eblock.prologue.buffer_size){ buffer_write_lim(&joined, current_eblock.prologue.buffer, current_eblock.prologue.buffer_size); }
    if (current_eblock.body.buffer_size){ buffer_write_lim(&joined, current_eblock.body.buffer, current_eblock.body.buffer_size); }
    if (current_eblock.epilogue.buffer_size){ buffer_write_lim(&joined, current_eblock.epilogue.buffer, current_eblock.epilogue.buffer_size); }
    write_full_file(path,joined.buffer,joined.buffer_size);
}