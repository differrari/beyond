#include "imaginal.h"
#include "syscalls/syscalls.h"
#include "ir/manual_gen.h"

#define IMAGINAL_DEBUG
#ifdef IMAGINAL_DEBUG
#define imaginal_debug(...) print(__VA_ARGS__)
#else
#define imaginal_debug(...)
#endif

bool is_atom(codegen exp){
    s_exp_code *code = exp.ptr;
    if (!code) return true;
    if (code->cdr.ptr) return false;
    if (code->car.type != car_subexp) return true;
    return false;
} 

static inline codegen is_atom_s_exp(codegen exp){
    return is_atom(exp) ? make_true_atom() : (codegen){};
}

// apply[fn;x;a] =
//      [atom[fn] → [eq[fn;CAR] → caar[x];
//                  eq[fn;CDR] → cdar[x];
//                  eq[fn;CONS] → cons[car[x];cadr[x]];
//                  eq[fn;ATOM] → atom[car[x]];
//                  eq[fn;EQ] → eq[car[x];cadr[x]];
//                  T → apply[eval[fn;a];x;a]];
//      eq[car[fn];LAMBDA] → eval[caddr[fn]; pairlis[cadr[fn];x;a]];
//      eq[car[fn];LABEL] → apply[caddr[fn];x;cons[cons[cadr[fn];caddr[fn]];a]]]

// codegen simple_s_exp()

codegen imaginal_builtin_add(codegen exp){
    s_exp_code *code = exp.ptr;
    if (!code) { print("[ADD error] Add null ptr"); return (codegen){}; }
    if (code->car.type != car_num) { print("[ADD error] non-numeric lh %v",token_to_slice(code->car.token)); return (codegen){}; }
    i64 a = code->car.number;
    i64 b = 0;
    s_exp_code *cdr = code->cdr.ptr;
    if (cdr){
        if (cdr->car.type != car_num) { print("[ADD error] non-numeric rh"); return (codegen){}; }
        b = cdr->car.number;
    }
    imaginal_debug("Add %i + %i = %i",a,b,a+b);
    // codegen r = s_exp_code_init();
    return make_int_atom(a+b);
}

codegen apply(codegen fn_exp, codegen a, imaginal_env env){
    s_exp_code *fn = fn_exp.ptr;
    if (!fn) { print("[APPLY error] Apply null ptr"); return (codegen){}; }
    if (is_atom(fn_exp)){
        imaginal_debug("[APPLY] Atomic expression");
        if (fn->car.type != car_token || fn->car.token.kind != TOK_IDENTIFIER) { print("[APPLY error]: Wrong expression type"); return (codegen){}; }
        string_slice s = token_to_slice(fn->car.token);
        if (slice_lit_match(s, "car", true)){
            // caar[x];
            return (codegen){};
        }
        else if (slice_lit_match(s, "cdr", true)){
            // cdar[x];
            return (codegen){};
        }
        else if (slice_lit_match(s, "cons", true)){
            // cons[car[x];cadr[x]];
            return (codegen){};
        }
        else if (slice_lit_match(s, "atom", true)){
            // atom[car[x]];
            return is_atom_s_exp(a);
        }
        else if (slice_lit_match(s, "eq", true)){
            // eq[car[x];cadr[x]];
            return (codegen){};
        } else {
            return apply(eval(fn_exp,env),a,env);
        }
    } else {
        imaginal_debug("[APPLY] Non-atomic expression");
        if (fn->car.type != car_subexp || !fn->car.subexp.ptr){ print("[APPLY error] not a sub-expression"); return (codegen){}; }
        s_exp_code *car = fn->car.subexp.ptr;
        if (!is_atom(fn->car.subexp) || car->car.type != car_token || car->car.token.kind) { print("[APPLY error] no atomic expression in sub-expression"); return (codegen){}; }
        string_slice s = token_to_slice(car->car.token);
        if (slice_lit_match(s, "lambda", true)){
            imaginal_debug("[APPLY todo] Handle lambda case");
            // eval[caddr[fn]; pairlis[cadr[fn];x;a]];
            return (codegen){};
        }
        if (slice_lit_match(s, "label", true)){
            imaginal_debug("[APPLY todo] Handle lambda case");
            // apply[caddr[fn];x;cons[cons[cadr[fn];caddr[fn]];a]]]
            return (codegen){};
        }
        print("[APPLY error] unknown expression %v in apply",s);
        return (codegen){};
    }
}

// eval[e;a] = 
    // [atom[e] → cdr[assoc[e;a]];
    //  atom[car[e]] →
    //          [eq[car[e];QUOTE] → cadr[e];
    //          eq[car[e];COND] → evcon[cdr[e];a];
    //          T → apply[car[e];evlis[cdr[e];a];a]];
    //  T → apply[car[e];evlis[cdr[e];a];a]]
     
codegen eval(codegen exp, imaginal_env env){
    s_exp_code *code = exp.ptr;
    if (!code) { print("[EVAL error] null ptr"); return (codegen){}; }
    if (is_atom(exp)){
        imaginal_debug("[EVAL todo] handle atomic expression");
        return (codegen){};
    }
    else if (code->car.type == car_token && code->car.token.kind == TOK_IDENTIFIER){
        string_slice s = token_to_slice(code->car.token);
        imaginal_debug("[EVAL] Atomic car expression %v",s);
        if (slice_lit_match(s, "cond", true)){
            imaginal_debug("[EVAL todo] Handle cond case");
            // evcon[cdr[e];a];
            return (codegen){};
        }
        else if (slice_lit_match(s, "quote", true)){
            imaginal_debug("[EVAL todo] Handle quote case");
            //  cadr[e];
            return (codegen){};
        } else if (slice_lit_match(s, "add", true)){
            return imaginal_builtin_add(code->cdr);
        } else {
            // apply[car[e];evlis[cdr[e];a];a]];
            return (codegen){};
        }
    } else if (code->car.type == car_subexp){
        imaginal_debug("[EVAL todo] Has sub-expression");
        // apply[car[e];evlis[cdr[e];a];a]];
        return (codegen){};
    }
    else {
        print("[EVAL error] Unknown expression");
        return (codegen){};
    }
}

codegen s_exp_code_transform(void *ptr, codegen this){
    s_exp_code *code = ptr;
    code->car.subexp = eval(code->car.subexp, 0);
    if (code->cdr.ptr) codegen_transform(code->cdr, code->cdr);
    return this;
}