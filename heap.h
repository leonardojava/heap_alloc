/*
#ifndef HEAP_H_
#define HEAP_H_

#include <stdio.h>
#include <stdbool.h>

#define CAP 640000 //640 kilobytes 
#define CHUNK_CAP 1024 //fuck it why not

typedef struct Heap_Chunk{
    void* start;
    size_t size;
} Heap_Chunk;

typedef struct Chunk_List{
    size_t size;
    Heap_Chunk chunks[CHUNK_CAP];
} Chunk_List; 
//prints out chunks start ptr and size of chunks in list
void chunk_list_dump(const Chunk_List *list);
//allocs size bytes
void *heap_alloc(size_t size);

//frees allocated chunks
void heap_free(void *ptr);

#endif
*/