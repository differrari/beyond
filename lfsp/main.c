#include "syscalls/syscalls.h"
#include "files/system_module.h"
#include "files/helpers.h"

extern bool lfsp_init();
extern system_module language_mod;

int main(int argc, char* argv[]){
    if (!lfsp_init()) {
        print("Failed to init lfsp");
        return -1;
    }
    load_fsmodule(&language_mod, true);
    
    while (true){}
    
    return 0;
}