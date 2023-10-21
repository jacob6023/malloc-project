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
//CHUNKNOTFREE moves the current pointer two bytes forward to whether the chunk is free
#define CHUNKNOTFREE(x) (((int16_t*)x) + 1)
//PAYLOAD moves the current pointer four bytes forward where the payload size is
#define PAYLOAD(x) (((int16_t*)x) + 2)
#define MEMLENGTH 512

//Uncomment the below line for debug prints
//#define DEBUG

static double memory[MEMLENGTH];

void* mymalloc(size_t size, char *file, int line) {
    char* outOfBounds = (((char*)memory) + (MEMLENGTH * 8));

    //Checks to make sure a positive integer is entered
    if (size <= 0) {
        printf("%s: %d: Error: Cannot Allocate 0 Bytes or Less.", file, line);
        return NULL;
    }
    //Checks to see if initial size is out of bounds
    if ((size) > ((512 * 8) - 8)) {
        printf("%s: %d: Error: Out of Memory During Initialization.", file, line);
        return NULL;
    }
    //Rounds the inserted size
    //printf("Size: %d   ", size);
    if (size % 8 != 0) size = ROUNDUP8(size);

    //pack is what is returned
    char* pack = NULL;
    //start is the start of the header and moves to the payload when needed
    char* start = (char*)memory;
    //increases as I move through memory to make sure I stay in the bounds of memory
    //int count = 1;

    //printf("Value of size: %d\n", size);

    //Checks if there is no allocated memory yet. Then initializes 
    if (*PAYLOAD(start) == 0)
    {
        #ifdef DEBUG
            printf("INIT\n");
        #endif

        //Establishes the first header
        *CHUNKNOTFREE(start) = 1;
        *PAYLOAD(start) = (int16_t)size;

        #ifdef DEBUG
            printf("First chunk size: %d\n", *PAYLOAD(start));
        #endif

        //Assigns the returned payload and moves the start and inserter to start of 
        //header 2
        pack = start + 8;

        //Checks to see if there is room for next header
        if ((size) != ((512 * 8) - 8)) {
            start = start + (8 + size);

            //Initializes right most header with size of previous chunk
            *LASTPAYLOAD(start) = (int16_t)size;

            #ifdef DEBUG
                printf("Value inserted: %d\n", *inserter);
            #endif

            *CHUNKNOTFREE(start) = 0;
            *PAYLOAD(start) = (int16_t)((MEMLENGTH * 8) - 16 - size);
            //printf("Leftover space: %d  ", *PAYLOAD(start));
            #ifdef DEBUG
                printf("Leftover space: %d\n\n", *PAYLOAD(start));
            #endif
        }

        return pack;
    }
//((char*)start) + size + 8 < outOfBounds
    //Loop to check for open memory until memory is found or we go out of memory.
    
    while(((char*)start) < outOfBounds) {
        #ifdef DEBUG
            printf("Current Chunk Space: %d\n", *PAYLOAD(start));
        #endif

        //Case where there is a header at the end of memory
        /*if (*PAYLOAD(start))
        {
            printf("%s: %d: Error: Out of Memory during header creation.", file, line);
            return NULL;
        }*/
        //Space can be allocated
        if (*CHUNKNOTFREE(start) == 0 && *PAYLOAD(start) >= (size)) 
        {
            //Stores previous size and then stores new size and that it is allocated
            int16_t tempSize = *PAYLOAD(start);
            *CHUNKNOTFREE(start) = 1;
            *PAYLOAD(start) = (int16_t)size;

            #ifdef DEBUG
                printf("Chunk size: %d\n", *PAYLOAD(start));
            #endif

            pack = start + 8;

            #ifdef DEBUG
                printf("COUNT AND SIZE: %d\n", count + size);
                printf("CHECKING <-> %d\n", count + ((int)size));
            #endif
            char* tempStart = start;
            start = start + 8 + ((int)size);
            //Checks to see if a header would fit
            if ((((char*)tempStart) + size + 8 < outOfBounds) && *CHUNKNOTFREE(start) == 0) {

                *LASTPAYLOAD(start) = (int16_t)size;
                *CHUNKNOTFREE(start) = 0;
                *PAYLOAD(start) = (int16_t)(tempSize - 8 - ((int16_t)size));
                //printf("Leftover space: %d  ", *PAYLOAD(start));
                #ifdef DEBUG
                    printf("Leftover space: %d\n\n", *PAYLOAD(start));
                #endif
            }

            return pack;
        }
        //Moves to the next chunk if current chunk is not big enough or allocated
        if (*CHUNKNOTFREE(start) == 1 || *PAYLOAD(start) < (size + 8))
        {
            #ifdef DEBUG
                printf("Count1 -> %d\n", count);
            #endif

            //count = count + ((int)*PAYLOAD(start)) + 8;
            start = start + *PAYLOAD(start) + 8;

            #ifdef DEBUG
                printf("Count2 -> %d\n", count);
            #endif
        }
        
    }
    printf("%s: %d: Error: Out of Memory when traversing.\n", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    // Checks if ptr is outside the valid memory range
    if (ptr < (void*)memory || ptr >= (void*)(memory + MEMLENGTH)) {
        printf("%s: %d: Error: Attempt to free data invalid\n", file, line);
        return;
    }
    // Convert memory and ptr
    //int16_t* memoryStart = (int16_t*)memory;

    // FAssigns metadata to beginning of memory
    char* metadata = (char*)memory;
    //int16_t* chunk = (int16_t*)(metadata + 8);
    char* prevdata = metadata;
    char* updater;

    char* outOfBounds = (((char*)memory) + (MEMLENGTH * 8));
    // Loops until we reach the start of the memory
    while (!(metadata > outOfBounds)) {
        //checks if we passed the ptr
        if(metadata > ((char*)ptr)) {
            //printf("CHUNK FOUND\n");
            if (*CHUNKNOTFREE(prevdata) == 1) {
                *CHUNKNOTFREE(prevdata) = 0;
                //printf("PREV DATA BEFORE %d\n", *PAYLOAD(prevdata));
                if (*CHUNKNOTFREE(metadata) == 0) {
                    //printf("METADATA1 %d\n", *PAYLOAD(metadata));
                    *PAYLOAD(prevdata) = *PAYLOAD(prevdata) + 8 + *PAYLOAD(metadata);
                    //printf("Do right %d\n", *PAYLOAD(prevdata));
                }
                
                metadata = prevdata;
                //printf("LAST PAYLOAD: %d\n", *LASTPAYLOAD(prevdata));
                prevdata = prevdata - 8 - *LASTPAYLOAD(prevdata);
                

                if (*CHUNKNOTFREE(prevdata) == 0 && *LASTPAYLOAD(metadata) != 0) {
                    //printf("METADATA2 %d\n", *PAYLOAD(metadata));
                    *PAYLOAD(prevdata) = *PAYLOAD(prevdata) + 8 + *PAYLOAD(metadata);
                    //printf("Do left %d\n", *PAYLOAD(prevdata));
                }
                
                if ((prevdata + 8 + *PAYLOAD(prevdata)) < outOfBounds) {
                    updater = prevdata + 8 + *PAYLOAD(prevdata);
                    *LASTPAYLOAD(updater) = *PAYLOAD(prevdata);
                }
                //printf("Leftover space: %d\n", *PAYLOAD(prevdata));
                //printf("Is Free: %d\n", *CHUNKNOTFREE(prevdata));
                return;
            } else {
                printf("%s: %d: Error: Chunk is already free\n", file, line);
                return;
            }
        } else {
            
            prevdata = metadata;
            //printf("META      PREV: %d   IS NOT FREE: %d   PAY: %d\n", *LASTPAYLOAD(metadata), *CHUNKNOTFREE(metadata), *PAYLOAD(metadata));
            metadata = metadata + 8 + *PAYLOAD(metadata);
        }
    }
    

    printf("Memory is free\n");
}
