#include "syscalls/syscalls.h" 
#include "data/scanner/scanner.h"
#include "parser.h"
#include "data/helpers/token_stream.h"
#include "common.h"
#include "irgen.h"
#include "sem_analysis.h"

Scanner scan;

typedef struct {
    uint32_t file;
    uint32_t line_number;
    uint32_t column;
} ln_report; 

ln_report parse_ln(uint32_t pos, char *content){
    ln_report rep = {
        .line_number = 1,
    };
    for (uint32_t i = 0; i < pos && content[i]; i++){
        if (content[i] == '\n'){
            rep.column = 1;
            rep.line_number++;
        } else rep.column++;
    }
    return rep;
}

int main(int argc, char *argv[]){
    
    const char *compname = argc > 1 ? argv[1] : "street.cred";
    
    print("Compiling %s",compname);
    
    char *content = read_full_file(compname);
    
    scan = scanner_make(content,strlen(content));
    
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
    
    parse_result parse_res = parse(content, &ts, &parser);
    
    if (!parse_res.result){
        ln_report ln = parse_ln(parse_res.furthest_parse_pos, content);
        printf("Found syntax error on l%i:%i in file %i",ln.line_number,ln.column,ln.file);
    } else if (analyze_semantics(parse_res.ast_stack, parse_res.ast_count)){
        const char* outname = argc > 2 ? argv[2] : "output.c";
        print("Generating %s",outname);
        gen_code(parse_res.ast_stack, parse_res.ast_count, outname);
    }
    
    return 0;
}