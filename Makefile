OBJS	= mymalloc.o memgrind.o
SOURCE	= mymalloc.c memgrind.c
HEADER	= mymalloc.h
OUT	= a.exe
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

mymalloc.o: mymalloc.c
	$(CC) $(FLAGS) mymalloc.c 

memgrind.o: memgrind.c
	$(CC) $(FLAGS) memgrind.c 


clean:
	rm -f $(OBJS) $(OUT)