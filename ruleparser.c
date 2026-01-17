#include "syscalls/syscalls.h"
#include "data/scanner/scanner.h"
#include "data/tokenizer/tokenizer.h"
#include "data/helpers/token_stream.h"
#include "data_struct/linked_list.h"

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

typedef struct {
    Token t;
    Token tag;
    bool optional;
    char *type;
} rule_sequence;

typedef struct {
    string_slice name;
    clinkedlist_t *list;  
    Token tag;
} rule_entry;

clinkedlist_t *allrules;

int rule_to_index(string_slice name){
    int i = 0;
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        if (slices_equal(e->name, name, true)) return i;
        i++;
    }
    return -1;
}

int emit_sequence(clinkedlist_t *list){
    if (!list->length) return 0;
    int num_optionals = 0;
    for (clinkedlist_node_t* node = list->head; node; node = node->next){
        rule_sequence *s = node->data;
        if (s && s->optional) num_optionals++;
    }
    for (int i = num_optionals; i >= 0; i--){
        emit("\t\t{{");
        int taken = 0;
        int count = 0;
        for (clinkedlist_node_t* node = list->head; node; node = node->next){
            rule_sequence *s = node->data;
            bool should_emit = true;
            if (s && s->optional) {
                if (taken++ >= i) should_emit = false;
            }
            if (should_emit){
                count++;
                if (s->tag.kind)
                    emit("\t\t\t%s(%v,%v),",s->type,token_to_slice(s->t),token_to_slice(s->tag));
                else 
                    emit("\t\t\t%s(%v),",s->type,token_to_slice(s->t));
            }
        }
        emit("\t\t},%i},",count);
    }
    return num_optionals;
}

clinkedlist_t * parse_rule(Token rule, TokenStream *ts){
    clinkedlist_t *rulelist = clinkedlist_create();
    Token t = {};
    subrule_count++;
    sequence_count = 0;
    clinkedlist_t *seq = clinkedlist_create();
    while (ts_next(ts, &t) && t.kind){
        if (t.kind == TOK_OPERATOR && *t.start == '|') {
            if (seq->length){
                clinkedlist_push(rulelist,seq);
                seq = clinkedlist_create();
            }
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
            sequence_count++;
            rule_sequence *seqrule = zalloc(sizeof(rule_sequence));
            if (t.kind == TOK_STRING){
                if (has_tag) {
                    print("Literal isn't expected to have a tag");
                    break;
                }
                seqrule->t = t;
                seqrule->type = "LITERAL";
            } else if (is_capitalized(token_to_slice(t))){
                Token p1, s, p2;
                uint32_t p = ts->tz->s->pos;
                if (ts_peek(ts, &p1) && p1.kind == TOK_LPAREN && ts_next(ts, &p1) && ts_peek(ts, &s) && s.kind == TOK_STRING && ts_next(ts, &s) && ts_peek(ts, &p2) && p2.kind == TOK_RPAREN && ts_next(ts, &p2)){
                    if (has_tag) {
                        print("Literal isn't expected to have a tag");
                        break;
                    }
                    seqrule->type = "LITTOK";
                    seqrule->t = t;
                    seqrule->tag = s;
                } else {
                    ts->tz->s->pos = p;
                    if (has_tag){
                        if (is_dec){
                            seqrule->type = "SYMDEC";
                            seqrule->t = t;
                            seqrule->tag = tag;
                        } else {
                            seqrule->type = "SYMCHECK";
                            seqrule->t = t;
                            seqrule->tag = tag;
                        }
                    } else {
                        seqrule->type = "TOKEN";
                        seqrule->t = t;
                    }
                }
            } else {
                if (has_tag){
                    seqrule->type = "SYMRULE";
                    seqrule->t = t;
                    seqrule->tag = tag;
                } else {
                    seqrule->type = "RULE";
                    seqrule->t = t;
                }
            }
            Token opt;
            if (ts_peek(ts, &opt) && opt.kind == TOK_OPERATOR && *opt.start == '?'){
                seqrule->optional = true;
                ts_next(ts, &opt);
            }
            clinkedlist_push(seq, seqrule);
        }
    }
    clinkedlist_push(rulelist,seq);
    return rulelist;
}

void emit_rule_enum(){
    emit("typedef enum { ");
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit("\trule_%v,",e->name);
    }
    emit(" \tnum_grammar_rules\n} grammar_rules;");
}

void emit_rules(){
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit("\t[rule_%v] = {{",e->name);
        subrule_count = clinkedlist_length(e->list);
        for (clinkedlist_node_t *o = e->list->head; o; o = o->next){
            subrule_count += emit_sequence(o->data);
        }
        if (e->tag.length)
            emit("\t},%i, sem_%v},",subrule_count,token_to_slice(e->tag));
        else 
            emit("\t},%i, 0},",subrule_count);
    }
}

void emit_rule_prints(){
    emit("\nchar* rule_names[num_grammar_rules] = {");
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit("\t[rule_%v] = \"%v\",",e->name,e->name);
    }
    emit("};");
}

int main(int argc, char *argv[]){
    char *content = read_full_file("rules.config");
    
    Scanner s = scanner_make(content, strlen(content));
    Tokenizer tk = tokenizer_make(&s);
    tk.parse_newline = true;
    tk.comment_type = TOKENIZER_COMMENT_TYPE_HASH;
    TokenStream ts;
    ts_init(&ts, &tk);
    
    allrules = clinkedlist_create();
    
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
            rule_entry *entry = zalloc(sizeof(rule_entry));
            entry->name = token_to_slice(t);
            entry->list = parse_rule(t, &ts);
            if (tag.length) entry->tag = tag;
            clinkedlist_push(allrules, entry);
            subrule_count = 0;
        } else {
            print("Unrecognized grammar token %v",token_to_slice(t));
            return -1;
        }
    }
    emit("#include \"rules.h\" \n");
    emit_rule_enum();
    emit("\ngrammar_rule language_rules[num_grammar_rules] = {");
    emit_rules();
    emit("};");
    
    emit_rule_prints();
    
    if (strlen(code_buf) < MAX_BUF-1) code_buf[strlen(code_buf)] = '\0';
    
    write_full_file("rules.c",code_buf,cursor);
    
    return 0;
}