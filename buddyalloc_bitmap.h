#pragma once 
#include <unistd.h>

typedef struct{
    char* memory;
    int max_levels;
}buddyalloc;    

void buddy_init(buddyalloc* buddy , char* mem , int levels);
void* get_buddy(buddyalloc* buddy , size_t size);