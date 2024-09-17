#include "pseudo_malloc.h"

//variabili globali
buddyalloc buddy;
char* mem[MEMORY_SIZE];

void pseudo_init(){
    buddy_init(&buddy , mem , MAX_LEVELS , BUCKET_SIZE);
}

//questa funzione alloca in base a quanta memoria viene richiesta
void* pseudo_malloc(size_t size){

    if(size < (size_t)getpagesize()/4 ){
        //allochiamo con buddy
        return get_buddy(&buddy , size);
    }
    else{
        //allochiamo la memoria con mmap    
        void* ret = mmap(NULL , size +sizeof(int), PROT_READ | PROT_WRITE , MAP_PRIVATE , 0 , 0);
        int* ret_size=(int*)ret;
        *ret_size = size;
        return ret;
    }

}

//libera la memoria allocata in precedenza, secondo lo stesso principio di prima
void pseudo_free(void* p){

    size_t p_size = *((int*)p);
    if( (p_size - sizeof(int)) < (size_t)getpagesize()/4 ){
        //liberiamo tramite il buddy allocator
    }
    else{
        //liberiamo la memoria acon la munmap
        munmap(p , p_size);
    }
}