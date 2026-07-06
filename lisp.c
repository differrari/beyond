#include "common.h"
#include "syscalls/syscalls.h"
#include "data/format/scanner/scanner.h"
#include "parser/parser.h"
#include "data/format/helpers/token_stream.h"
#include "files/buffer.h"
#include "files/helpers.h"
#include "alloc/allocate.h"
#include "parser/ast.h"
#include "ir/lisp.h"
#include "interpreter/imaginal.h"
#include "ir/irgen.h"
#include "ir/arch_transformer.h"

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
    
    Scanner scan = scanner_make(buf.buffer,strlen(buf.buffer));
        
    Tokenizer tk = tokenizer_make(&scan);
    tk.comment_type = TOKENIZER_COMMENT_TYPE_SLASH;
    tk.parse_newline = argc && strcmp(argv[1],"-nl") == 0;
    TokenStream ts;
    ts_init(&ts, &tk);

    parser_sm parser = (parser_sm){
        .current_rule = 0,
        .option = 0,
        .sequence = 0,
        .scan = &scan,
        .scanner_pos = scan.pos,
    };

    parse_result parse_res = parse(buf.buffer, &ts, &parser);
    
    if (!parse_res.result){
        ln_report ln = parse_ln(parse_res.fail_info.found.pos, buf.buffer);
        print("Expected %s, found %v (%i) in %s (l%i:%i in file %i)",token_name(parse_res.fail_info.expected.value),token_to_slice(parse_res.fail_info.found),parse_res.fail_info.found.kind,rule_names[parse_res.fail_info.rule],ln.line_number,ln.column,ln.file);
        return -1;
    }

    // print_stack(parse_res.ast_stack);
    codegen ir = gen_ir_lisp(parse_res.ast_stack);

    debug_print_ir(ir);

    ir = perform_transformations(ir);

    imaginal_print(ir);
    
}
