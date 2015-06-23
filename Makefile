CFLAGS = -O3

OUTPUT_PATH=output
JAVA_GUI_PATH=java

all: landscape JavaGUI

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
	
JavaGUI: $(JAVA_GUI_PATH)/*.java
	cd $(JAVA_GUI_PATH) && make && cd ..
	
clean:
	rm -f *.o *.class 1.txt
	rm -f $(OUTPUT_PATH)/*.png
	rm -f landscape JavaGUI
	cd $(JAVA_GUI_PATH) && make clean && cd ..
