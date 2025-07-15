#include <stdio.h>
#include <windows.h>
#include "dynamic_heap.h"

#define PAGE_SIZE 4096
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define valloc_size 640000

typedef struct Heap_Chunk{
    void* start;
    size_t size;
    struct Heap_Chunk* prev;
    struct Heap_Chunk* next;
} Heap_Chunk;
typedef struct Chunk_List{
    struct Heap_Chunk* head;
    struct Heap_Chunk* tail;
} Chunk_List; 

Chunk_List heap_alloced = {NULL,NULL};
Chunk_List heap_freed = {NULL,NULL};

void init_heap(size_t size){
    if(size <= sizeof(Heap_Chunk)){
        printf("Invalid size");
        return;
    }
    void *ptr = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(!ptr){
        printf("init heap failed");
        return;
    }
    Heap_Chunk* first = (Heap_Chunk*)ptr;
    first->start = (char*)ptr + sizeof(Heap_Chunk);
    first->size = size - sizeof(Heap_Chunk); 
    first->prev = NULL;
    first->next = NULL;

    heap_freed.head = first;
    heap_freed.tail = first;
}
void insert_chunk(Chunk_List* list,Heap_Chunk* chunk){
    chunk->prev = NULL;
    chunk->next = NULL;

    if (!list->head) {
        list->head = list->tail = chunk;
        return;
    }

    Heap_Chunk* curr = list->head;
    while (curr && curr->start < chunk->start) curr = curr->next;

    if (!curr) {
        chunk->prev = list->tail;
        list->tail->next = chunk;
        list->tail = chunk;
    } else if (!curr->prev) {
        chunk->next = curr;
        curr->prev = chunk;
        list->head = chunk;
    } else {
        chunk->prev = curr->prev;
        chunk->next = curr;
        curr->prev->next = chunk;
        curr->prev = chunk;
    }
}
void remove_chunk(Chunk_List* list,Heap_Chunk* chunk){
    if(chunk->prev) chunk->prev->next = chunk->next;
    else list->head = chunk->next;

    if(chunk->next)chunk->next->prev = chunk->prev;
    else list->tail = chunk->prev;

    chunk->prev = chunk->next = NULL;
}
void merge_freed(){
    Heap_Chunk* curr = heap_freed.head;
    while(curr && curr->next){
        if((char*)curr->start + curr->size == (char*)curr->next){
            Heap_Chunk* next = curr->next;
            curr->size += next->size + sizeof(Heap_Chunk);
            remove_chunk(&heap_freed, next);
        }
        else curr = curr->next;
    }
}
void* heap_alloc(size_t size){
    if(size <= 0) return NULL;
    size = ALIGN(size);
    Heap_Chunk* curr = heap_freed.head;
    while(curr){
        size_t total_size = size + sizeof(Heap_Chunk);
        if(curr->size >= total_size){
            size_t remainder = curr->size - total_size;
            
            Heap_Chunk* alloced = curr;
            alloced->start = (char*)curr + sizeof(Heap_Chunk);
            alloced->size = size;
            
            remove_chunk(&heap_freed, curr);
            insert_chunk(&heap_alloced, alloced);

            if(remainder > sizeof(Heap_Chunk)){
                void* splitptr = (char*)alloced->start + size;
                Heap_Chunk* split_chunk = (Heap_Chunk*)splitptr;
                split_chunk->start = (char*)splitptr + sizeof(Heap_Chunk);
                split_chunk->size = remainder - sizeof(Heap_Chunk);
                split_chunk->prev = split_chunk->next = NULL;
                insert_chunk(&heap_freed,split_chunk);
            }
            return alloced->start;
        }
        curr = curr->next;
    }
    void *ptr = VirtualAlloc(NULL, valloc_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if(!ptr){
        printf("heap expansion failed");
        return NULL;
    }
    Heap_Chunk* region = (Heap_Chunk*)ptr;
    region->start = (char*)ptr + sizeof(Heap_Chunk);
    region->size = valloc_size - sizeof(Heap_Chunk);
    region->prev = NULL;
    region->next = NULL;
    insert_chunk(&heap_freed, region);
    return heap_alloc(size);
}
void heap_free(void* start){
    Heap_Chunk* curr = heap_alloced.head;
    while(curr){
        if(curr->start == start){
            remove_chunk(&heap_alloced, curr);
            insert_chunk(&heap_freed, curr);
            merge_freed();
            return;
        }
        curr = curr->next;
    }
    
}
void delete_heap(){
    Heap_Chunk* curr = heap_alloced.head;
    while(curr){
        Heap_Chunk* next = curr->next;
        remove_chunk(&heap_alloced, curr);
        insert_chunk(&heap_freed,curr);
        curr = next;
    }
    merge_freed();
    curr = heap_freed.head;
    while(curr){
        Heap_Chunk* next = curr->next;
        VirtualFree(curr, 0, MEM_RELEASE);
        curr = next;
    }
    heap_freed.head = NULL;
    heap_freed.tail = NULL;
    heap_alloced.head = NULL;
    heap_alloced.tail = NULL;
}
void dump_freed() {
    printf("=== Freed Chunks ===\n");
    Heap_Chunk* curr = heap_freed.head;
    while (curr) {
        printf("Chunk address: %p\n", curr->start);
        printf("Chunk size: %zu\n", curr->size);
        curr = curr->next;
    }
}

void dump_alloced() {
    printf("=== Allocated Chunks ===\n");
    Heap_Chunk* curr = heap_alloced.head;
    while (curr) {
        printf("Chunk address: %p\n", curr->start);
        printf("Chunk size: %zu\n", curr->size);
        curr = curr->next;
    }
}
