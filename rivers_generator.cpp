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
	/*if (probability >= 99)
		return true;*/
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

bool generateRiver(landscapeCell *landscape, int mapSize, int index, int size) {
	vector<int> river;
	river.push_back(index);
	int riverLength = 0;
	while (landscape[index] > 0 && !(index%mapSize == 0 || index/mapSize == 0 || index%mapSize == mapSize - 1 || index/mapSize == mapSize - 1)) {
		index = nextRiverCell(landscape, mapSize, river, index);
		if (index == -1)
			return false;
		river.push_back(index);
		riverLength++;
	}
	printf("River done! Length = %d\n", riverLength);
	rivers.push_back(river);
	return true;
}

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y) {
	if (x < 0 || y < 0 || x > mapSize || y > mapSize || landscape[y*mapSize + x] <= 0)
		return true;
	else
		return false;
}

inline int distanceToWater(landscapeCell *landscape, int mapSize, int index, int step) {
	int dist = 1, x = index%mapSize, y = index/mapSize;
	while(1) {
		for (int i = x - dist; i <= x + dist; i++)
			if (isWater(landscape, mapSize, i, y - dist) || isWater(landscape, mapSize, i, y + dist))
				return dist;
		for (int i = y - dist + 1; i <= y + dist - 1; i++)
			if (isWater(landscape, mapSize, x - dist, i) || isWater(landscape, mapSize, x + dist, i))
				return dist;
		if (x - dist < 0 && x + x + dist > mapSize && y - dist < 0 && y + dist > mapSize)
			return -1;
		dist += step;
	}
}

void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int size) {
	vector<int> highlands;
	int len = mapSize*mapSize, max = 0, min = MAX_LANDSCAPE_CELL;
	double average = 0.0, factor = 0.5;
	for (int i = 0; i < len; i++)
		if (landscape[i] > 0) {
			if (landscape[i] < min)
				min = landscape[i];
			if (landscape[i] > max)
				max = landscape[i];
			highlands.push_back(i);
		}
	int highlandsSize = highlands.size();
	for (int i = 0; i < highlandsSize; i++)
		average += (double)landscape[highlands[i]]/highlandsSize;
	printf(" --- average = %g max = %d min = %d number = %d ---\n", average, max, min, highlandsSize);
	max = (max - min)*factor > average ? (max - min)*factor : average;
	while (number > 0) {
		int index = -1, dist, count = 0;
		do {
			index = highlands[rand()%highlandsSize];
			dist = distanceToWater(landscape, mapSize, index, 1);
			if (count++ > 20)
				break;
		} while (landscape[index] < max || dist < length);
		//printf("%d ", dist);
		for (int i = 0; i < 4; i++)
			if (generateRiver(landscape, mapSize, index, size)) {
				printf("dist = %d\n", dist);
				number--;
				break;
			}
	}
	for (int i = 0; i < rivers.size(); i++)
		for (int j = rivers[i].size() - 1; j >= 0; j--)
			landscape[rivers[i][j]] = 0;
}
