C_FLAGS = -g -Wextra -Wall -Wvla -std=c99 -c

all: PlayBoard Gomoku

PlayBoard: PlayBoard.o Board.o ErrorHandle.o
	gcc PlayBoard.o Board.o ErrorHandle.o –DNDEBUG -o PlayBoard

PlayBoard.o: PlayBoard.c Board.h ErrorHandle.h
	gcc ${C_FLAGS} PlayBoard.c

Board.o: Board.c Board.h ErrorHandle.h
	gcc ${C_FLAGS} Board.c

ErrorHandle.o: ErrorHandle.c ErrorHandle.h
	gcc ${C_FLAGS} ErrorHandle.c

Gomoku.o: Gomoku.c Board.h ErrorHandle.h
	gcc ${C_FLAGS} Gomoku.c

Gomoku: Gomoku.o Board.o ErrorHandle.o
	gcc Gomoku.o Board.o ErrorHandle.o –DNDEBUG -o Gomoku
	
clean: 
	rm -f PlayBoard Gomoku *.o
