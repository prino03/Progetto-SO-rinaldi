#include "pseudo_malloc.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char* separator = "---------------------------------------------------------\n\n";

//test function for both buddyalloc and mmap
void test_alloc_and_free(){
    void* test_ptr;
    int j=0;
    for (int i = 1 ; i<MEMORY_SIZE*2 ; i=i<<1){ 
        printf("allocating block of size %d\n" , i );
        test_ptr = pseudo_malloc(i);
        if(j%4==0)pseudo_free(test_ptr);
        j++;
    }

}

//test function only for buddy alloc, this allocates and frees blocks of random size
//it only frees odd size blocks and stops when we send "end" 
void test_buddy(){
    srand(time(NULL));

    char input[20];
    memset(input,0,strlen(input));
    char* end_cmd = "stop";

    while(memcmp(input , end_cmd , strlen(end_cmd))){

        int randint = rand() % (getpagesize()/4);   

        void* test_ptr= pseudo_malloc(randint);
        if(randint%2 == 1)pseudo_free(test_ptr);

        printf("send stop to terminate the allocation or anything else to continue\n");
        scanf("%s" , input);
        printf("\n");
    }
}

//this function allocates blocks with buddyalloc and free them in opposite order
void test_buddy_free(){
    int size = getpagesize()/6;
    int n = 10;
    int start_pos = 4;
    void* ptr_array[n];

    for (int i = 0 ; i < n ; i++){
        ptr_array[i]=pseudo_malloc(size);
    }

    for (int i = start_pos ; i< n+start_pos ; i++){
        pseudo_free(ptr_array[i%n]);
    }
}

int main(int argc, char const *argv[]){
    
    //we start by inizializing our pseudo malloc
    pseudo_init();

    printf("%s",separator);
    test_buddy_free();

    printf("%s",separator);
    test_buddy();

    printf("%s",separator);
    test_alloc_and_free();

    printf("%s",separator);

    return 0;
}
