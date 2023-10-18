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
        printf("INIT\n");
        //Establishes the first header
        *CHUNKISFREE(start) = 1;
        *PAYLOAD(start) = (int16_t)size;
        printf("First chunk size: %d\n", *PAYLOAD(start));
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
            printf("Leftover space: %d\n\n", *PAYLOAD(start));
        }

        return pack;
    }

    //Loop to check for open memory until memory is found or we go out of memory.
    while (count < (512 * 8)) {
        //printf("Current Chunk Space: %d\n", *PAYLOAD(start));
        //Case where there is a header at the end of memory
        if (*PAYLOAD(start) == 0)
        {
            printf("%s: %d: Error: Out of Memory.", file, line);
            return NULL;
        }
        //Space can be allocated
        if (*CHUNKISFREE(start) == 0 && *PAYLOAD(start) >= (size + 8)) 
        {
            //Stores previous size and then stores new size and that it is allocated
            int16_t tempSize = *PAYLOAD(start);
            *CHUNKISFREE(start) = 1;
            *PAYLOAD(start) = (int16_t)size;
            //printf("Chunk size: %d\n", *PAYLOAD(start));

            pack = start + 8;
            //printf("COUNT AND SIZE: %d\n", count + size);
            //printf("CHECKING <-> %d\n", count + ((int)size));

            //Checks to see if a header would fit
            if ((count + size + 8) < (512 * 8)) {
                start = start + 8 + ((int)size);

                *LASTPAYLOAD(start) = (int16_t)size;
                *CHUNKISFREE(start) = 0;
                *PAYLOAD(start) = (int16_t)(tempSize - 8 - ((int16_t)size));
                //printf("Leftover space: %d\n\n", *PAYLOAD(start));
            }

            return pack;
        }
        //Moves to the next chunk if current chunk is not big enough or allocated
        if (*CHUNKISFREE(start) == 1 || *PAYLOAD(start) < (size + 8))
        {
            //printf("Count1 -> %d\n", count);
            count = count + ((int)*PAYLOAD(start)) + 8;
            start = start + *PAYLOAD(start) + 8;
            //printf("Count2 -> %d\n", count);
        }
        
    }
    printf("%s: %d: Error: Out of Memory.", file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    //convert file to void, and compare to ptr
    if ((void*)memory <= ptr && ptr <= (void*)(memory+MEMLENGTH)){
         printf("work")
    }


}
