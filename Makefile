CFLAGS = -O3

landscape: diamond_square.o hill_algorithm.o perlin_noise.o landscape.o
	g++ $(CFLAGS) diamond_square.o landscape.o hill_algorithm.o perlin_noise.o -o landscape
	
diamond_square.o: diamond_square.h diamond_square.cpp
	g++ $(CFLAGS) diamond_square.cpp -c

hill_algorithm.o: hill_algorithm.h hill_algorithm.cpp
	g++ $(CFLAGS) hill_algorithm.cpp -c
	
perlin_noise.o: perlin_noise.h perlin_noise.cpp
	g++ $(CFLAGS) perlin_noise.cpp -c	
	
landscape.o: landscape.cpp
	g++ $(CFLAGS) landscape.cpp -c
	
clean:
	rm -f *.o
	rm -f output/*.png
	rm -f landscape
