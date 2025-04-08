# Me Malloc
I mostly followed the steps in [this video](https://www.youtube.com/watch?v=sZ8GJ1TiMdk&t=1489s&ab_channel=TsodingDaily) by tsoding for the heap.c implementation, it's just a static-sized byte array that you can borrow memory from. 
For dynamic_heap.c I use virtual alloc in the same way other people use mmap to take a bunch of memory from the OS that I manage myself borrowing a lot of logic from previous heap.c.
## Description
Free list memory allocator that dynamically sizes up using Virtual Alloc I compiled it in GCC and didn't test it lol.
