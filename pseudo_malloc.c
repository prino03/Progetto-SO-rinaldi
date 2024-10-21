#include "pseudo_malloc.h"
#include "buddyalloc_bitmap.h"
#include <math.h>

//global variables 
buddyalloc buddy;
bitmap map;
char* mem[MEMORY_SIZE];
char* map_buf[BITMAP_SIZE]; 

//inizialization of buddy and bitmap
void pseudo_init(){
    int required_bitmap_bits = (1 << (MAX_LEVELS + 1)) - 1;
    bitmap_init(&map , required_bitmap_bits , map_buf);

    buddy_init(&buddy , mem , MAX_LEVELS ,MEMORY_SIZE, BUCKET_SIZE , &map);
}

//our pseudo malloc allocates differently based on the size of the block requested
void* pseudo_malloc(size_t size){

    if(size + sizeof(int) < (size_t)getpagesize()/4 ){
        return alloc_buddy(&buddy , size);
    }
    else{
        void* ret = mmap(NULL , size +sizeof(int), PROT_READ | PROT_WRITE , MAP_PRIVATE , 0 , 0);
        int* ret_size=(int*)ret;
        *ret_size = size;
        return ret + sizeof(int);  //da controllare 
    }
}

//we free previously allocated memory
void pseudo_free(void* p){

    size_t p_size = *((int*)p);  //da controllare
    if (p >= mem && p< mem+(MEMORY_SIZE*sizeof(char))  ){   
        free_buddy(&buddy , p);
    }
    else{
        munmap(p , p_size);
    }
}