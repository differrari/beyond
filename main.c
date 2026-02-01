#include "syscalls/syscalls.h" 
#include "data/format/scanner/scanner.h"
#include "parser.h"
#include "data/format/helpers/token_stream.h"
#include "common.h"
#include "irgen.h"
#include "sem_analysis.h"
#include "files/buffer.h"
#include "files/helpers.h"

Scanner scan;

typedef struct {
    uint32_t file;
    uint32_t line_number;
    uint32_t column;
} ln_report; 

ln_report parse_ln(uint32_t pos, char *content){
    ln_report rep = {
        .line_number = 1,
        .column = 1,
    };
    for (uint32_t i = 0; i < pos && content[i]; i++){
        if (content[i] == '\n'){
            rep.column = 1;
            rep.line_number++;
        } else rep.column++;
    }
    return rep;
}

const char* outname = "output";
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
        } else {
            if (strcmp(argv[i], "-o") == 0){
                i++;
                outname = argv[i];
            }
        }
    }
    if (!buf.buffer_size){
        char *content = read_full_file("street.cred",0);
        buffer_write_const(&buf, content);
    }
    return true;
}

int *********************************************************a;

int main(int argc, char *argv[]){
    
    if (!parse_arguments(argc, argv)) return -1;
    
    scan = scanner_make(buf.buffer,strlen(buf.buffer));
    
    Tokenizer tk = tokenizer_make(&scan);
    tk.skip_type_check = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_SLASH;
    tk.parse_newline = argc && strcmp(argv[1],"-nl") == 0;
    TokenStream ts;
    ts_init(&ts,&tk);
    
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
        print("Expected %s, found %v in %s (l%i:%i in file %i)",token_name(parse_res.fail_info.expected.value),token_to_slice(parse_res.fail_info.found),rule_names[parse_res.fail_info.rule],ln.line_number,ln.column,ln.file);
        return -1;
    } else if (analyze_semantics(parse_res.ast_stack, parse_res.ast_count)){
        print("Generating %s",outname);
        gen_code(parse_res.ast_stack, parse_res.ast_count, outname);
    }
    
    return 0;
}