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

void* mymalloc(size_t size, char *file, int line) {
    //Checks to make sure a positive integer is entered
    if (size <= 0) {
        printf("%s: %d: Error: Cannot Allocate 0 Bytes or Less.", file, line);
        return NULL;
    }
    //Checks to see if initial size is out of bounds
    if ((size) > ((512 * 8) - 8)) {
        printf("%s: %d: Error: Out of Memory.", file, line);
        return NULL;
    }
    //Rounds the inserted size
    size = ROUNDUP8(size);

    //pack is what is returned
    char* pack = NULL;
    //start is the start of the header and moves to the payload when needed
    char* start = (char*)memory;
    //increases as I move through memory to make sure I stay in the bounds of memory
    int count = 1;

    //printf("Value of size: %d\n", size);

    //Checks if there is no allocated memory yet. Then initializes 
    if (*PAYLOAD(start) == 0)
    {
        //Establishes the first header
        *CHUNKISFREE(start) = 1;
        *PAYLOAD(start) = (int16_t)size;

        //Assigns the returned payload and moves the start and inserter to start of 
        //header 2
        pack = start + 8;

        //Checks to see if there is room for next header
        if ((size) != ((512 * 8) - 8)) {
            start = start + (8 + size);

            //Initializes right most header with size of previous chunk
            *LASTPAYLOAD(start) = (int16_t)size;
            //printf("Value inserted: %d\n", *inserter);
            *CHUNKISFREE(start) = 0;
            *PAYLOAD(start) = (int16_t)((MEMLENGTH * 8) - 16 - size);
        }

        return pack;
    }

    //Loop to check for open memory
    while (count < (MEMLENGTH * 8)) {
        //Case where there is a header at the end of memory
        if (*PAYLOAD(start) == 0)
        {
            printf("%s: %d: Error: Out of Memory.", file, line);
            return NULL;
        }
        //Space can be allocated
        if (*CHUNKISFREE(start) == 1 && *PAYLOAD(start) >= size) 
        {
            //Stores previous size and then stores new size and that it is allocated
            int16_t tempSize = *PAYLOAD(start);
            *CHUNKISFREE(start) = 1;
            *PAYLOAD(start) = (int16_t)size;

            pack = start + 8;

            if ((count + tempSize + 8) < MEMLENGTH) {
                count += 8, start += 8;
                count += size, start += size;

                *LASTPAYLOAD(start) = (int16_t)size;
                *CHUNKISFREE(start) = 0;
                *PAYLOAD(start) = (int16_t)(tempSize - 8 - size);
            }
        }
        
    }

    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    

}
