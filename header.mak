CC=		gcc
CFLAGS= 	-ggdb -Wall -std=c99
SOURCES=	interp.c parser.c expNode.c stack.c stackNode.c symTbl.c
OBJECTS=	$(SOURCES:.c=.o)
EXECUTABLE=	interp

interp : interp.o parser.o expNode.o stack.o stackNode.o symTbl.o
	$(CC) $(CFLAGS) -o interp interp.o parser.o expNode.o stack.o stackNode.o symTbl.o

interp.o : interp.c interp.h parser.h symTbl.h
	$(CC) $(CFLAGS) -c interp.c

parser.o : parser.c parser.h stack.h expNode.h
	$(CC) $(CFLAGS) -c parser.c

expNode.o : expNode.c expNode.h symTbl.h
	$(CC) $(CFLAGS) -c expNode.c

stack.o : stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

stackNode.o : stackNode.c stackNode.h
	$(CC) $(CFLAGS) -c stackNode.c

symTbl.o : symTbl.c symTbl.h
	$(CC) $(CFLAGS) -c symTbl.c
