#include "buddyalloc_bitmap.h"

void buddy_init(buddyalloc* buddy , char* mem , int levels , int bucket_size){
    
    buddy->memory      = mem;
    buddy->max_levels  = levels;
    buddy->bucket_size = bucket_size;

}

//ritorna l'indice del genitore 
int get_parent_idx(int idx){
    return (idx - 1) / 2;
}

int get_level(buddyalloc* buddy , int size){
    
    int bucket_size = buddy->bucket_size;
    int pow = 0;
    while(bucket_size < size){                          //appena la memoria è sufficiente ci fermiamo
        bucket_size = bucket_size << 1;                 //raddoppiamo ogni volta
        pow++;
    }
    return buddy->max_levels - pow - 1;                 //condiderando 0 il livello più grande
}

void* alloc_buddy(buddyalloc* buddy , size_t size){
    /*per allocare dobbiamo 

        - calcolare il livello corrispondente alla memoria richiesta                                     FATTO
        - vedere dalla bitmap se c'è un buddy libero a quel livello      (serve una struct forse)
        - aggiornare i valori sulla bitmap  
        - restituire il buddy oppure un errore se la memoria era insufficente 
    





    (salve prof, questi commenti sono per me, li eliminerò)
    */
    
   //a quale livello corrisponde la memoria che dobbiamo allocare?
   int level = get_level(buddy , size);

   if(level < 0)return -1;



}