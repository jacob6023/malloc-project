#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mymalloc.h"

void noMemory(void) {
    void* p = malloc(0);
}

void varMemory(int i) {
    void* p = malloc(i);
}

int main(void) {
    //noMemory();
    varMemory(15);
    return 1;
}