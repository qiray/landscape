#include "landscape.h"
#include "a_star/mapfield.h"

extern float minMaxRandom(float min, float max);

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish, int width);
void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int width);
