
CPPFLAGS=-g -Wc++11-extensions


SRCS = main.cc instruction.cc refcounted.cc variable.cc constantparser.cc varpool.cc boolean.cc infinity.cc nan.cc number.cc string.cc unknown.cc exception.cc mathop.cc addvar.cc compare.cc exit.cc debugprint.cc removevar.cc set.cc scopestack.cc utils.cc
OBJS = $(SRCS:.cc=.o)
MAIN = pxs

all: $(MAIN)
	echo "Done!"


$(MAIN): $(OBJS)
	g++ -g -o pxs $(OBJS)



clean:
	rm -f $(OBJS) $(MAIN)



