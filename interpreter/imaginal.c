#include "imaginal.h"
#include "syscalls/syscalls.h"
#include "ir/manual_gen.h"
#include "ir/arch_transformer.h"

// #define IMAGINAL_DEBUG
#ifdef IMAGINAL_DEBUG
#define imaginal_debug(...) print(__VA_ARGS__)
#else
#define imaginal_debug(...)
#endif

buffer imaginal_buf;

void imaginal_print(codegen exp){
    if (!imaginal_buf.buffer) imaginal_buf = buffer_create(0x1000, buffer_can_grow);
    buffer_wipe(&imaginal_buf);
    codegen_debug_print(exp, exp, 0);
    print("%v",slice_from_buffer(&imaginal_buf));
}

#define nil_exp (codegen){}

bool is_atom(codegen exp){
    return exp.type == sem_rule_lisp_val;
} 

bool is_true(codegen exp){
    switch (exp.type) {
        case sem_rule_sexp: {
            s_exp_code *scode = exp.ptr;
            return is_true(scode->car);
        }
        case sem_rule_lisp_val: {
            lisp_val_code *lcode = exp.ptr;
            return lcode->type == car_true;
        }
        default: return false;
    }
}

static inline codegen is_atom_s_exp(codegen exp){
    return is_atom(exp) ? make_true_atom() : nil_exp;
}

bool is_nil(codegen m){
    return (!m.ptr || !m.type);
}

codegen evlis(codegen m){
    imaginal_debug("[EVLIS trace] evaluating list elements");
    return codegen_transform(m, m);
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
    imaginal_debug("[ADD trace] %i + %i = %i",a,b,a+b);
    // codegen r = s_exp_code_init();
    return make_int_atom(a+b);
}

static inline string_slice car_id(codegen car){
    if (car.type != sem_rule_lisp_val) return (string_slice){};
    lisp_val_code *code = car.ptr;
    if (code->type != car_identifier) return (string_slice){};
    return code->val;
}

bool equality_atom(codegen a, codegen b){
    if (a.type != b.type || a.type != sem_rule_lisp_val) return false;
    lisp_val_code *acode = a.ptr;
    lisp_val_code *bcode = b.ptr;
    if (acode->type != bcode->type) return false;
    switch (acode->type) {
        case car_none:
            return true;
        case car_identifier:
        case car_string:
            return slices_equal(acode->val, bcode->val, true);
        case car_num:
            return acode->number == bcode->number;
        case car_true:
            return true;
      break;
    }
}

static inline codegen equality_atom_exp(codegen a, codegen b){
    return equality_atom(a, b) ? make_true_atom() : nil_exp;
}

static inline codegen equality(codegen args){
    s_exp_code *s = args.ptr;
    codegen a = s->car;
    codegen b = ((s_exp_code*)s->cdr.ptr)->car;
    return equality_atom_exp(a, b);
}

static inline codegen car(codegen a){
    if (a.type != sem_rule_sexp) return nil_exp;
    if (is_atom(a)) return nil_exp;
    s_exp_code *arg = a.ptr;
    return arg->car;
}

static inline codegen cdr(codegen a){
    if (a.type != sem_rule_sexp) return nil_exp;
    if (is_atom(a)) return nil_exp;
    s_exp_code *arg = a.ptr;
    return arg->cdr;
}

codegen apply(codegen fn_exp, codegen a){
    if (!fn_exp.ptr) { print("[APPLY error] Apply null ptr"); return (codegen){}; }
    if (is_atom(fn_exp)){
        string_slice s = car_id(fn_exp);
        imaginal_debug("[APPLY trace] Atomic expression %v",s);
        if (!s.length) { print("[APPLY error]: Wrong expression type"); return (codegen){}; }
        if (slice_lit_match(s, "car", true)){
            return car(a);
        }
        else if (slice_lit_match(s, "cdr", true)){
            return cdr(a);
        }
        else if (slice_lit_match(s, "cons", true)){
            return a;
        }
        else if (slice_lit_match(s, "atom", true)){
            s_exp_code *arg = a.ptr;
            if (!arg) return make_true_atom();
            if (is_atom(a)) return make_true_atom();
            if (arg->cdr.ptr && arg->cdr.type) return nil_exp;
            return is_atom_s_exp(arg->car);
        }
        else if (slice_lit_match(s, "eq", true)){
            return equality(a);
        } else if (slice_lit_match(s, "add", true)){
            return imaginal_builtin_add(a);
        } else {
            return apply(eval(fn_exp),a);
        }
    } else {
        s_exp_code *fn = fn_exp.ptr;
        imaginal_debug("[APPLY trace] Non-atomic expression");
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
            imaginal_debug("[APPLY todo] Handle label case");
            // apply[caddr[fn];x;cons[cons[cadr[fn];caddr[fn]];a]]]
            return (codegen){};
        }
        print("[APPLY error] unknown expression %v in apply",s);
        return (codegen){};
    }
}

// evcon[c;a] = [eval[caar[c];a] → eval[cadar[c];a];
//              T → evcon[cdr[c];a]]

codegen evcon(codegen c){
    if (is_true(eval(car(car(c))))){
        return eval(car(cdr(car(c))));
    }
    codegen tail = cdr(c);
    if (!tail.ptr) return nil_exp;
    return evcon(cdr(c));
}

// eval[e;a] = 
    // [atom[e] → cdr[assoc[e;a]];
    //  atom[car[e]] →
    //          [eq[car[e];QUOTE] → cadr[e];
    //          eq[car[e];COND] → evcon[cdr[e];a];
    //          T → apply[car[e];evlis[cdr[e];a];a]];
    //  T → apply[car[e];evlis[cdr[e];a];a]]

codegen eval(codegen exp){
    s_exp_code *code = exp.ptr;
    if (!code) { print("[EVAL error] null ptr"); return nil_exp; }
    if (is_atom(exp)){
        imaginal_debug("[EVAL todo] handle atomic expression");
        imaginal_print(exp);
        return (codegen){};
    }
    else if (is_atom(code->car)){
        string_slice s = car_id(code->car);
        imaginal_debug("[EVAL trace] Atomic car expression %v",s);
        if (slice_lit_match(s, "cond", true)){
            imaginal_debug("[EVAL todo] Handle cond case");
            return evcon(code->cdr);
        }
        else if (slice_lit_match(s, "quote", true)){
            return car(cdr(exp));
        } else {
            return apply(code->car, evlis(code->cdr));
        }
    } 
    else {
        imaginal_debug("[EVAL trace] non-atomic expression %v");
        return apply(code->car, evlis(code->cdr));
    }
}

codegen s_exp_code_transform(void *ptr, codegen this){
    imaginal_debug("[S_EXP trace] transform");
    if (is_nil(this)) return nil_exp;
    s_exp_code *code = ptr;
    if (code->car.type == sem_rule_sexp) {
        code->car = eval(code->car);
        if (!code->cdr.ptr) return code->car;
    }
    TRANSFORM(cdr);
    return this;
}