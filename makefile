CFLAG = -Wall 
LIBS = -lncurses
CC = gcc
EXEC = jogo
MAIN = jogo.c
INCLUDE = pacman.c
OBJS = jogo.o pacman.o

all: hello

hello: $(OBJS)
	$(CC) -o $(EXEC) $(MAIN) $(INCLUDE) $(LIBS) $(CFLAG)

jogo.o: jogo.c pacman.h 
pacman.o: pacman.c pacman.h 

clean:
	-rm -f *~ *.o

purge:
	-rm -f $(OBJS) $(EXEC)
