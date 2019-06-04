#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <time.h>
#include "landscape.h"
#include "diamond_square.h"
#include "hill_algorithm.h"
#include "perlin_noise.h"
#include "cellular_automata.h"
#include "rivers_generator.h"

//TODO: refactoring
//TODO: version
//TODO: readme

static int rivers_number = 10, river_length = 20, river_width = 2;
static int gens = 20;

const std::string landscapeAlgorithm::diamond_square = "diamond_square";
const std::string landscapeAlgorithm::hill_algorithm = "hill_algorithm";
const std::string landscapeAlgorithm::perlin_noise = "perlin_noise";
const std::string landscapeAlgorithm::cellular_automata = "cellular_automata";

landscapeAlgorithm::landscapeAlgorithm(const std::string &type) {
    if (checkAlgorithm(type))
        this->type = diamond_square;
    else
        this->type = type;
    outfileName = "";
    mapSize = 256; //default values
    numberOfIslands = 1;
    islandSize = 3;
    randomSeed = 0;
    srand(static_cast<unsigned int>(time(nullptr)));
    startHeight = 5;
    roughness = 0.1f;
    outHeight = -5;
    persistence = 0.1f;
    frequency = 0.1f;
    amplitude = 0.1f;
    hillNoise = 0;
    landscape = new landscapeCell[mapSize*mapSize];
}

landscapeAlgorithm::~landscapeAlgorithm() {
    delete [] landscape;
}

int landscapeAlgorithm::checkAlgorithm(const std::string&type) {
    if (type != diamond_square && type != hill_algorithm && type != perlin_noise && type != cellular_automata)
        return 1;
    else
        return 0;
}

std::string landscapeAlgorithm::setOutFileName(const std::string &name) {
    return outfileName = name;
}

int landscapeAlgorithm::setType(const std::string &type) {
    if (checkAlgorithm(type)) {
        this->type = diamond_square;
        return 1;
    } else {
        this->type = type;
        return 0;
    }
}

int landscapeAlgorithm::setMapSize(int size) {
    if (size <= 0)
        size = 256;
    delete [] landscape;
    landscape = new landscapeCell[size*size];
    return mapSize = size;
}

int landscapeAlgorithm::setNumberOfIslands(int number) {
    if (number <= 0)
        return numberOfIslands = 0;
    return numberOfIslands = number;
}

int landscapeAlgorithm::setIslandSize(int size) {
    if (size <= 0)
        return 0;
    return islandSize = size;
}

int landscapeAlgorithm::setRandomSeed(int seed) {
    srand(static_cast<unsigned int>(seed == 0 ? time(nullptr) : seed));
    return randomSeed = seed;
}

int landscapeAlgorithm::setStartHeight(int height) {
    return startHeight = height;
}

float landscapeAlgorithm::setRoughness(float r) {
    return roughness = r;
}

float landscapeAlgorithm::setOutHeight(float h) {
    return outHeight = h;
}

float landscapeAlgorithm::setAmplitude(float a) {
    return amplitude = a;
}

float landscapeAlgorithm::setPersistence(float p) {
    return persistence = p;
}

float landscapeAlgorithm::setFrequency(float f) {
    return frequency = f;
}

int landscapeAlgorithm::setHillNoise(int n) {
    return hillNoise = n;
}

void landscapeAlgorithm::printLandscape() {
    std::streambuf * buf;
    std::fstream fs(outfileName.c_str(), std::fstream::out);
    int flag = 0;
    if (fs.is_open()) {
        flag = 1;
        buf = fs.rdbuf();
    } else {
        std::cerr << "Failed to write into file " << outfileName << std::endl;
        buf = std::cout.rdbuf();
    }
    std::ostream out(buf);
    out << mapSize << std::endl;
    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++)
            out << static_cast<int>(landscape[j + i*mapSize]) << " ";
        out << std::endl;
    }
    if (flag)
        fs.close();
}

float minMaxRandom(float min, float max) {
    return min + rand()/(RAND_MAX/(max - min));
}

inline int nearestPower2(int n) {
    int i = 1;
    while (i < n)
        i *= 2;
    return i;
}

void landscapeAlgorithm::generateLandscape() {
    int length = mapSize*mapSize;
    int heightsLength = (mapSize + 1)*(mapSize + 1);
    float * heights = new float [heightsLength];
    int size = mapSize/((numberOfIslands & (numberOfIslands - 1)) == 0 ? numberOfIslands : nearestPower2(numberOfIslands));
    if (type == hill_algorithm) {
        Hill_algorithm(heights, mapSize + 1, numberOfIslands, roughness, mapSize, mapSize > 128 ? mapSize/5 : 50, MAX_LANDSCAPE_CELL*4);
        if (hillNoise) {
            float *heights2 = new float [heightsLength];
            PerlinNoise(heights2, mapSize + 1, minMaxRandom(static_cast<float>(M_PI*2*10), static_cast<float>(M_PI*3*10)), 1, 0.25, 60.0f/mapSize, 0.1f);
            for (int i = 0; i < heightsLength; i++)
                heights[i] += heights2[i];
            delete [] heights2;
        }
    } else if (type == perlin_noise) {
        PerlinNoise(heights, mapSize + 1, minMaxRandom(static_cast<float>(M_PI*2*10), static_cast<float>(M_PI*3*10)), 5, persistence, frequency, amplitude);
    } else if (type == cellular_automata) {
        CellularAutomaton(heights, mapSize + 1, gens);
    } else //default
        generateDiamondSquareHeights(heights, mapSize + 1, startHeight, roughness, size, outHeight, 0, 0, mapSize, mapSize);
    for (int i = 0; i < length; i++) {
        int tmp = static_cast<int>(roundf((
             heights[i + i/mapSize] + heights[i + i/mapSize + 1] +
             heights[i + i/mapSize + mapSize + 1] + heights[i + i/mapSize + mapSize + 2])/4)); //average value
        landscape[i] = static_cast<landscapeCell>(tmp > MAX_LANDSCAPE_CELL ? MAX_LANDSCAPE_CELL : tmp < -MAX_LANDSCAPE_CELL ? -MAX_LANDSCAPE_CELL : tmp);
    }
    delete [] heights;
}

void generateWorld(landscapeAlgorithm &alg) {
    alg.generateLandscape();
    generateRivers(alg, rivers_number, river_length, river_width);
}

#define PARSE_INT(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(atoi(argv[++i])); \
        continue; \
    }

#define PARSE_INT_VAL(desc, val) \
    if (strcmp(argv[i], desc) == 0) { \
        val = atoi(argv[++i]); \
        continue; \
    }

#define PARSE_FLOAT(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(static_cast<float>(atof(argv[++i]))); \
        continue; \
    }

#define PARSE_STRING(desc, func) \
    if (strcmp(argv[i], desc) == 0) { \
        func(argv[++i]); \
        continue; \
    }

int main(int argc, char **argv) {
    landscapeAlgorithm alg("");
    for (int i = 0; i < argc; i++) {
        PARSE_STRING("--algorithm", alg.setType);
        PARSE_STRING("--output", alg.setOutFileName);
        PARSE_INT("--size", alg.setMapSize);
        PARSE_INT("--islands", alg.setNumberOfIslands);
        PARSE_INT("--islandSize", alg.setIslandSize);
        PARSE_INT("--seed", alg.setRandomSeed);
        PARSE_INT("--height", alg.setStartHeight);
        PARSE_FLOAT("--roughness", alg.setRoughness);
        PARSE_FLOAT("--out_height", alg.setOutHeight);
        PARSE_FLOAT("--amplitude", alg.setAmplitude);
        PARSE_FLOAT("--persistence", alg.setPersistence);
        PARSE_FLOAT("--frequency", alg.setFrequency);
        PARSE_INT_VAL("--rivers_number", rivers_number);
        PARSE_INT_VAL("--gens", gens);
        if (strcmp(argv[i], "--noise") == 0) {
            alg.setHillNoise(1);
            continue;
        }
    }
    generateWorld(alg);
    alg.printLandscape();
    return 0;
}
