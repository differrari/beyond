#include "syscalls/syscalls.h" 
#include "data/scanner/scanner.h"
#include "parser.h"
#include "data/helpers/token_stream.h"
#include "common.h"

Scanner scan;

char *indent = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

int main(int argc, char *argv[]){
    
    char *content = read_full_file("street.cred");
    
    scan = scanner_make(content,strlen(content));
    
    Tokenizer tk = tokenizer_make(&scan);
    tk.skip_type_check = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_SLASH;
    // tk.parse_newline = true;
    TokenStream ts;
    ts_init(&ts,&tk);
    
    parser_sm parser = (parser_sm){
        .current_rule = rule_block,
        .option = 0,
        .sequence = 0,
        .scan = &scan,
        .scanner_pos = scan.pos,
    };
    
    parse_result parse_res = parse(content, &ts, &parser);
    
    printf("String scanned %i",parse_res.result);
    if (!parse_res.result)
        printf("Found syntax error at position %i",parse_res.furthest_parse_pos);
    else {
        print("There are %i nodes in the stack",parse_res.ast_count);
        for (uint32_t i = 0; i < parse_res.ast_count; i++){
            if (!parse_res.ast_stack[i].t.kind)
                printf("[%i,%i] %s ->",parse_res.ast_stack[i].rule,parse_res.ast_stack[i].option,indent_by(parse_res.ast_stack[i].depth));
            else
                printf("[%i,%i] %s %v",parse_res.ast_stack[i].rule,parse_res.ast_stack[i].option,indent_by(parse_res.ast_stack[i].depth),token_to_slice(parse_res.ast_stack[i].t));
        }
    }
    
    return 0;
}