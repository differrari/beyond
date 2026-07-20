#include "repl.h"
#include "data/format/tokenizer/tokenizer.h"
#include "data/format/helpers/token_stream.h"
#include "parser/parser.h"
#include "ir/lisp.h"

codegen parse_lisp(string_slice code){
    Scanner scan = scanner_make(code.data,strlen(code.data));
        
    Tokenizer tk = tokenizer_make(&scan);
    tk.comment_type = TOKENIZER_COMMENT_TYPE_SEMI;
    tk.parse_newline = false;
    TokenStream ts;
    ts_init(&ts, &tk);

    parser_sm parser = (parser_sm){
        .current_rule = 0,
        .option = 0,
        .sequence = 0,
        .scan = &scan,
        .scanner_pos = scan.pos,
    };

    parse_result parse_res = parse(code.data, &ts, &parser);
    
    if (!parse_res.result){
        ln_report ln = parse_ln(parse_res.fail_info.found.pos, code.data);
        print("Expected %s, found %v (%i) in %s (l%i:%i in file %i)",token_name(parse_res.fail_info.expected.value),token_to_slice(parse_res.fail_info.found),parse_res.fail_info.found.kind,rule_names[parse_res.fail_info.rule],ln.line_number,ln.column,ln.file);
        return (codegen){};
    }

    return gen_ir_lisp(parse_res.ast_stack);
}

codegen *repl_env = 0;

imaginal_fallback imaginal_fallback_fncall;

bool repl_run(string_slice code, void *_ctx){
    imaginal_repl_ctx *ctx = _ctx;
    if (!ctx) return false;

    imaginal_fallback_fncall = ctx->fallback;
    
    codegen ir = parse_lisp(code);

    if (!ir.type || !ir.ptr){
        return false;
    } 

    repl_env = &ctx->environment;

    ir = perform_transformations(ir);

    if (ctx->should_print) {
        imaginal_print(ir);
    }

    ctx->output = cons(ctx->output,ir);

    return true;
}

codegen s_exp_code_transform(codegen instance, codegen this){
    return evlis(this, repl_env ?: &nil_exp);
}

#if !defined(CTRANS) && !defined(RULETRANSFORM)
codegen perform_transformations(codegen root){
    return codegen_transform(root,root);
}
#endif