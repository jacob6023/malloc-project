#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "mymalloc.h"

struct timeval start_time, end_time;

void test1() {
    for(int i = 0; i < 120; i++) {
        char *ptr = malloc(1);  // Allocate 1 byte of memory
        free(ptr);  // Release the memory
    }
}

void test2() {
    char *ptrArray[120];  // Array to store 120 pointers

    for(int i = 0; i < 120; i++) {
        ptrArray[i] = malloc(1);  // Allocate 1 byte of memory and store the address
    }

    for(int i = 0; i < 120; i++) {
        free(ptrArray[i]);  // Release the memory
    }
}
void test3() {
    char *ptrArray[120];  // Array to store 120 pointers
    int allocated[120] = {0};  // Initialize the memory allocation status array
    int loc = 0;  // Current location

    for(int i = 0; i < 120; i++) {
        int random = rand() % 2;
        //printf("Random %d\n", random);
        if(loc == 0 || (random == 0 && loc < 120)) {
            // Allocate 1 byte of memory and store the address
            //printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(1);
            allocated[loc] = 1;
            loc++;
            //printf("CREATE %d\n", loc);
        } else {
            // Release the memory
            //printf("free loc=%d\n", loc);
            if (loc > 0) {
                loc--;
                free(ptrArray[loc]);
                allocated[loc] = 0;
            }
            //printf("FREE %d\n", loc);
        }
    }

    //printf("Process is done.\n");

    // Clean up any unreleased memory
    for(int i = 0; i < 120; i++) {
        if(allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }
}

void test4() {
    char *ptrArray[64];

    for (int i = 0; i < 64; i++) {
        ptrArray[i] = malloc(56);
    }

    for (int i = 0; i < 32; i++) {
        free(ptrArray[i*2]);
    }

    for (int i = 0; i < 32; i++) {
        ptrArray[i*2] = malloc(56);
    }

    for(int i = 0; i < 64; i++) {
        free(ptrArray[i]);
    }
}

void test5() {
    char *ptrArray[60];  // Array to store 120 pointers
    int allocated[60] = {0};  // Initialize the memory allocation status array
    int loc = 0;  // Current location

    for(int i = 0; i < 60; i++) {
        int random = rand() % 2;

        int randomSize = rand() % 3;

        switch (randomSize)
        {
        case 0:
            randomSize = 8;
            break;
        case 1:
            randomSize = 16;
            break;
        case 2:
            randomSize = 24;
            break;
        default:
            break;
        }
        //printf("Random %d\n", random);
        if(loc == 0 || (random == 0 && loc < 60)) {
            // Allocate 1 byte of memory and store the address
            //printf("alloc loc=%d\n", loc);
            ptrArray[loc] = malloc(randomSize);
            allocated[loc] = 1;
            loc++;
            //printf("CREATE %d\n", loc);
        } else {
            // Release the memory
            //printf("free loc=%d\n", loc);
            if (loc > 0) {
                loc--;
                free(ptrArray[loc]);
                allocated[loc] = 0;
            }
            //printf("FREE %d\n", loc);
        }
    }

    //printf("Process is done.\n");

    // Clean up any unreleased memory
    for(int i = 0; i < 60; i++) {
        if(allocated[i] == 1) {
            free(ptrArray[i]);
        }
    }
}

int main() {
    gettimeofday(&start_time, NULL);
    //printf("START: %ld\n", start_time.tv_usec);
    for(int i = 0; i < 50; i++) {
        test1();
        
    }
    gettimeofday(&end_time, NULL);
    //printf("END: %ld\n", end_time.tv_usec);
    long avg = (double)((end_time.tv_usec - start_time.tv_usec) / 50);
    printf("Test 1 average runtime: %ld microseconds\n", avg);

    
    gettimeofday(&start_time, NULL);
    //printf("START: %ld\n", start_time.tv_usec);
    for (int i = 0; i < 50; i++) {
        test2();
    }
    gettimeofday(&end_time, NULL);
    //printf("END: %ld\n", end_time.tv_usec);
    avg = (double)((end_time.tv_usec - start_time.tv_usec) / 50);
    printf("Test 2 average runtime: %ld microseconds\n", avg);

    gettimeofday(&start_time, NULL);
    for(int i = 0; i < 50; i++) {
        test3();
    }
    gettimeofday(&end_time, NULL);
    avg = (double)((end_time.tv_usec - start_time.tv_usec) / 50);
    printf("Test 3 average runtime: %ld microseconds\n", avg);

    gettimeofday(&start_time, NULL);
    for(int i = 0; i < 1; i++) {
        test4();
    }
    gettimeofday(&end_time, NULL);
    avg = (double)((end_time.tv_usec - start_time.tv_usec) / 50);
    printf("Test 4 average runtime: %ld microseconds\n", avg);

    gettimeofday(&start_time, NULL);
    for(int i = 0; i < 50; i++) {
        test5();
    }
    gettimeofday(&end_time, NULL);
    avg = (double)((end_time.tv_usec - start_time.tv_usec) / 50);
    printf("Test 5 average runtime: %ld microseconds\n", avg);
}
