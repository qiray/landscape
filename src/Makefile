CFLAGS = -O3

ifeq ($(OS),win) 
	CPP=i686-w64-mingw32-g++
	LDFLAGS= -static-libstdc++ -static -s 
	BIN=landscape.exe
else #readelf -d landscape
	CPP=g++
	LDFLAGS= -s
	BIN=landscape
endif

OUTPUT_PATH=../output
JAVA_GUI_PATH=../utils/java
BIN_PATH=../bin

all: $(BIN) JavaGUI

$(BIN): diamond_square.o hill_algorithm.o perlin_noise.o rivers_generator.o landscape.o astar.a
	mkdir -p $(OUTPUT_PATH)
	mkdir -p $(BIN_PATH)
	$(CPP) $(CFLAGS) diamond_square.o landscape.o hill_algorithm.o perlin_noise.o rivers_generator.o a_star/astar.a -o $(BIN) $(LDFLAGS)
	mv $(BIN) $(BIN_PATH)
	
diamond_square.o: diamond_square.h diamond_square.cpp
	$(CPP) $(CFLAGS) diamond_square.cpp -c

hill_algorithm.o: hill_algorithm.h hill_algorithm.cpp
	$(CPP) $(CFLAGS) hill_algorithm.cpp -c
	
perlin_noise.o: perlin_noise.h perlin_noise.cpp
	$(CPP) $(CFLAGS) perlin_noise.cpp -c
	
rivers_generator.o: rivers_generator.cpp rivers_generator.h
	$(CPP) $(CFLAGS) rivers_generator.cpp -c
	
landscape.o: landscape.cpp
	$(CPP) $(CFLAGS) landscape.cpp -c
	
astar.a: a_star/*.cpp a_star/*.h
	cd a_star && make OS=$(OS) && cd ..
	
JavaGUI: $(JAVA_GUI_PATH)/*.java
	pushd . && cd $(JAVA_GUI_PATH) && make && popd
	
clean:
	rm -f *.o *.class
	rm -f $(OUTPUT_PATH)/*
	rm -f $(BIN_PATH)/$(BIN)
	pushd . && cd $(JAVA_GUI_PATH) && make clean && popd
	cd a_star && make clean && cd ..
