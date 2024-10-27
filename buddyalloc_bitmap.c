#include "buddyalloc_bitmap.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

void buddy_init(buddyalloc* buddy , char* mem , int levels ,int mem_size, int bucket_size, bitmap* map){
    
    buddy->memory      = mem;
    buddy->max_levels  = levels;
    buddy->bucket_size = bucket_size;
    buddy->mem_size = mem_size;
    buddy->bitmap = map; 

}

void bitmap_init(bitmap* mappa , int req_bitmap_bits , char* buf_map){
    memset(buf_map, 0, req_bitmap_bits);
    mappa->map = buf_map;
    mappa->nbits = req_bitmap_bits;
}

//returns index of the parent
int get_parent_idx(int idx){
    if(idx==0)return -1;

    return (idx - 1) / 2;
}

//return correspondive level of idx
int get_idx_level(int idx){
    int level = floor(log2(idx+1));
    return level;
}

//returns idx of buddy
int get_buddy_idx(int idx){
    if (idx==0){
        return 0;
    }
    if (idx%2 == 1){
        return idx + 1;
    }
    return idx - 1;
}

//returns left child idx
int get_left_children_idx(int idx){
    return idx*2+1;
}

//returns right child idx
int get_right_children_idx(int idx){
    return idx*2+2;
}

//calculates the correspondive level for a "size" size mem
int get_level(buddyalloc* buddy , int size){
    
    int bucket_size = buddy->bucket_size;
    int pow = 0;
    while(bucket_size < size){                          //when memory is enough we stop
        bucket_size = bucket_size << 1;                 //we double every time
        pow++;
    }
    return buddy->max_levels - pow - 1;                 //0 is the biggest level
}

//returns value of idx bit in bitmap
int bitmap_ret_bit_value(bitmap* buddy_map, int idx){
    int byte_map_idx = idx/8;
    int rest = idx%8;
    int mask = 1<<rest;
    mask = mask & buddy_map->map[byte_map_idx];
    int result = mask>>rest;
    return result;
}

//updates value of bit
void bitmap_set_bit(bitmap* buddy_map , int idx, int value){
    //we have to calculate the idx of the map array
    int byte_map_idx = idx/8;
    int rest = idx%8;
    int mask = 1<<rest;

    if(value){
        buddy_map->map[byte_map_idx] |= mask; //we have to save all the other values 
    }                                         //we create a mask for this purpose
    else{
        buddy_map->map[byte_map_idx] &= ~mask;
    }
}

//prints the bitmap
void bitmap_printf(bitmap* map){
    for (int i = 0 ; i < map->nbits ; i++ ){
        printf("%d ", bitmap_ret_bit_value(map , i));
        if( get_idx_level(i) != get_idx_level(i+1)  )printf("\n");
    }
}

//return idx of free buddy in level
int bitmap_get_free_buddy_idx(bitmap* buddy_map , int level){

    int first_level_idx = pow(2,level) - 1;    //first element idx of the level 
    int last_level_idx = pow(2,(level+1)) -2;  //last element idx of the level

    for (int i = first_level_idx ; i<=last_level_idx ; i++){
        if (!bitmap_ret_bit_value(buddy_map , i)){
            return i;
        }
    }
    return -1;
}

//changes value of our block and recursively of every parent
void bitmap_occupy_block(bitmap* buddy_map ,int idx){
    bitmap_set_bit( buddy_map , idx , 1 );
    
    int parent = get_parent_idx(idx);
    if(parent == -1)return;

    bitmap_occupy_block(buddy_map , parent);
}

//changes value of parent if both buddys are free
void bitmap_free_parent(bitmap* map , int idx){

    if(idx==0){
        bitmap_set_bit(map , idx , 0);
        return;
    }
    bitmap_set_bit(map , idx , 0);
    
    if (  !( bitmap_ret_bit_value(map , get_buddy_idx(idx))) ){      
        bitmap_free_parent(map , get_parent_idx(idx));
    }    
    return;
}

//changes value of all childs recursively
void bitmap_change_children(bitmap* buddy_map ,int parent_idx , int value){
    if (parent_idx > buddy_map->nbits)return;
    
    bitmap_set_bit(buddy_map , parent_idx , value);
    bitmap_change_children(buddy_map , get_right_children_idx(parent_idx) , value );
    bitmap_change_children(buddy_map , get_left_children_idx(parent_idx) , value );

}

//allocates a block 
void* alloc_buddy(buddyalloc* buddy , size_t size){

   if(size<=0){
    printf("cant allocate 0 or less bytes\n");
    return NULL;
   }
    
    size+=sizeof(int); //we need some space to store the idx of the block


   //we calculate the level of the buddy
   int level = get_level(buddy , size);
   if(level < 0)return NULL; //error 
   if(level>buddy->max_levels)level=buddy->max_levels;   
    printf("allocating %ld bytes at level %d\n", size, level);


   //check if there is an available buddy 
    bitmap* buddy_map = buddy->bitmap;

    int idx = bitmap_get_free_buddy_idx(buddy_map , level);
    if(idx<0){
        printf("no free block available\n");
        return NULL;
    }
    printf("we found a block of index %d, now we update the bitmap\n" , idx);
    //now we operate on the bitmap to signal the block has been occupied
    //we have to change the value of the block and of its parents
    bitmap_occupy_block(buddy->bitmap , idx);
    //we also have to change the value of all its children
    bitmap_change_children(buddy->bitmap , idx , 1 );

    printf("bitmap updated\n");

    int block_size = buddy->mem_size >> level;

    int to_sum = ( idx-(pow(2,get_idx_level(idx)) - 1) )*block_size;   
    char* to_return = buddy->memory + to_sum;     
    ((int*)to_return)[0] = idx;    
    
    bitmap_printf(buddy->bitmap);

    return (void*) (to_return+sizeof(int));
}

//frees previously allocated block
void free_buddy(buddyalloc* buddy , void* to_free){
    if(!to_free){
        printf("empty address\n");
        return;
    }

    void* pt=to_free-sizeof(int);
    char* ptr = (char*)pt;
    int block_idx = ((int*)ptr)[0];

    bitmap_change_children( buddy->bitmap , block_idx , 0 ); //we free the childrens 

    //now we have to free the parent, but only if our buddy is free
    bitmap_free_parent(buddy->bitmap , block_idx);
    
    //bitmap_printf(buddy->bitmap);
} 