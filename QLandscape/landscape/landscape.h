#include <string>
#include <vector>

#ifndef LANDSCAPE_H

#define LANDSCAPE_H

typedef char landscapeCell;
#define MAX_LANDSCAPE_CELL 127

#define MAJOR_VERSION 0 //TODO: use version
#define MINOR_VERSION 0
#define BUILD_VERSION 1

float minMaxRandom(float min, float max);

class LandscapeAlgorithm {
    static const std::string diamond_square;
    static const std::string hill_algorithm;
    static const std::string perlin_noise;
    static const std::string cellular_automata;
    
    std::string type;
    landscapeCell *landscape;
    std::string outfileName;
    int mapSize, numberOfIslands, islandSize;
    int randomSeed, startHeight;
    float roughness, outHeight;
    float persistence, frequency, amplitude;
    int hillNoise;
    int generations;
    int rivers_number, river_length, river_width;
    bool isValidType(const std::string&);
    void makeHillNoise(float *heights, int heightsLength);
    void applyHeightsToLandscape(float *heights, int length);
public:
    LandscapeAlgorithm();
    ~LandscapeAlgorithm();
    std::string setOutFileName(const std::string&);
    int setType(const std::string&);
    int setMapSize(int);
    int setNumberOfIslands(int);
    int setIslandSize(int);
    int setRandomSeed(int);
    int setStartHeight(int);
    int setHillNoise(int);
    float setRoughness(float);
    float setOutHeight(float);
    float setAmplitude(float);
    float setPersistence(float);
    float setFrequency(float);
    void printLandscape();
    void generateLandscape();
    void setGenerations(int generations);
    void setRiversCount(int count);
    void generateRivers(int, int, int);
};

#endif
