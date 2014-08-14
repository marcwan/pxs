
CC=gcc	
CXX=g++
CPPFLAGS=-g
LEX=flex
YACC=bison
LIBS=-ll


SRCS = main.cc assemblyloader.cc instruction.cc instructionrunner.cc engine.cc refcounted.cc variable.cc constantparser.cc varpool.cc boolean.cc function.cc infinity.cc nan.cc number.cc string.cc undefined.cc exception.cc mathop.cc callfunc.cc exitfunc.cc declare.cc declarefn.cc jump.cc test.cc exit.cc debugprint.cc poparg.cc pusharg.cc removevar.cc set.cc scopestack.cc utils.cc parse.cc parsenode.cc
OBJS = $(SRCS:.cc=.o)
MAIN = pxs

all: $(MAIN)
	echo "Done!"


LEXOBJS=pxs.tab.o
YACCOBJS=lex.yy.o

pxs.tab.o: pxs.tab.c
	$(CC) -c pxs.tab.c

lex.yy.o: lex.yy.c
	$(CC) -c lex.yy.c

pxs.tab.c: pxs.y
	$(YACC) -d pxs.y

lex.yy.c: pxs.l
	$(LEX) pxs.l


$(MAIN): $(LEXOBJS) $(YACCOBJS) $(OBJS)
	$(CXX) -g -o pxs $(LIBS) $(OBJS) $(LEXOBJS) $(YACCOBJS)


clean:
	rm -f $(OBJS) $(MAIN) lex.yy.c pxs.tab.c pxs.tab.h pxs.tab.o lex.yy.o



