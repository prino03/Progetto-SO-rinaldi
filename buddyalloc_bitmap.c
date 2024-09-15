#include "buddyalloc_bitmap.h"

void buddy_init(buddyalloc* buddy , char* mem , int levels){
    
    buddy->memory     = mem;
    buddy->max_levels = levels;
}

void* get_buddy(buddyalloc* buddy , size_t size){

    
}