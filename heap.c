/*
#include <stdio.h>
#include <stdbool.h>
#include "./heap.h"
#define CAP 640000 //640 kilobytes 
#define CHUNK_CAP 1024 //fuck it why not
char heap[CAP] = {0};

typedef struct Heap_Chunk{
    void* start;
    size_t size;
} Heap_Chunk;
typedef struct Chunk_List{
    size_t size;
    Heap_Chunk chunks[CHUNK_CAP];
} Chunk_List; 
Chunk_List heap_alloced = {0}; //alloced chunks
Chunk_List heap_freed = {
    .size = 1,
    .chunks = {[0] = {.start = heap, .size = sizeof(heap)}}

}; //initially say entire heap is freed and is 1 big ol chunk
Chunk_List tmp = {0};

int chunk_find(const Chunk_List *list, void *ptr){
    for(int i = 0; i < list->size; i++){
        if(list->chunks[i].start == ptr) return i;
    }
    return -1;
}

void chunk_insert(Chunk_List *list, void *ptr, size_t size){
    if(list->size >= CHUNK_CAP){
        printf("Chunk List full insertion failed");
        return;
    }
    list->chunks[list->size].start = ptr;
    list->chunks[list->size].size = size;
    //insertion sort the chunk
    int i = list->size;
    while(i > 0 && list->chunks[i - 1].start > list->chunks[i].start){
        const Heap_Chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i-1] = tmp;
        i--;
    }
    list->size++;
}
//literally just move all the chunks right of index to the left lol
void chunk_remove(Chunk_List *list, size_t index){
    if(index >= list->size){
        printf("index out of bounds chunk remove failed");
        return;
    }
    for(int i = index; i < list->size - 1; i++){
        list->chunks[i] = list->chunks[i + 1];
    }
    list->size--;
}
void chunk_list_dump(const Chunk_List *list){ //print all our chunks
    printf("Chunks (%zu):\n", list->size);
    for(size_t i = 0; i < list->size; i++){
        printf(" start: %p, size: %zu\n",
        list->chunks[i].start,
        list->chunks[i].size
        );
    }
}
//TODO: implement merge lol
//so pointers are sorted so I guess we could go for each and check if any chunks are contigious 
void merge_freed(Chunk_List *tmp, Chunk_List *src){
    tmp-> size = 0;
    for(size_t i = 0; i < src->size; i++){
        const Heap_Chunk chunk = src->chunks[i];
        if(tmp-> size > 0 && tmp->chunks[tmp->size - 1].start + tmp->chunks[tmp->size - 1].size == chunk.start){
            tmp->chunks[tmp->size - 1].size += chunk.size;
        }
        else{
            tmp->chunks[tmp->size] = chunk;
            tmp->size++;
        }
    }
}

//merge all the free chunks then search within freed for a big enough chunk
void *heap_alloc(size_t size){
    
    if(size <= 0) return NULL;
    merge_freed(&tmp, &heap_freed);
    heap_freed = tmp;
    for(size_t i = 0; i < heap_freed.size; i++){
        if(heap_freed.chunks[i].size >= size){
            const Heap_Chunk chunk = heap_freed.chunks[i];
            chunk_remove(&heap_freed, i);
            const size_t remainder = chunk.size - size;
            chunk_insert(&heap_alloced, chunk.start, size);
            if(remainder > 0) chunk_insert(&heap_freed, chunk.start + size, remainder);
            return chunk.start;
        }
    }
    return NULL;
}

//basically just find chunk insert it into freed list then remove it from alloced list
void heap_free(void *ptr){
    const int index = chunk_find(&heap_alloced, ptr);
    assert(index >= 0);
    chunk_insert(&heap_freed, heap_alloced.chunks[index].start, heap_alloced.chunks[index].size);
    chunk_remove(&heap_alloced, (size_t)index);
}
*/