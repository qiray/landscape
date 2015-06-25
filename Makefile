CFLAGS = -O3

ifeq ($(OS),win) 
CPP=i686-w64-mingw32-g++
LDFLAGS= -static-libstdc++ -static -s 
#CPP=x86_64-w64-mingw32-g++
BIN=landscape.exe
else
CPP=g++
LDFLAGS= -s
BIN=landscape
endif

OUTPUT_PATH=output
JAVA_GUI_PATH=java

all: $(BIN) JavaGUI

$(BIN): diamond_square.o hill_algorithm.o perlin_noise.o landscape.o
	$(CPP) $(CFLAGS) diamond_square.o landscape.o hill_algorithm.o perlin_noise.o -o $(BIN) $(LDFLAGS)
	cp $(BIN) $(JAVA_GUI_PATH)
	
diamond_square.o: diamond_square.h diamond_square.cpp
	$(CPP) $(CFLAGS) diamond_square.cpp -c

hill_algorithm.o: hill_algorithm.h hill_algorithm.cpp
	$(CPP) $(CFLAGS) hill_algorithm.cpp -c
	
perlin_noise.o: perlin_noise.h perlin_noise.cpp
	$(CPP) $(CFLAGS) perlin_noise.cpp -c
	
landscape.o: landscape.cpp
	$(CPP) $(CFLAGS) landscape.cpp -c
	
JavaGUI: $(JAVA_GUI_PATH)/*.java
	cd $(JAVA_GUI_PATH) && make && cd ..
	
clean:
	rm -f *.o *.class 1.txt
	rm -f $(OUTPUT_PATH)/*.png
	rm -f landscape landscape.exe JavaGUI $(JAVA_GUI_PATH)/landscape $(JAVA_GUI_PATH)/landscape.exe
	cd $(JAVA_GUI_PATH) && make clean && cd ..
