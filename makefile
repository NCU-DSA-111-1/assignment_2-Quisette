CC = gcc
SRC := $(wildcard ./src/*.c)
LL := $(wildcard ./src/linkedlist/*.c)
main.o: FORCE
	
clean:
	rm *.o

FORCE:
	$(CC) $(SRC) $(LL) -o ./obj/main
	#$(CC) main.c -g shogilib.c -g  utilities.c -g  stacklib.c -g linkedlist/linkedlist.c -g -o main 
