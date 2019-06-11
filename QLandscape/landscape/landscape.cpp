
#include <iostream>
#include <fstream>
#include "landscape.h"
#include "diamond_square.h"
#include "hill_algorithm.h"
#include "perlin_noise.h"
#include "cellular_automata.h"
#include "rivers_generator.h"

const std::string LandscapeAlgorithm::diamond_square = "diamond_square";
const std::string LandscapeAlgorithm::hill_algorithm = "hill_algorithm";
const std::string LandscapeAlgorithm::perlin_noise = "perlin_noise";
const std::string LandscapeAlgorithm::cellular_automata = "cellular_automata";

LandscapeAlgorithm::LandscapeAlgorithm() {
    srand(static_cast<unsigned int>(time(nullptr)));
    //default values
    type = diamond_square;
    outfileName = "1.map";
    mapSize = 512;
    numberOfIslands = 1;
    islandSize = 3;
    randomSeed = 0;
    startHeight = 5;
    roughness = 0.1f;
    outHeight = -5;
    persistence = 0.1f;
    frequency = 0.1f;
    amplitude = 0.1f;
    hillNoise = 0;
    generations = 20;
    rivers_number = 10;
    river_length = 20;
    river_width = 2;
    landscape = new landscapeCell[mapSize*mapSize];
}

LandscapeAlgorithm::~LandscapeAlgorithm() {
    delete [] landscape;
}

bool LandscapeAlgorithm::isValidType(const std::string &type) {
    return type == diamond_square || type == hill_algorithm || type == perlin_noise || type == cellular_automata;
}

std::string LandscapeAlgorithm::setOutFileName(const std::string &name) {
    return outfileName = name;
}

int LandscapeAlgorithm::setType(const std::string &type) {
    if (!isValidType(type))
        return 1;
    this->type = type;
    return 0;
}

int LandscapeAlgorithm::setMapSize(int size) {
    if (size <= 0)
        size = 512;
    delete [] landscape;
    landscape = new landscapeCell[size*size];
    return mapSize = size;
}

int LandscapeAlgorithm::setNumberOfIslands(int number) {
    if (number <= 0)
        return numberOfIslands = 0;
    return numberOfIslands = number;
}

int LandscapeAlgorithm::setIslandSize(int size) {
    if (size <= 0)
        return 0;
    return islandSize = size;
}

int LandscapeAlgorithm::setRandomSeed(int seed) {
    srand(static_cast<unsigned int>(seed == 0 ? time(nullptr) : seed));
    return randomSeed = seed;
}

int LandscapeAlgorithm::setStartHeight(int height) {
    return startHeight = height;
}

float LandscapeAlgorithm::setRoughness(float r) {
    return roughness = r;
}

float LandscapeAlgorithm::setOutHeight(float h) {
    return outHeight = h;
}

float LandscapeAlgorithm::setAmplitude(float a) {
    return amplitude = a;
}

float LandscapeAlgorithm::setPersistence(float p) {
    return persistence = p;
}

float LandscapeAlgorithm::setFrequency(float f) {
    return frequency = f;
}

int LandscapeAlgorithm::setHillNoise(int n) {
    return hillNoise = n;
}

void LandscapeAlgorithm::printLandscape() {
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

void LandscapeAlgorithm::makeHillNoise(float *heights, int heightsLength) {
    float *heights2 = new float [heightsLength];
    PerlinNoise(heights2, mapSize + 1, minMaxRandom(static_cast<float>(M_PI*2*10), static_cast<float>(M_PI*3*10)), 1, 0.25, 60.0f/mapSize, 0.1f);
    for (int i = 0; i < heightsLength; i++)
        heights[i] += heights2[i];
    delete [] heights2;
}

void LandscapeAlgorithm::applyHeightsToLandscape(float *heights, int length) {
    for (int i = 0; i < length; i++) {
        int tmp = static_cast<int>(roundf((
             heights[i + i/mapSize] + heights[i + i/mapSize + 1] +
             heights[i + i/mapSize + mapSize + 1] + heights[i + i/mapSize + mapSize + 2])/4)); //average value
        if (tmp > MAX_LANDSCAPE_CELL)
            tmp = MAX_LANDSCAPE_CELL;
        else if (tmp < -MAX_LANDSCAPE_CELL)
            tmp = -MAX_LANDSCAPE_CELL;
        landscape[i] = static_cast<landscapeCell>(tmp);
    }
}

void LandscapeAlgorithm::generateRivers(int rivers_number, int river_length, int river_width) {
    RiverGenerator rg;
    rg.generateRivers(rivers_number, river_length, river_width, landscape, mapSize);
}

void LandscapeAlgorithm::generateLandscape() {
    int length = mapSize*mapSize;
    int heightsLength = (mapSize + 1)*(mapSize + 1);
    float * heights = new float [heightsLength];
    int size = mapSize/((numberOfIslands & (numberOfIslands - 1)) == 0 ? numberOfIslands : nearestPower2(numberOfIslands));
    if (type == hill_algorithm) {
        Hill_algorithm(heights, mapSize + 1, numberOfIslands, roughness, mapSize, mapSize > 128 ? mapSize/5 : 50, MAX_LANDSCAPE_CELL*4);
        if (hillNoise)
            makeHillNoise(heights, heightsLength);
    } else if (type == perlin_noise) {
        PerlinNoise(heights, mapSize + 1, minMaxRandom(static_cast<float>(M_PI*2*10), static_cast<float>(M_PI*3*10)), 5, persistence, frequency, amplitude);
    } else if (type == cellular_automata) {
        CellularAutomaton(heights, mapSize + 1, generations);
    } else //default
        generateDiamondSquareHeights(heights, mapSize + 1, startHeight, roughness, size, outHeight, 0, 0, mapSize, mapSize);
    applyHeightsToLandscape(heights, length);
    delete [] heights;
    generateRivers(rivers_number, river_length, river_width);
}

void LandscapeAlgorithm::setGenerations(int generations) {
    this->generations = generations;
}

void LandscapeAlgorithm::setRiversCount(int count) {
    if (count >= 0)
        rivers_number = count;
}
