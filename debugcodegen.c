#include "codegen.h"
#include "rules.h"

bool is_dec = false;

#ifdef DEBUG_CODEGEN
//TODO: Needs updating with the new function signatures
bool begin_rule(int type){
    is_dec = false;
    switch (type){
        case sem_dec: is_dec = true; print("DECLARATION"); break;
        case sem_assign: print("ASSIGNMENT"); break;
        case sem_exp: print("EXPRESSION"); break;
        case sem_call: print("FUNCCALL"); break;
        case sem_args: print("ARGUMENT"); break;
        case sem_cond: print("CONDITIONAL"); break;
        case sem_jmp: print("JUMP"); break;
        case sem_label: is_dec = true; print("LABEL"); break;
        default: return false;
    }
    return true;
}

void register_elem(int type, Token elem){
    print("%s: %v",is_dec ? get_sem_elem_name(type) : sem_rule_name(type), token_to_slice(elem));
}

void end_rule(int type){
    
}

void register_subrule(int type, void *child){
    
}

char* emit_code(){
    return "";
}

#endif