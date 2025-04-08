#ifndef DYNAMIC_HEAP_H_
#define DYNAMIC_HEAP_H_

#include <stdio.h>

extern void init_heap(size_t size);//initialize heap/freed list

extern void* heap_alloc(size_t size); //alloc size

extern void heap_free(void* start); //free alloced

extern void delete_heap(); //deletes heap

extern void dump_freed(); //prints out freed chunks

extern void dump_alloced();//prints out alloced chunks
#endif