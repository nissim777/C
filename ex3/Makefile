C_FLAGS = -g -Wextra -Wall -Wvla -std=c99 -DNDEBUG -c
LIBOBJECTS = GenGroup.o

all: main

GenGroup.o: GenGroup.c
	gcc ${C_FLAGS} GenGroup.c

GenGroup: libgenGroup.a

libgenGroup.a: ${LIBOBJECTS}
	ar rcs libgenGroup.a ${LIBOBJECTS}


main: MyGroupMain.o Complex.o Integer.o GenGroup
	gcc MyGroupMain.o Complex.o Integer.o -L. libgenGroup.a -o MyGroupMain


MyGroupMain.o: MyGroupMain.c Complex.h Integer.h libgenGroup.a
	gcc ${C_FLAGS} -I. -libgenGroup.a MyGroupMain.c


Complex.o: Complex.c
	gcc ${C_FLAGS} Complex.c


Integer.o: Integer.c 
	gcc ${C_FLAGS} Integer.c
	

clean:
	rm -f MyGroupMain GenGroup Complex *.o

.PHONY: clean main all GenGroup

