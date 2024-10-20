#include "buddyalloc_bitmap.h"
#include <math.h>

void buddy_init(buddyalloc* buddy , char* mem , int levels ,int mem_size, int bucket_size, bitmap* map){
    
    buddy->memory      = mem;
    buddy->max_levels  = levels;
    buddy->bucket_size = bucket_size;
    buddy->mem_size = mem_size;
    buddy->bitmap = map;

}

void bitmap_init(bitmap* mappa , int req_bitmap_bits , char* buf_map){
    mappa->map = buf_map;
    mappa->nbits = req_bitmap_bits;
}

//ritorna l'indice del genitore 
int get_parent_idx(int idx){
    if(idx==0)return -1;

    return (idx - 1) / 2;
}

int get_idx_level(int idx){
    int level = floor(log2(idx+1));
    return level;
}

int get_buddy_idx(int idx){
    if (idx==0){
        return 0;
    }
    if (idx%2 == 1){
        return idx + 1;
    }
    return idx - 1;
}

int get_left_children_idx(int idx){
    return idx*2+1;
}

int get_right_children_idx(int idx){
    return idx*2+2;
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

int bitmap_ret_bit_value(bitmap* buddy_map, int idx){
    int byte_map_idx = idx/8;
    int rest = idx%8;
    int mask = 1<<rest;
    mask = mask & buddy_map->map[byte_map_idx];
    int result = mask>>rest;

    return result;
}

void bitmap_set_bit(bitmap* buddy_map , int idx, int value){
    //we have to calculate the idx of the map array
    int byte_map_idx = idx/8;
    int rest = idx%8;
    int mask = 1<<rest;

    if(value){
        buddy_map->map[byte_map_idx] |= mask; //we have to save all the other values 
    }                                         //we create a mask for this purpose
    else{
        buddy_map->map[byte_map_idx] &= !mask;
    }
}

int bitmap_get_free_buddy_idx(bitmap* buddy_map , int level){
    
    int first_level_idx = (2^level) - 1;    //first element idx of the level 
    int last_level_idx = (2^(level+1)) -2;  //last element idx of the level
    for (int i = first_level_idx ; i<=last_level_idx ; i++){
        if (!bitmap_ret_bit_value(buddy_map , i)){
            return i;
        }
    }
    return -1;
}

void bitmap_occupy_block(bitmap* buddy_map ,int idx){
    bitmap_set_bit( buddy_map , idx , 1 );
    
    int parent = get_parent_idx(idx);
    if(parent == -1)return;

    bitmap_occupy_block(buddy_map , parent);
}

void bitmap_occupy_children(bitmap* buddy_map ,int parent_idx){
    if (parent_idx > buddy_map->nbits)return;
    
    bitmap_set_bit(buddy_map , parent_idx , 1);
    bitmap_occupy_children(buddy_map , get_right_children_idx(parent_idx));
    bitmap_occupy_children(buddy_map , get_left_children_idx(parent_idx));

}

void* alloc_buddy(buddyalloc* buddy , size_t size){
    /*per allocare dobbiamo 
    
       - aggiornare i valori sulla bitmap      FATTO
        - restituire il buddy oppure un errore se la memoria era insufficente 

    (salve prof, questi commenti sono per me, li eliminerò)
    */
    
   if(size<=0){
    printf("cant allocate 0 or less bytes");
    return NULL;
   }



   //a quale livello corrisponde la memoria che dobbiamo allocare?
   int level = get_level(buddy , size);
   if(level < 0)return -1; //error 
   if(level>buddy->max_levels)level=buddy->max_levels;   

   //verifichiamo se un buddy a quel livello è disponibile
    bitmap* buddy_map = buddy->bitmap;
   
    int idx = bitmap_get_free_buddy_idx(buddy_map , level);
    if(idx<0){
        printf("no free block available");
        return -1;
    }

    //now we operate on the bitmap to signal the block has been occupied
    //we have to change the value of the block and of its parents
    bitmap_occupy_block(buddy_map , idx);
    //we also have to change the value of all its children
    bitmap_occupy_children(buddy_map , idx);

    int block_size = buddy->mem_size >> level;

    char* to_return = buddy->memory + (idx- ( (2^get_idx_level(idx)) - 1) )*block_size; 
    return (void*) to_return;
}

void* free_buddy(void* to_free){



} 