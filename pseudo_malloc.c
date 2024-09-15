#include "pseudo_malloc.h"

void* pseudo_malloc(size_t size){

    if(size < (size_t)getpagesize()/4 ){
        //allochiamo con buddy
        return ;
    }
    else{
        //allochiamo la memoria con mmap
        return mmap();
    }

}


void pseudo_free(void* p){

    if(1){

    }
    else{

    }
    
}