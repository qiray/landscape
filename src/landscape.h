#include <string>
#include <vector>

using namespace std;

#ifndef LANDSCAPE_H

#define LANDSCAPE_H

//sizes: //TODO: use these sizes
#define TINY 1
#define SMALL 2
#define NORMAL 3
#define BIG 4
#define LARGE 5

typedef char landscapeCell;
#define MAX_LANDSCAPE_CELL 127

extern vector<vector<int> > rivers; //TODO: maybe move to class

class landscapeAlgorithm {
	static const string diamond_square;
	static const string hill_algorithm;
	static const string perlin_noise;
	
	string type;
	landscapeCell *landscape;
	string outfileName;
	int mapSize, numberOfIslands, islandSize;
	int randomSeed, startHeight;
	float roughness, outHeight;
	float persistence, frequency, amplitude;
	int hillNoise;
	int checkAlgorithm(const string&);
public:
	landscapeAlgorithm(const string&);
	int setType(const string&);
	string setOutFileName(const string&);
	int setMapSize(int);
	int setNumberOfIslands(int);
	int setIslandSize(int);
	int setRandomSeed(int);
	int setStartHeight(int);
	float setRoughness(float);
	float setOutHeight(float);
	float setAmplitude(float);
	float setPersistence(float);
	float setFrequency(float);
	int setHillNoise(int);
	void printLandscape();
	void generateLandscape();
	~landscapeAlgorithm();
	
	friend void generateRivers(landscapeAlgorithm&, int, int, int);
};

#endif
