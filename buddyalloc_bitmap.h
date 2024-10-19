#pragma once 
#include <unistd.h>



//buddyalloc struct e sue funzioni
typedef struct{
    char* memory;
    int max_levels;
    int bucket_size;
    bitmap* bitmap;
}buddyalloc;    

void buddy_init(buddyalloc* buddy , char* mem , int levels , int bucket_size , bitmap* map);
void* alloc_buddy(buddyalloc* buddy , size_t size);
int get_buddy_idx(int idx);
int get_parent_idx(int idx);
int get_level(buddyalloc* buddy , int size);



//bitmap e sue funzioni
typedef struct  {
    char* map;
    int nbits;
}bitmap;

void bitmap_init(bitmap* mappa , int req_bitmap_bits , char* buf_map);
int bitmap_ret_bit_value(bitmap* map , int idx);
void bitmap_set_bit(bitmap* map , int idx , int value);
int bitmap_get_free_buddy_idx(bitmap* buddy_map , int level);
void bitmap_occupy_block(bitmap* buddy_map ,int idx);


