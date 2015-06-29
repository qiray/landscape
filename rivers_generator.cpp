#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

inline bool suitableHeight(landscapeCell *landscape, int newIndex, int index) {
	int diff = landscape[newIndex] - landscape[index];
	if (diff <= 0)
		return true;
	int probability = rand()%101;
	if (probability >= 25 && diff == 1)
		return true;
	if (probability >= 50 && diff == 2)
		return true;
	if (probability >= 90 && diff == 3)
		return true;
	return false;
}

inline void checkNeighbour(landscapeCell *landscape, int mapSize, const vector<int>& river, vector<int>& candidates, int diff, int index) {
	int x = index%mapSize, y = index/mapSize;
	if (diff == 1 || diff == -1)
		x += diff;
	else
		y += diff < 0 ? -1 : 1;
	int newIndex = index + diff;
	if (x >= 0 && x < mapSize && y >= 0 && y < mapSize && suitableHeight(landscape, newIndex, index) &&
	    find(river.begin(), river.end(), newIndex) == river.end())
		candidates.push_back(newIndex);
}

inline int nextRiverCell (landscapeCell *landscape, int mapSize, const vector<int>& river, int index) {
	vector<int> candidates;
	checkNeighbour(landscape, mapSize, river, candidates, 1, index);
	checkNeighbour(landscape, mapSize, river, candidates, -1, index);
	checkNeighbour(landscape, mapSize, river, candidates, mapSize, index);
	checkNeighbour(landscape, mapSize, river, candidates, -mapSize, index);
	int size = candidates.size();
	if (size == 0)
		return -1;
	else
		return candidates[rand()%size];
}

bool generateRiver(landscapeCell *landscape, int mapSize, int index, int length, int size) {
	vector<int> river;
	river.push_back(index);
	int riverLength = length;
	while (/*riverLength != 0 && */landscape[index] > 0) {
		index = nextRiverCell(landscape, mapSize, river, index);
		if (index == -1)
			return false;
		river.push_back(index);
		riverLength--;
	}
	printf("River done! Length = %d\n", river.size());
	for (int i = river.size() - 1; i >= 0; i--)
		landscape[river[i]] = 0; //TODO: replace with something
	return true;
}

inline int distanceToWater(landscapeCell *landscape, int mapSize, int index) {
	
}

void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int size) {
	while (number > 0) {
		int index = -1, len = mapSize*mapSize;
		do {
			index = rand()%len;
		} while (distanceToWater(landscape, mapSize, index) < 50);
		if (generateRiver(landscape, mapSize, index, length, size))
			number--;
	}
}
