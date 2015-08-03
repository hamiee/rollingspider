#include "define_J.h"
#include <stdio.h>


extern uint32_t cnt;
void PRINT_BUG(uint32_t aa, char * str,...){
    if(cnt%aa==0){
        printf(str);
    }
}

