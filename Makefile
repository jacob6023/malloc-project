OBJS	= memgrind.o mymalloc.o test1.o
SOURCE	= memgrind.c mymalloc.c test1.c
HEADER	= mymalloc.h
OUT	= a.exe
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

memgrind.o: memgrind.c
	$(CC) $(FLAGS) memgrind.c 

mymalloc.o: mymalloc.c
	$(CC) $(FLAGS) mymalloc.c 

test1.o: test1.c
	$(CC) $(FLAGS) test1.c 


clean:
	rm -f $(OBJS) $(OUT)