#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mymalloc.h"

//DEFINITIONS
//ROUNDUP8 rounds up the inserted integer to the next highest multiple of 8
#define ROUNDUP8(x) (((x) + 7) & (-8))
//LASTPAYLOAD converts the current pointer to the correct data type
#define LASTPAYLOAD(x) ((int16_t*)x)
//CHUNKISFREE moves the current pointer two bytes forward to whether the chunk is free
#define CHUNKISFREE(x) (((int16_t*)x) + 1)
//PAYLOAD moves the current pointer four bytes forward where the payload size is
#define PAYLOAD(x) (((int16_t*)x) + 2)
#define MEMLENGTH 512

static double memory[MEMLENGTH];

void *MyMalloc(size_t noOfBytes, char *file, int line) {
    struct block *curr, *prev;
    void *result;

    // Check if the free list is empty
    if (freeList->size == 0) {
        initialize();
        printf("Memory initialized\n");
    }

    curr = freeList;

    while (((curr->size < noOfBytes) || (curr->free == 0)) && (curr->next != NULL)) {
        // Iterate through the free list to find suitable block
        prev = curr;
        curr = curr->next;
        printf("One block checked\n");
    }

    if (curr->size == noOfBytes) {
        // Find an exact fitting block
        curr->free = 0;
        result = (void *)(++curr);
        printf("Exact fitting block allocated\n");
        return result;
    } else if (curr->size > (noOfBytes + sizeof(struct block))) {
        // If finds block with more space it splits
        split(curr, noOfBytes);
        result = (void *)(++curr);
        printf("Fitting block allocated with a split\n");
        return result;
    } else {
        result = NULL;
        printf("Sorry. Not enough memory to allocate\n");
        return result;
    }
}


void merge() {
    struct block *curr, *prev;
    curr = freeList;

    while (curr->next != NULL) {
        if (curr->free && curr->next->free) {
            // Merge adjacent free blocks in the free list.
            curr->size += (curr->next->size) + sizeof(struct block);
            curr->next = curr->next->next;
        }
        prev = curr;
        curr = curr->next;
    }
}

void MyFree(void* ptr, char *file) {
    if ((void*)memory <= ptr && ptr <= (void*)(memory + 20000)) {
        // Verify that the provided pointer is within the allocated memory range.
        struct block* curr = ptr;
        --curr;
        curr->free = 1;
        merge(); // After freeing memory, merge adjacent free blocks.
    } else
printf("Error invalid pointer\n");
}

