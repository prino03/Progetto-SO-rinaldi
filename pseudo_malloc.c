#include "pseudo_malloc.h" 
#include "buddyalloc_bitmap.h"
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>

//global variables 
buddyalloc buddy;
bitmap map;
char mem[MEMORY_SIZE];
char map_buf[BITMAP_SIZE]; 

//inizialization of buddy and bitmap
void pseudo_init(){
    int required_bitmap_bits = (1 << (MAX_LEVELS )) - 1;
    bitmap_init(&map , required_bitmap_bits , map_buf);

    buddy_init(&buddy , mem , MAX_LEVELS ,MEMORY_SIZE, BUCKET_SIZE , &map);
}

//our pseudo malloc allocates differently based on the size of the block requested
void* pseudo_malloc(size_t size){

    if(size + sizeof(int) < (size_t)getpagesize()/4 ){
        printf("allocating block with buddyallocator\n");
        return alloc_buddy(&buddy , size);
    }
    else{
        printf("allocating block with mmap\n");
        void* ret = mmap(NULL , size +sizeof(int), PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if(ret == MAP_FAILED ){
            printf("Error: allocation with mmap failed\n");
            return NULL;
        }

        int* ret_size=(int*)ret;
        *ret_size = size+sizeof(int);
        printf("allocation with mmap completed returning pointer\n");
        return ret + sizeof(int);  
    }
}

//we free previously allocated memory
void pseudo_free(void* p){
    if(p==NULL){
        printf("Error: cannot free a NULL pointer\n");
        return;
    }

    int* ptr = p-sizeof(int);
    size_t p_size = *((int*)ptr);   
    void* v_mem = (void*)mem;

    if (p >= v_mem && p< v_mem+(MEMORY_SIZE*sizeof(char))  ){   
        printf("freeing the block with buddyalloc\n");
        free_buddy(&buddy , p);
    }
    else{
        printf("freeing the block with munmap\n");
        int ret = munmap(p-sizeof(int), p_size);
        if(ret<0)printf("Error: munmap failed\n");
        else printf("block freed\n");
    }
}