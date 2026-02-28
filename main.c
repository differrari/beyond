#include "syscalls/syscalls.h" 
#include "data/format/scanner/scanner.h"
#include "parser/parser.h"
#include "data/format/helpers/token_stream.h"
#include "ir/irgen.h"
#include "semantic/sem_analysis.h"
#include "files/buffer.h"
#include "files/helpers.h"
#include "alloc/allocate.h"
#include "ir/arch_transformer.h"
#include "codegen/code_generator.h"
#include "debug/profiler.h"

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
const char* sym_out = "symbols.sym";
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

int main(int argc, char *argv[]){
    
    profiler_init();
    
    if (!parse_arguments(argc, argv)) return -1;
    
    Scanner scan = scanner_make(buf.buffer,strlen(buf.buffer));
    
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
    
    u64 sud = profiler_delta();
    
    parse_result parse_res = parse(buf.buffer, &ts, &parser);
    
    u64 pd = profiler_delta();
    
    if (!parse_res.result){
        ln_report ln = parse_ln(parse_res.fail_info.found.pos, buf.buffer);
        print("Expected %s, found %v (%i) in %s (l%i:%i in file %i)",token_name(parse_res.fail_info.expected.value),token_to_slice(parse_res.fail_info.found),parse_res.fail_info.found.kind,rule_names[parse_res.fail_info.rule],ln.line_number,ln.column,ln.file);
        return -1;
    } 
    
    // symbol_table *existing_symbols = deserialize_table("test.sym");
    
    symbol_table *symbols = analyze_semantics(parse_res.ast_stack);
    
    if (!symbols) return -1;
    u64 sd = profiler_delta();
    
    // serialize_table(symbols, sym_out);
    
    u64 sg = profiler_delta();
    
    codegen ir = gen_code(parse_res.ast_stack, outname);
    if (!ir.ptr) return -1;
    
    u64 id = profiler_delta();
    
    ir = perform_transformations(ir);
    if (!ir.ptr) return -1;
    
    u64 td = profiler_delta();
        
    generate_code(outname, ir);
    
    u64 gd = profiler_delta();
    
    print("Compilation finished. Total time %llims",sud + pd + sd + sg + id + td + gd);
    print("Setup took %llims",sud);
    print("Parsing took %llims",pd);
    print("Semantic analysis took %llims",sd);
    print("Symbol generation took %llims",sg);
    print("IR Gen took %llims",id);
    print("Transformation took %llims",td);
    print("Generation took %llims",gd);
    
    return 0;
}