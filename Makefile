#
# Created by gmakemake (Ubuntu Sep  7 2011) on Thu Oct  9 23:40:45 2014
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

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

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	expNode.c interp.c parser.c stack.c stackNode.c stackTest.c symTbl.c symTblTest.c
PS_FILES =	
S_FILES =	
H_FILES =	expNode.h interp.h parser.h stack.h stackNode.h symTbl.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	expNode.o parser.o stack.o stackNode.o symTbl.o 

#
# Main targets
#

all:	interp stackTest symTblTest 

interp:	interp.o $(OBJFILES)
	$(CC) $(CFLAGS) -o interp interp.o $(OBJFILES) $(CLIBFLAGS)

stackTest:	stackTest.o $(OBJFILES)
	$(CC) $(CFLAGS) -o stackTest stackTest.o $(OBJFILES) $(CLIBFLAGS)

symTblTest:	symTblTest.o $(OBJFILES)
	$(CC) $(CFLAGS) -o symTblTest symTblTest.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

expNode.o:	expNode.h symTbl.h
interp.o:	expNode.h interp.h parser.h symTbl.h
parser.o:	expNode.h parser.h stack.h stackNode.h symTbl.h
stack.o:	stack.h stackNode.h
stackNode.o:	stackNode.h
stackTest.o:	stack.h stackNode.h
symTbl.o:	symTbl.h
symTblTest.o:	symTbl.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) interp.o stackTest.o symTblTest.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf interp stackTest symTblTest 
