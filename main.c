#include "pseudo_malloc.h"
#include <stdio.h>


void test_alloc_and_free(){
    printf("Testing allocation\n");
    void* test_ptr;
    for (int i = 1 ; i<MEMORY_SIZE*2 ; i=i<<1){ 
        printf("allocating block of size %d\n" , i );
        test_ptr = pseudo_malloc(i);
        if(test_ptr!=NULL)printf("block allocated\nNow we free it\n");
        pseudo_free(test_ptr);
        printf("block freed\n");
    }

}


int main(int argc, char const *argv[]){
    
    //we start by inizializing our pseudo malloc
    pseudo_init();


    test_alloc_and_free(); 


    return 0;
}
