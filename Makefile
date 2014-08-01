
CXX=g++
CPPFLAGS=-g


SRCS = main.cc assemblyloader.cc instruction.cc instructionrunner.cc engine.cc refcounted.cc variable.cc constantparser.cc varpool.cc boolean.cc function.cc infinity.cc nan.cc number.cc string.cc undefined.cc exception.cc mathop.cc callfunc.cc exitfunc.cc declare.cc declarefn.cc compare.cc jump.cc exit.cc debugprint.cc poparg.cc pusharg.cc removevar.cc set.cc scopestack.cc utils.cc
OBJS = $(SRCS:.cc=.o)
MAIN = pxs

all: $(MAIN)
	echo "Done!"


$(MAIN): $(OBJS)
	$(CXX) -g -o pxs $(OBJS)



clean:
	rm -f $(OBJS) $(MAIN)



