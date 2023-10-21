Devon Botney, dcb200
Jacob Jeong, jhj48

Our test plan is to run the methods included in memgrind.c, to make sure that
all normal uses of malloc() and free() work, and to make sure that detectable
errors are handled (a few examples being from section 2 of the writeup).

For memgrind, we did the first 3 tests from the writeup. The first test was to
malloc() and then immediately free() a 1-byte object, 120 times. This was
accomplished by using a for loop that looped 120 times. The second test handled
us creating 120 1-byte objects which were stored in an array and then freed after.
This was accomplished through two for loops which did one after the other. The third
test was to create an array of 120 pointers and to make random choices between allocating
and deallocating objects. This test was more difficult to accomplish and allowed
us to catch bugs like freeing multiple chunks in a row causing a pointer to go
out of bounds. As for the custom tests, we made sure that these tests were both
tests to see how effecienct our code was while also allowing us to test for
correctness.

The first of the custom tests was to fill the entirety of memory which accomplishes
what memtest does in that it makes sure that the entirety of memory can be used.
It then frees every other chunk that was allocated. This is to make sure that when
we free it doesn't coallesce random chunks together and that the freed space is
kept free while freeing other spaces. Afterwards, those spaces are then remalloced
which tests to make sure that malloc is able to assign perfect sized chunks to these
freed spaces and not make extra headers. Afterwards, it frees memory so that the next
test can start.

The second of the custom tests was to do something similar to test 3 but instead the
size of the payload was randomized. This makes sure that malloc takes into account
different sized spaces when assigning to a freed space between two allocated chunks.
This also allowed us to catch a bug in the structure of our doubly linked list where
the allocated chunk in front of the newly freed chunk's metadata had not been updated
so it wasn't facing backwards correctly, messing up future frees due to this oversight.
We were able to fix this by including an if statement that corrects the chunk in front
of the newly made unallocated space.

mymalloc 
-----------
The way mymalloc was implemented was to directly insert the metadata into memory using
int16_t variables. This variable always is a 2-byte integer on both 32x and 64x systems.
The reason we used this smaller integer is so that we can do a doubly linked list and
store 3 variables in the header. Doing so allows us to move both forward and backwards
through memory.

Quite a few tests were done on mymalloc to make sure it worked. Test cases that were used
include making sure that mymalloc cannot assign less than 0 bytes to memory (0 bytes
is allowed by normal malloc so I assumed that would be allowed here as well), making sure
the payload + header fit and not just the payload when mallocing, making sure no right header
is made when the payload + header exactly fills up memory, and only creating a new header
after mallocing when the space in front is both free and in bounds.

Error messages were made specific so that the client knows what line and file caused the
error as well as what specifically went wrong so they are able to fix their code.

myfree
------------
The way myfree was implemented was on the same doubly linked list system as mymalloc
where the data was directly inserted into memory instead of using structs.

Free is a bit less complicated than malloc but there were a few tests that were done to
make sure things worked. One of the tests was to make sure that when you make 3 allocated
chunks, free the first and third chunks, and then free the middle one, the data is correctly
freed and the headers are also correct. There was originally an issue where, if these
3 chunks were at the start of memory, an error would occur. This has since been recitfied
through testing. The other issue, which was found through memgrind test 5, was that the
chunk in front of newly coallesced chunks don't have their backwards facing data
updated to reflect the change in size of the unallocated space. This has since been fixed.

There were very few errors that could occur within free so that is reflected by only having
two errors in the function, both being specific to their issue so that the client can
understand what went wrong and fix any code they have wrote.

Testing
---------
All testing was done on a seperate file, not included with the project as they are
non-permanent functions that were made to test individual test cases and to make sure
things were working properly. The main way things were tested afterwards was through
memgrind. All testing done outside of memgrind was documented in the appropriate function
description above.

As for running memgrind, Makefile is already set up to be able to run mymalloc with
tests.