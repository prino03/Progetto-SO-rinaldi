#include "pseudo_malloc.h"

//variabili globali
buddyalloc buddy;
bitmap map;
char* mem[MEMORY_SIZE];
char* map_buf[10]; // cambia il 10, non so la dimensione 

//inizializziamo buddyalloc e mappa 
void pseudo_init(){
    int required_bitmap_bits = (1 << (MAX_LEVELS + 1)) - 1;
    bitmap_init(&map , required_bitmap_bits , map_buf);

    buddy_init(&buddy , mem , MAX_LEVELS ,MEMORY_SIZE, BUCKET_SIZE , &map);
}

//questa funzione alloca in base a quanta memoria viene richiesta
void* pseudo_malloc(size_t size){

    if(size < (size_t)getpagesize()/4 ){
        //allochiamo con buddy
        return alloc_buddy(&buddy , size);
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