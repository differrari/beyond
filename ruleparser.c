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

static inline bool is_dec(string_slice sv){
    return slice_lit_match(sv, "type", false) || slice_lit_match(sv, "name", false);
}

void parse_rule(Token rule, TokenStream *ts){
    Token t = {};
    print("\t\t{{");
    subrule_count++;
    sequence_count = 0;
    while (ts_next(ts, &t) && t.kind){
        if (t.kind == TOK_OPERATOR && *t.start == '|') {
            print("\t\t},%i},",sequence_count);
            print("\t\t{{");
            subrule_count++;
            sequence_count = 0;
        } else if (t.kind == TOK_NEWLINE) break;
        else {
            bool has_tag = false;
            Token sep = {};
            Token tag;
            if (ts_peek(ts, &sep) && *sep.start == '@'){
                ts_next(ts, &sep);
                if (!ts_next(ts, &tag) || tag.kind != TOK_IDENTIFIER){
                    print("Malformed tag on %v. Expected identifier, got %v",token_to_slice(t),token_to_slice(tag));
                    break;
                } else {
                    has_tag = true;
                    Token test;
                    ts_peek(ts, &test);
                }
            }
            string_slice sv = token_to_slice(t);
            sequence_count++;
            if (t.kind == TOK_STRING){
                if (has_tag) {
                    print("Literal isn't expected to have a tag");
                    break;
                }
                printf("\t\t\tLITERAL(%v),",sv);
            } else if (is_capitalized(sv)){
                Token p1, s, p2;
                uint32_t p = ts->tz->s->pos;
                if (ts_peek(ts, &p1) && p1.kind == TOK_LPAREN && ts_next(ts, &p1) && ts_peek(ts, &s) && s.kind == TOK_STRING && ts_next(ts, &s) && ts_peek(ts, &p2) && p2.kind == TOK_RPAREN && ts_next(ts, &p2)){
                    if (has_tag) {
                        print("Literal isn't expected to have a tag");
                        break;
                    }
                    printf("\t\t\tLITTOK(%v,%v),",sv,token_to_slice(s));
                } else {
                    ts->tz->s->pos = p;
                    if (has_tag){
                        if (is_dec(token_to_slice(tag)))
                            print("\t\t\tSYMDEC(%v,%v),",sv,token_to_slice(tag));
                        else 
                            print("\t\t\tSYMCHECK(%v,%v),",sv,token_to_slice(tag));
                    } else 
                        printf("\t\t\tTOKEN(%v),",sv);
                }
            } else 
                printf("\t\t\tRULE(%v),",sv);
        }
    }
    print("\t\t},%i},",sequence_count);
}

int main(int argc, char *argv[]){
    char *content = read_full_file("rules.config");
    
    Scanner s = scanner_make(content, strlen(content));
    Tokenizer tk = tokenizer_make(&s);
    tk.parse_newline = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_HASH;
    TokenStream ts;
    ts_init(&ts, &tk);
    
    print("#include \"rules.h\" \n\ngrammar_rule language_rules[num_grammar_rules] = {");
    
    Token t;
    while (ts_next(&ts, &t) && t.kind) {
        if (t.kind == TOK_IDENTIFIER) {
            Token op1;
            Token tag = {};
            if (!ts_next(&ts, &op1)){
                print("Unfinished rule %v",token_to_slice(t));
                return -1;
            }
            if (*op1.start == '@'){
                if (!ts_next(&ts, &tag) || tag.kind != TOK_IDENTIFIER){
                    print("Malformed tag on %v. Expected identifier, got %v",token_to_slice(t),token_to_slice(tag));
                    return -1;
                } //else print("Should store tag %v@%v",token_to_slice(t),token_to_slice(tag));
                if (!ts_next(&ts, &op1)){
                    print("Unfinished rule %v",token_to_slice(t));
                    return -1;
                }
            }
            if (*op1.start != '-' || *(op1.start+1) != '>'){
                print("Malformed rule %v. Expected -> Got %v",token_to_slice(t),token_to_slice(op1));
                return -1;
            }
            print("\t[rule_%v] = {{",token_to_slice(t));
            parse_rule(t, &ts);
            if (tag.length)
                print("\t},%i, sem_%v},",subrule_count,token_to_slice(tag));
            else 
                print("\t},%i, 0},",subrule_count);
            subrule_count = 0;
        } else {
            print("Unrecognized grammar token %v",token_to_slice(t));
            return -1;
        }
    }
    print("};");
    return 0;
}