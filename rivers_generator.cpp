#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y,  int &finish) {
	if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
		return false;
	if(landscape[y*mapSize + x] <= -1 || landscape[y*mapSize + x] == 0 && rand()%101 > 90) {
		for (int i = x - 1; i <= x + 1; i += 2)
			if (i >= 0 && i < mapSize && landscape[y*mapSize + i] > 0) {
				finish = y*mapSize + i;
				return true;
			}
		for(int i = y - 1; i <= y + 1; i += 2)
			if (i >= 0 && i < mapSize && landscape[i*mapSize + x] > 0) {
				finish = i*mapSize + x;
				return true;
			}
	} else
		return false;
}

inline int distanceToWater(landscapeCell *landscape, int mapSize, int start, int &finish, int step) { //TODO: replace with another function
	int dist = 1, x = start%mapSize, y = start/mapSize;
	while(1) {
		for (int i = x - dist; i <= x + dist; i++)
			if (isWater(landscape, mapSize, i, y - dist, finish) || isWater(landscape, mapSize, i, y + dist, finish))
				return dist;
		for (int i = y - dist + 1; i <= y + dist - 1; i++)
			if (isWater(landscape, mapSize, x - dist, i, finish) || isWater(landscape, mapSize, x + dist, i, finish))
				return dist;
		if (x - dist < 0 && x + x + dist > mapSize && y - dist < 0 && y + dist > mapSize)
			return -1;
		dist += step;
	}
}

inline float factorByLength(int mapSize, int start, int finish) {
	int len = abs(start%mapSize - finish%mapSize) + abs(start/mapSize - finish/mapSize);
	if (len <= 64)
		return 0.5;
	if (len <= 128)
		return 0.1;
	return 0.02;
}

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish) {
	vector<int> river;
	node startNode(start%mapSize, start/mapSize, 0);
	node finishNode(finish%mapSize, finish/mapSize, 0);
	m.Astar(startNode, finishNode, river, factorByLength(mapSize, start, finish), 0);
	printf("River done! Length = %d\n", river.size());
	int length = river.size() - 1;
	for (int i = 0; i <= length; i++) 
		landscape[river[i]] = 0;
	rivers.push_back(river);
}

int neighbourWater(landscapeCell *landscape, int mapSize, int index) {
	int sum = 0;
	if (landscape[index + 1] == 0)
		sum++;
	if (landscape[index - 1] == 0)
		sum++;
	if (landscape[index + mapSize] == 0)
		sum++;
	if (landscape[index - mapSize] == 0)
		sum++;
	return sum;
}

void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int width) {
	vector<int> highlands;
	int len = mapSize*mapSize, max = 0, min = MAX_LANDSCAPE_CELL;
	int *tempMap = new int [len];
	double average = 0.0, factor = 0.3;
	for (int i = 0; i < len; i++) {
		tempMap[i] = landscape[i] > 0 ? landscape[i] : blockedCell;
		if (landscape[i] > 0) {
			if (landscape[i] < min)
				min = landscape[i];
			if (landscape[i] > max)
				max = landscape[i];
			highlands.push_back(i);
		}
	}
	int highlandsSize = highlands.size();
	for (int i = 0; i < highlandsSize; i++)
		average += (double)landscape[highlands[i]]/highlandsSize;
	printf(" --- average = %g max = %d min = %d number = %d ---\n", average, max, min, highlandsSize);
	max = (max - min)*factor > average ? (max - min)*factor : average;
	mapField m(mapSize, tempMap);
	while (number > 0) {
		printf("Making river\n");
		int start = -1, finish = -1, dist, count = 0;
		do {
			start = highlands[rand()%highlandsSize];
			dist = distanceToWater(landscape, mapSize, start, finish, 1);
			if (count++ > 20)
				break;
		} while (landscape[start] < max || dist < length || landscape[finish] > landscape[start]);
		//printf("%d %d, %d %d\n", start, finish, landscape[start], landscape[finish]);
		generateRiverAstar(landscape, mapSize, m, start, finish);
		number--;
	}
	vector<int> intersections;
	for (int i = 0; i < rivers.size(); i++)
		for (int j = 0; j < rivers[i].size(); j++) {
			int index = rivers[i][j];
			if (neighbourWater(landscape, mapSize, index) > 2)
				intersections.push_back(index);
		}
	for (int i = 0; i < rivers.size(); i++) {
		length = rivers[i].size();
		int size = 1, halfsize = 0, totalLength = mapSize*mapSize;
		int newWidth = width <= 1 ? 1 : length/100;
		newWidth = newWidth > width ? width : newWidth;
		newWidth = newWidth <= 1 ? 1 : newWidth;
		for (int j = 0; j < length; j++) { //TODO: river must change landscape
			int index = rivers[i][j], nextIndex = rivers[i][j + 1];
			if (size < newWidth && (j == size*length/newWidth || find (intersections.begin(), intersections.end(), index) != intersections.end()))
				halfsize = ++size/2;
			int direction = abs(index - nextIndex) == 1 ? mapSize : 1;
			for (int k = index - direction*halfsize; k <= index + (size&1 ? direction*halfsize : direction*(halfsize - 1)); k += direction)
				if (k > 0 && k < totalLength)
					landscape[k] = 0;
		}
	}
	delete [] tempMap;
}
