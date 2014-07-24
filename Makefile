
CPPFLAGS = -g -Wc++11-extensions


SRCS = main.cc instruction.cc refcounted.cc variable.cc varpool.cc number.cc unknown.cc exception.cc add.cc addvar.cc exit.cc debugprint.cc freetemp.cc gettemp.cc scopestack.cc utils.cc
OBJS = $(SRCS:.cc=.o)
MAIN = pxs


all: $(MAIN)
	echo "Done!"


$(MAIN): $(OBJS)
	g++ -g -o pxs $(OBJS)



clean:
	rm -f $(OBJS) $(MAIN)
