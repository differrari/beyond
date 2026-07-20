#include "syscalls/syscalls.h"
#include "files/buffer.h"
#include "files/helpers.h"
#include "alloc/allocate.h"
#include "interpreter/imaginal.h"
#include "ir/irgen.h"
#include "ir/arch_transformer.h"
#include "interpreter/repl.h"

static buffer buf;

bool parse_arguments(int argc, char *argv[]){
    buf = (buffer){
        .buffer = zalloc(0x10000),
        .limit = 0x10000,
        .buffer_size = 0,
        .options = buffer_can_grow,
        .cursor = 0,
    };
    for (int i = 1; i < argc; i++){
        if (*argv[i] != '-'){
            char *content = read_full_file(argv[i],0);
            if (!content){
                print("Failed to open file %s",argv[i]);
                return false;
            }
            buffer_write_const(&buf, content);
        }
    }
    if (!buf.buffer_size){
        char *content = read_full_file("test.lisp",0);
        buffer_write_const(&buf, content);
    }
    return true;
}

int main(int argc, char *argv[]) {

    if (!parse_arguments(argc, argv)) return -1;
    
    codegen ir = parse_lisp(slice_from_buffer(&buf));

    if (!ir.type || !ir.ptr) return -1;

    // debug_print_ir(ir);

    ir = perform_transformations(ir);

    print("Final result");
    imaginal_print(ir);
    
}
