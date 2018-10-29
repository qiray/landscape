
SHELL:=/bin/bash

.PHONY: all clean src utils

all: src utils

src: src/*.h src/*.cpp src/Makefile
	cd src && make ; cd ..

utils: utils/java/Makefile utils/java/*.java
	cd utils/java/ && make ; cd ../..

clean:
	cd src && make clean && cd ..
	cd utils/java && make clean && cd ../..

allclean: clean
	rm -rf output/*
