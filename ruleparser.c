#include "syscalls/syscalls.h"
#include "data/scanner/scanner.h"
#include "data/tokenizer/tokenizer.h"
#include "data/helpers/token_stream.h"

int subrule_count = 0;
int sequence_count = 0;

#define MAX_BUF 0x10000

static char code_buf[MAX_BUF];
uintptr_t cursor;

void emit(char* fmt, ...){
    __attribute__((aligned(16))) va_list args;
    va_start(args, fmt); 
    size_t n = string_format_va_buf(fmt, code_buf+cursor, MAX_BUF-cursor, args);
    cursor += n;
    if (cursor < MAX_BUF-2) code_buf[cursor++] = '\n';
    va_end(args);
}

bool is_capitalized(string_slice sv){
    bool caps = true;
    for (int i = 0; i < sv.length; i++){
        char c = *(sv.data + i);
        caps &= 'A' <= c && c <= 'Z';
    }
    return caps;
}

bool is_dec;

void parse_rule(Token rule, TokenStream *ts){
    Token t = {};
    emit("\t\t{{");
    subrule_count++;
    sequence_count = 0;
    while (ts_next(ts, &t) && t.kind){
        if (t.kind == TOK_OPERATOR && *t.start == '|') {
            emit("\t\t},%i},",sequence_count);
            emit("\t\t{{");
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
                emit("\t\t\tLITERAL(%v),",sv);
            } else if (is_capitalized(sv)){
                Token p1, s, p2;
                uint32_t p = ts->tz->s->pos;
                if (ts_peek(ts, &p1) && p1.kind == TOK_LPAREN && ts_next(ts, &p1) && ts_peek(ts, &s) && s.kind == TOK_STRING && ts_next(ts, &s) && ts_peek(ts, &p2) && p2.kind == TOK_RPAREN && ts_next(ts, &p2)){
                    if (has_tag) {
                        print("Literal isn't expected to have a tag");
                        break;
                    }
                    emit("\t\t\tLITTOK(%v,%v),",sv,token_to_slice(s));
                } else {
                    ts->tz->s->pos = p;
                    if (has_tag){
                        if (is_dec)
                            emit("\t\t\tSYMDEC(%v,%v),",sv,token_to_slice(tag));
                        else 
                            emit("\t\t\tSYMCHECK(%v,%v),",sv,token_to_slice(tag));
                    } else 
                        emit("\t\t\tTOKEN(%v),",sv);
                }
            } else {
                if (has_tag){
                    emit("\t\t\tSYMRULE(%v,%v),",sv,token_to_slice(tag));
                } else 
                    emit("\t\t\tRULE(%v),",sv);
            }
        }
    }
    emit("\t\t},%i},",sequence_count);
}

int main(int argc, char *argv[]){
    char *content = read_full_file("rules.config");
    
    Scanner s = scanner_make(content, strlen(content));
    Tokenizer tk = tokenizer_make(&s);
    tk.parse_newline = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_HASH;
    TokenStream ts;
    ts_init(&ts, &tk);
    
    emit("#include \"rules.h\" \n\ngrammar_rule language_rules[num_grammar_rules] = {");
    
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
                }
                is_dec = slice_lit_match(token_to_slice(tag), "param", false) || slice_lit_match(token_to_slice(tag), "func", false) || slice_lit_match(token_to_slice(tag), "dec", false) || slice_lit_match(token_to_slice(tag), "label", false);
                if (!ts_next(&ts, &op1)){
                    print("Unfinished rule %v",token_to_slice(t));
                    return -1;
                }
            } else is_dec = false;
            if (*op1.start != '-' || *(op1.start+1) != '>'){
                print("Malformed rule %v. Expected -> Got %v",token_to_slice(t),token_to_slice(op1));
                return -1;
            }
            emit("\t[rule_%v] = {{",token_to_slice(t));
            parse_rule(t, &ts);
            if (tag.length)
                emit("\t},%i, sem_%v},",subrule_count,token_to_slice(tag));
            else 
                emit("\t},%i, 0},",subrule_count);
            subrule_count = 0;
        } else {
            print("Unrecognized grammar token %v",token_to_slice(t));
            return -1;
        }
    }
    emit("};");
    
    print("Wrote %i",strlen(code_buf));
    
    if (strlen(code_buf) < MAX_BUF-1) code_buf[strlen(code_buf)] = '\0';
    
    write_full_file("rules.c",code_buf,cursor);
    
    return 0;
}