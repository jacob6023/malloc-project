Devon Botney, dcb200
Jacob _______, _______

Our test plan is to run the methods included in memgrind.c, to make sure that
all normal uses of malloc() and free() work, and to make sure that detectable
errors are handled (a few examples being from section 2 of the writeup).

[Insert talking about memgrind once we do it]

mymalloc 
-----------
The tests for the mymalloc function was written in different method which would
be called by main(). This way the previous test methods could be tracked and
retested for ease of use in case major code changes were made.

The first test was simply to make sure that malloc(0) was detected as an error
and printed a proper error message.

[Insert talking about myfree as implemented]