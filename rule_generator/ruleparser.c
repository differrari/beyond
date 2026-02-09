#include "syscalls/syscalls.h"
#include "data/format/scanner/scanner.h"
#include "data/format/tokenizer/tokenizer.h"
#include "data/format/helpers/token_stream.h"
#include "data/struct/linked_list.h"
#include "codegen/codeformat.h"
#include "files/helpers.h"
#include "alloc/allocate.h"

#define MAX_BUF 0x10000

static char code_buf[MAX_BUF];
uintptr_t cursor;

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
    Token value;
    bool optional;
    char *type;
    bool declare;
} rule_sequence;

typedef struct {
    string_slice name;
    clinkedlist_t *list;  
    Token tag;
    bool declaration;
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
    uint64_t opt_mask = 0;
    int count = 0;
    for (clinkedlist_node_t* node = list->head; node; node = node->next){
        rule_sequence *s = node->data;
        if (s && s->optional){ opt_mask |= (1 << num_optionals++); }
        count++;
    }
    for (int i = (1 << num_optionals)-1; i >= 0; i--){
        emit_newline();
        emit("{{ ");
        increase_indent();
        emit_newline();
        int opts = 0;
        int count = 0;
        for (clinkedlist_node_t* node = list->head; node; node = node->next){
            rule_sequence *s = node->data;
            bool should_emit = true;
            if (s && s->optional) {
                if (!((1 << opts++) & i)) {
                    should_emit = false;
                }
            }
            if (should_emit){
                count++;
                if (s->tag.kind){
                    if (s->value.kind){
                        emit("%s(%v,%v,%s,%s,%v), ",s->type,token_to_slice(s->t),token_to_slice(s->tag),s->declare ? "declare" : "check",s->declare ? "sem_elem" : "sem_rule", token_to_slice(s->value));
                    } else emit("%s(%v,%v), ",s->type,token_to_slice(s->t),token_to_slice(s->tag));
                }
                else 
                    emit("%s(%v), ",s->type,token_to_slice(s->t));
            }
        }
        decrease_indent();
        emit_newline();
        emit(" },%i},",count);
    }
    return (1 << num_optionals++)-1;
}

clinkedlist_t * parse_rule(Token rule, TokenStream *ts){
    clinkedlist_t *rulelist = clinkedlist_create();
    Token t = {};
    clinkedlist_t *seq = clinkedlist_create();
    while (ts_next(ts, &t) && t.kind){
        if (t.kind == TOK_OPERATOR && *t.start == '|') {
            if (seq->length){
                clinkedlist_push(rulelist,seq);
                seq = clinkedlist_create();
            }
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
            rule_sequence *seqrule = zalloc(sizeof(rule_sequence));
            if (t.kind == TOK_STRING){
                if (has_tag)
                    seqrule->tag = tag;
                seqrule->t = t;
                seqrule->type = has_tag ? "LITTAG" : "LITERAL";
            } else if (is_capitalized(token_to_slice(t))){
                Token p1, s, p2;
                uint32_t p = ts->tz->s->pos;
                if (ts_peek(ts, &p1) && p1.kind == TOK_LPAREN && ts_next(ts, &p1) && ts_peek(ts, &s) && s.kind == TOK_STRING && ts_next(ts, &s) && ts_peek(ts, &p2) && p2.kind == TOK_RPAREN && ts_next(ts, &p2)){
                    seqrule->type = has_tag ? "SYMTOK" : "LITTOK";
                    seqrule->t = t;
                    seqrule->declare = is_dec;
                    if (has_tag){
                        seqrule->value = s;
                        seqrule->tag = tag;
                    } else seqrule->tag = s;
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
    emit_const("typedef enum { ");
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit("rule_%v,",e->name);
    }
    emit_const(" num_grammar_rules } grammar_rules;");
}

void emit_rules(){
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit_newline();
        emit("[rule_%v] = {{",e->name);
        increase_indent();
        int subrule_count = clinkedlist_count(e->list);
        for (clinkedlist_node_t *o = e->list->head; o; o = o->next){
            subrule_count += emit_sequence(o->data);
        }
        decrease_indent();
        emit_newline();
        if (e->tag.length)
            emit("},%i, sem_rule_%v, sem_action_%s},",subrule_count,token_to_slice(e->tag),e->declaration ? "declare" : "none");
        else 
            emit("},%i, 0},",subrule_count);
    }
}

void emit_rule_prints(){
    emit_newline();
    emit_newline();
    emit("char* rule_names[num_grammar_rules] = {");
    increase_indent();
    emit_newline();
    for (clinkedlist_node_t *node = allrules->head; node; node = node->next){
        rule_entry *e = node->data;
        if (!e) continue;
        emit("\t[rule_%v] = \"%v\",",e->name,e->name);
        emit_newline();
    }
    decrease_indent();
    emit_newline();
    emit("};");
}

int main(int argc, char *argv[]){
    char *content = read_full_file("rules.config",0);
    
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
                is_dec =    slice_lit_match(token_to_slice(tag), "param", false) || 
                            slice_lit_match(token_to_slice(tag), "func", false) || 
                            slice_lit_match(token_to_slice(tag), "dec", false) || 
                            slice_lit_match(token_to_slice(tag), "label", false) || 
                            slice_lit_match(token_to_slice(tag), "struct", false) || 
                            slice_lit_match(token_to_slice(tag), "interf", false) || 
                            slice_lit_match(token_to_slice(tag), "parent", false) || 
                            slice_lit_match(token_to_slice(tag), "enum", false) || 
                            slice_lit_match(token_to_slice(tag), "enum_case", false) || 
                            false ;
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
            entry->declaration = is_dec;
            if (tag.length) entry->tag = tag;
            clinkedlist_push(allrules, entry);
        } else {
            print("Unrecognized grammar token %v",token_to_slice(t));
            return -1;
        }
    }
    emit_const("#include \"rules.h\"");
    emit_newline();
    emit_rule_enum();
    emit_newlines(2);
    emit_const("grammar_rule language_rules[num_grammar_rules] = {");
    increase_indent();
    emit_rules();
    decrease_indent();
    emit_newline();
    emit_const("};");
    
    emit_rule_prints();
    
    if (strlen(code_buf) < MAX_BUF-1) code_buf[strlen(code_buf)] = '\0';
    
    output_code("rules","c");
    
    return 0;
}