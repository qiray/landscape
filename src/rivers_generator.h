#include "landscape.h"
#include "a_star/mapfield.h"

extern float minMaxRandom(float min, float max);

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish);
void generateRivers(landscapeAlgorithm &alg, int number, int length, int width);