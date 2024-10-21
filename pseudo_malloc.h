#pragma once
#include <unistd.h>
#include <sys/mman.h>

#include "buddyalloc_bitmap.h"


#define MAX_LEVELS 6
#define MEMORY_SIZE (1<<20)    //1 megabyte
#define BUCKET_SIZE (MEMORY_SIZE >> (MAX_LEVELS - 1))   
#define BITMAP_SIZE  ((1<<MAX_LEVELS) - 1) / sizeof(char) 
                                //    "(1<<max_levels)-1" is the numer of buddies
                                //    then we divide by the sizeof char    

void pseudo_init();
void* pseudo_malloc(size_t size);
void pseudo_free(void* p);