#include "syscalls/syscalls.h"
#include "data/scanner/scanner.h"
#include "data/tokenizer/tokenizer.h"
#include "data/helpers/token_stream.h"

int subrule_count = 0;
int sequence_count = 0;

bool is_capitalized(string_slice sv){
    bool caps = true;
    for (int i = 0; i < sv.length; i++){
        char c = *(sv.data + i);
        caps &= 'A' <= c && c <= 'Z';
    }
    return caps;
}

void parse_rule(Token rule, TokenStream *ts){
    Token t = {};
    print("\t{{");
    subrule_count++;
    sequence_count = 0;
    while (ts_next(ts, &t) && t.kind){
        if (t.kind == TOK_OPERATOR && *t.start == '|') {
            print("\t},%i},",sequence_count);
            print("\t{{");
            subrule_count++;
            sequence_count = 0;
        } else if (t.kind == TOK_NEWLINE) break;
        else {
            string_slice sv = token_to_slice(t);
            sequence_count++;
            if (is_capitalized(sv))
                printf("\t\tTOKEN(%v),",sv);
            else 
                printf("\t\tRULE(%v),",sv);
        }
    }
    print("\t},%i},",sequence_count);
    print("},%i},",subrule_count);
    subrule_count = 0;
}

int main(int argc, char *argv[]){
    char *content = read_full_file("rules.config");
    
    Scanner s = scanner_make(content, strlen(content));
    Tokenizer tk = tokenizer_make(&s);
    tk.parse_newline = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_HASH;
    TokenStream ts;
    ts_init(&ts, &tk);
    
    Token t;
    while (ts_next(&ts, &t) && t.kind) {
        switch (t.kind) {
            case TOK_IDENTIFIER:{
                Token op1;
                if (!ts_next(&ts, &op1) || *op1.start != '-' || *(op1.start+1) != '>'){
                    print("Malformed rule %v. Expected -> Got %v",token_to_slice(t),token_to_slice(op1));
                    return -1;
                }
                print("[rule_%v] = {{",token_to_slice(t));
                parse_rule(t, &ts);
            }
            break;
            default: 
                print("Unrecognized grammar token %v",token_to_slice(t));
                return -1;
            break;
        }
    }
    return 0;
}