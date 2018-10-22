OBJS 	= mygraph.o graph_interface.o
SOURCE	= mygraph.c graph_interface.c
HEADER  = graph_interface.h mytypes.h
OUT  	= mygraph
CC	= gcc
FLAGS   = -g -c

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mygraph.o: mygraph.c
	$(CC) $(FLAGS) mygraph.c

graph_interface.o: graph_interface.c
	$(CC) $(FLAGS) graph_interface.c

clean:
	rm -f $(OBJS) $(OUT)
