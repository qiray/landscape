CPP= g++
CFLAGS = -O3

astar.a: node.o binary_heap.o mapfield.o astar.o
	ar rvs astar.a node.o binary_heap.o mapfield.o astar.o

#astar: astar.o mapfield.o node.o binary_heap.o makefile
#	$(CC) -g node.o mapfield.o binary_heap.o astar.o -o astar
	
node.o:	node.cpp node.h
	$(CPP) $(CFLAGS) node.cpp -c

binary_heap.o: binary_heap.cpp binary_heap.h
	$(CPP) $(CFLAGS) binary_heap.cpp -c
	
mapfield.o: mapfield.cpp mapfield.h
	$(CPP) $(CFLAGS) mapfield.cpp -c
	
astar.o: astar.cpp
	$(CPP) $(CFLAGS) astar.cpp -c

clean:
	rm -f *.o astar astar.a
