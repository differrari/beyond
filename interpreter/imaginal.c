#include "imaginal.h"
#include "syscalls/syscalls.h"

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
    if (!code) { print("[E] Null ptr"); return (codegen){}; }
    i64 a = 0;
    if (code->car_t.kind) a = parse_int64(code->car_t.start, code->car_t.length);
    //TODO: subexpressions in car
    i64 b = 0;
    s_exp_code *car = code->cdr.ptr;
    if (car){
        if (car->car_t.kind) b = parse_int64(car->car_t.start, car->car_t.length);
    }
    //TODO: subexpression in cdr
    print("Add %i + %i = %i",a,b,a+b);
    // codegen r = s_exp_code_init();
    
}

void apply(codegen exp){
    s_exp_code *code = exp.ptr;
    if (!code) { print("[E] Null ptr"); return; }
    if (code->car_t.kind){
        string_slice s = token_to_slice(code->car_t);
        if (slice_lit_match(s, "add", true)){
            imaginal_builtin_add(code->cdr);
        }
        //Atom
        print("[IE] Atomic expression in car as %v",s);
    } else {
        s_exp_code *car = code->car_s.ptr;
        if (!car){ print("[P] Empty s-exp"); return; }
        if (!car->car_t.kind) { print("[E] no atomic expression in sub-expression"); return; }
        string_slice s = token_to_slice(car->car_t);
        if (slice_lit_match(s, "lambda", true)){
            print("[P] Handle lambda case");
            return;
        }
        if (slice_lit_match(s, "label", true)){
            print("[P] Handle lambda case");
            return;
        }
        print("[E] unknown expression %v in apply",s);
        return;
    }
}

// eval[e;a] = [atom[e] → cdr[assoc[e;a]];
//      atom[car[e]] →
//              [eq[car[e];QUOTE] → cadr[e];
//              eq[car[e];COND] → evcon[cdr[e];a];
//              T → apply[car[e];evlis[cdr[e];a];a]];
//      T → apply[car[e];evlis[cdr[e];a];a]]

void eval(codegen exp){
    
}