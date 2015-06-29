#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cmath>
#include <time.h>
#include "landscape.h"
#include "diamond_square.h"
#include "hill_algorithm.h"
#include "perlin_noise.h"
#include "rivers_generator.h"

using namespace std;

string landscapeAlgorithm::currentAlgorithm = "diamond_square"; //default value

void landscapeAlgorithm::checkAlgorithm() {
	if (currentAlgorithm != diamond_square && currentAlgorithm != hill_algorithm && currentAlgorithm != perlin_noise)
		currentAlgorithm = diamond_square;
}

string outputName("");
int mapSize = 16, numberOfIslands = 1, islandSize = NORMAL;
int randomSeed = 0, startHeight = 5;
float roughness = 0.1, outHeight = -5;
float persistence = 0.1, frequency = 0.1, amplitude = 0.1;
int hillNoise = 0;
int rivers_number = 10, river_length = 100;

float minMaxRandom(float min, float max) {
	return min + rand()/(RAND_MAX/(max - min));
}

inline int nearestPower2(int n) {
	int i = 1;
	while (i < n)
		i *= 2;
	return i;
}

void generateHeights(landscapeCell *landscape, int mapSize, int height, float roughness, float outHeight, int numberOfIslands, int islandSize) {
	int length = mapSize*mapSize;
	int heightsLength = (mapSize + 1)*(mapSize + 1);
	float * heights = new float [heightsLength];
	int size = mapSize/(numberOfIslands & (numberOfIslands - 1) == 0 ? numberOfIslands : nearestPower2(numberOfIslands));
	if (landscapeAlgorithm::currentAlgorithm == landscapeAlgorithm::hill_algorithm) {
		hill_algorithm(heights, mapSize + 1, numberOfIslands, islandSize, roughness, mapSize, mapSize > 128 ? mapSize/5 : 50, MAX_LANDSCAPE_CELL*4);
		if (hillNoise) {
			float *heights2 = new float [heightsLength];
			PerlinNoise(heights2, mapSize + 1, minMaxRandom(M_PI*2*10, M_PI*3*10), 1, 0.25, 60.0/mapSize, 0.1);
			for (int i = 0; i < heightsLength; i++)
				heights[i] += heights2[i];
			delete [] heights2;
		}
	} else if (landscapeAlgorithm::currentAlgorithm == landscapeAlgorithm::perlin_noise)
		PerlinNoise(heights, mapSize + 1, minMaxRandom(M_PI*2*10, M_PI*3*10), 5, persistence, frequency, amplitude);
	else //default
		generateDiamondSquareHeights(heights, mapSize + 1, height, roughness, size, outHeight, 0, 0, mapSize, mapSize);
	for (int i = 0; i < length; i++) {
		int tmp = round((heights[i + i/mapSize] + heights[i + i/mapSize + 1] + heights[i + i/mapSize + mapSize + 1] + heights[i + i/mapSize + mapSize + 2])/4); //average value
		landscape[i] = tmp > MAX_LANDSCAPE_CELL ? MAX_LANDSCAPE_CELL : tmp < -MAX_LANDSCAPE_CELL ? -MAX_LANDSCAPE_CELL : tmp;
	}
	delete [] heights;
}

void generateLandscape(landscapeCell *landscape, int mapSize, int height, float roughness, float outHeight, int numberOfIslands, int islandSize) {
	generateHeights(landscape, mapSize, height, roughness, outHeight, numberOfIslands, islandSize);
	generateRivers(landscape, mapSize, rivers_number, river_length, 5);
}

void printLandscape(landscapeCell *landscape, int mapSize, const string &outputName) {
	if (outputName.length() > 0) {
		fstream fs(outputName.c_str(), fstream::out);
		if (fs.is_open()) {
			fs << mapSize << endl;
			for (int i = 0; i < mapSize; i++) {
				for (int j = 0; j < mapSize; j++)
					fs << (int)landscape[j + i*mapSize] << " ";
				fs << endl;
			}
			fs.close();
		} else {
			cerr << "Failed to open file " << outputName << endl;
			return;
		}
	} else {
		cout << mapSize << endl;
		for (int i = 0; i < mapSize; i++) {
			for (int j = 0; j < mapSize; j++)
				cout << (int)landscape[j + i*mapSize] << " ";
			cout << endl;
		}
	}
}

#define PARSE_INT(desc, val) \
	if (strcmp(argv[i], desc) == 0) { \
		val = atoi(argv[++i]); \
		continue; \
	}
	
#define PARSE_FLOAT(desc, val) \
	if (strcmp(argv[i], desc) == 0) { \
		val = atof(argv[++i]); \
		continue; \
	}
	
#define PARSE_STRING(desc, val) \
	if (strcmp(argv[i], desc) == 0) { \
		val = argv[++i]; \
		continue; \
	}	

int main(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		PARSE_STRING("--output", outputName);
		PARSE_INT("--size", mapSize);
		PARSE_INT("--islands", numberOfIslands);
		PARSE_INT("--islandSize", islandSize);
		PARSE_INT("--seed", randomSeed);
		PARSE_INT("--height", startHeight);
		PARSE_FLOAT("--roughness", roughness);
		PARSE_FLOAT("--out_height", outHeight);
		PARSE_STRING("--algorithm", landscapeAlgorithm::currentAlgorithm);
		PARSE_FLOAT("--amplitude", amplitude);
		PARSE_FLOAT("--persistence", persistence);
		PARSE_FLOAT("--frequency", frequency);
		PARSE_FLOAT("--rivers_number", rivers_number);
		PARSE_FLOAT("--river_length", river_length);
		if (strcmp(argv[i], "--noise") == 0) {
			hillNoise = 1;
			continue;
		}
	}
	if (mapSize <= 0)
		mapSize = 256; //default
	landscapeAlgorithm::checkAlgorithm();
	srand(randomSeed == 0 ? time(NULL) : randomSeed);
	landscapeCell *landscape = new landscapeCell [mapSize*mapSize];
	cout << "Out file name = '" << outputName << "', size = " << mapSize << ", number of islands = " << numberOfIslands << endl;
	generateLandscape(landscape, mapSize, startHeight, roughness, outHeight, numberOfIslands, islandSize);
	printLandscape(landscape, mapSize, outputName);
	delete [] landscape;
	return 0;
}
