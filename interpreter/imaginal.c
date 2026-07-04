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
    return exp.type == sem_rule_lisp_val;
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

codegen imaginal_builtin_add(codegen exp){
    s_exp_code *code = exp.ptr;
    if (!code) { print("[ADD error] Add null ptr"); return (codegen){}; }
    lisp_val_code *car_val = code->car.ptr;
    if (car_val->type != car_num) { print("[ADD error] non-numeric lh %v",car_val->val); return (codegen){}; }
    i64 a = car_val->number;
    i64 b = 0;
    s_exp_code *cdr = code->cdr.ptr;
    if (cdr){
        lisp_val_code *cdr_val = (code->cdr.type == sem_rule_lisp_val) ? code->cdr.ptr : ((s_exp_code*)code->cdr.ptr)->car.ptr;
        if (cdr_val->type != car_num) { print("[ADD error] non-numeric rh"); return (codegen){}; }
        b = cdr_val->number;
    }
    imaginal_debug("Add %i + %i = %i",a,b,a+b);
    // codegen r = s_exp_code_init();
    return make_int_atom(a+b);
}

string_slice car_id(codegen car){
    if (car.type != sem_rule_lisp_val) return (string_slice){};
    lisp_val_code *code = car.ptr;
    if (code->type != car_identifier) return (string_slice){};
    return code->val;
}

codegen apply(codegen fn_exp, codegen a, imaginal_env env){
    s_exp_code *fn = fn_exp.ptr;
    if (!fn) { print("[APPLY error] Apply null ptr"); return (codegen){}; }
    if (is_atom(fn_exp)){
        imaginal_debug("[APPLY] Atomic expression");
        string_slice s = car_id(fn->car);
        if (!s.length) { print("[APPLY error]: Wrong expression type"); return (codegen){}; }
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
        s_exp_code *car = fn->car.ptr;
        if (!is_atom(car->car)) { print("[APPLY error] no atomic expression in sub-expression"); return (codegen){}; }
        string_slice s = car_id(car->car);
        if (!s.length) return (codegen){};
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
    else if (is_atom(code->car)){
        string_slice s = car_id(code->car);
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
    } 
    else {
        print("[EVAL error] Unknown expression");
        return (codegen){};
    }
}

codegen s_exp_code_transform(void *ptr, codegen this){
    s_exp_code *code = ptr;
    if (code->car.type == sem_rule_sexp) code->car = eval(code->car, 0);
    if (code->cdr.ptr) codegen_transform(code->cdr, code->cdr);
    return this;
}