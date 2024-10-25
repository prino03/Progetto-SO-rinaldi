#pragma once 
#include <unistd.h>

//bitmap struct
typedef struct  {
    char* map;
    int nbits;
}bitmap;  

//buddyalloc struct and its functions 
typedef struct{
    char* memory;
    int max_levels;
    int bucket_size;
    int mem_size;
    bitmap* bitmap;
}buddyalloc;    

void buddy_init(buddyalloc* buddy , char* mem , int levels ,int mem_size, int bucket_size , bitmap* map);
void* alloc_buddy(buddyalloc* buddy , size_t size);
void free_buddy(buddyalloc* buddy, void* to_free);
int get_buddy_idx(int idx);
int get_parent_idx(int idx);
int get_level(buddyalloc* buddy , int size);
int get_left_children_idx(int idx);
int get_right_children_idx(int idx);



//bitmap functions
void bitmap_init(bitmap* mappa , int req_bitmap_bits , char* buf_map);
int bitmap_ret_bit_value(bitmap* map , int idx);
void bitmap_set_bit(bitmap* map , int idx , int value);
int bitmap_get_free_buddy_idx(bitmap* buddy_map , int level);
void bitmap_occupy_block(bitmap* buddy_map ,int idx);
void bitmap_change_children(bitmap* buddy_map ,int parent_idx , int value);
void bitmap_free_parent(bitmap* map , int idx);

