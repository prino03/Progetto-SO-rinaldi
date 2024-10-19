#pragma once
#include <unistd.h>
#include <sys/mman.h>
#include "buddyalloc_bitmap.h"

#define MAX_LEVELS 6
#define MEMORY_SIZE (1<<20)    //sarebbe un megabyte, 2^20 = 1 shiftato 20 volte
#define BUCKET_SIZE (MEMORY_SIZE >> (MAX_LEVELS - 1))    //ricontrollare il "-1"

void pseudo_init();

void* pseudo_malloc(size_t size);

void pseudo_free(void* p);