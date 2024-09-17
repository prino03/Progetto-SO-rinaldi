#pragma once 
#include <unistd.h>

typedef struct{
    char* memory;
    int max_levels;
    int bucket_size;
}buddyalloc;    

void buddy_init(buddyalloc* buddy , char* mem , int levels , int bucket_size);
void* alloc_buddy(buddyalloc* buddy , size_t size);
int get_buddy_idx(int idx);
int get_parent_idx(int idx);
int get_level(buddyalloc* buddy , int size);
