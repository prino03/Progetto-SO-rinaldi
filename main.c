#include "pseudo_malloc.h"
#include <stdio.h>


void test_alloc_and_free(){
    //printf("Testing allocation\n");
    void* test_ptr;
    int j=0;
    for (int i = 1 ; i<MEMORY_SIZE*2 ; i=i<<1){ 
        printf("allocating block of size %d\n" , i );
        test_ptr = pseudo_malloc(i);
        if(j%4==0)pseudo_free(test_ptr);
        j++;
    }

}
void test_buddy(){

}

int main(int argc, char const *argv[]){
    
    //we start by inizializing our pseudo malloc
    pseudo_init();

    test_buddy();

    test_alloc_and_free(); 


    return 0;
}
