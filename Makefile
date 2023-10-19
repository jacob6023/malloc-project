OBJS	= mymalloc.o test1.o
SOURCE	= mymalloc.c test1.c
HEADER	= mymalloc.h
OUT	= a
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

mymalloc.o: mymalloc.c
	$(CC) $(FLAGS) mymalloc.c 

test1.o: test1.c
	$(CC) $(FLAGS) test1.c 

memgrind: memgrind.c mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) memgrind.c mymalloc.c -o memgrind
clean:
	rm -f $(OBJS) $(OUT)
