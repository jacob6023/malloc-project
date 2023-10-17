#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mymalloc.h"

void* mymalloc(size_t size, char *file, int line) {
    if (size == 0) {
        printf("%s: %d: Error: Cannot Allocate 0 Bytes.", file, line);
        return NULL;
    }
    return NULL;
}

void myfree(void *ptr, char *file, int line) {

}