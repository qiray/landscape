#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

inline bool checkNeighbour(landscapeCell *landscape, vector<int> &list, bool cond, int index, int &inc, int *map, int dist, int finish) {
	if (index == finish) {
		list.push_back(index);
		inc++;
		map[index] = dist;
		return true;
	}
	if (cond && landscape[index] > 0 && map[index] == -1) {
		list.push_back(index);
		inc++;
		map[index] = dist;
	}
	return false;
}

inline bool checkWay(bool cond, int *map, int index, int &result, vector<int> &river, int &dist) {
	if(cond && map[index] == map[result] - 1) {
		result = index;
		dist--;
		river.push_back(index);
		return true;
	}
	return false;
}

bool generateRiver(landscapeCell *landscape, int mapSize, int start, int finish, int size) {
	vector<int> river, list;
	int dist = 0, index = start, listIndex = 0, listSize = 1, inc = 1;
	bool flag = false;
	int *map = new int[mapSize*mapSize];
	for (int i = mapSize*mapSize; i >= 0; i--)
		map[i] = -1;
	map[index] = dist;
	list.push_back(index);
	while (!flag && inc > 0) {
		inc = 0;
		int index_inc = 0;
		for (int i = listIndex; i < listSize; i++) {
			int x = list[i]%mapSize, y = list[i]/mapSize;
			index_inc++;
			if (checkNeighbour(landscape, list, x - 1 >= 0, y*mapSize + x - 1, inc, map, dist + 1, finish) ||
			    checkNeighbour(landscape, list, x + 1 < mapSize, y*mapSize + x + 1, inc, map, dist + 1, finish) ||
			    checkNeighbour(landscape, list, y - 1 >= 0, (y - 1)*mapSize + x, inc, map, dist + 1, finish) ||
			    checkNeighbour(landscape, list, y + 1 < mapSize, (y + 1)*mapSize + x, inc, map, dist + 1, finish)) {
				flag = true;
				break;
			}
		}
		listIndex += index_inc;
		listSize += inc;
		dist++;
	}
	if (flag) { //river done
		index = finish;
		dist = map[finish];
		while(index != start) {
			int x = index%mapSize, y = index/mapSize;
			if (checkWay(x - 1 >= 0, map, y*mapSize + x - 1, index, river, dist) ||
			    checkWay(x + 1 < mapSize, map, y*mapSize + x + 1, index, river, dist) ||
			    checkWay(y - 1 >= 0, map, (y - 1)*mapSize + x, index, river, dist) ||
			    checkWay(y + 1 < mapSize, map, (y + 1)*mapSize + x, index, river, dist)) {
				continue;
			}
		}
		rivers.push_back(river);
		printf("River done! Length = %d\n", river.size());
	}
	delete [] map;
	return flag;
}

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y, int &finish) {
	if (x < 0 || y < 0 || x > mapSize || y > mapSize)
		return false;
	if (0 || landscape[y*mapSize + x] <= 0) {
		finish = y*mapSize + x;
		return true;
	} else
		return false;
}

inline int distanceToWater(landscapeCell *landscape, int mapSize, int start, int &finish, int step) {
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
		int start = -1, finish = -1, dist, count = 0;
		do {
			start = highlands[rand()%highlandsSize];
			dist = distanceToWater(landscape, mapSize, start, finish, 1);
			if (count++ > 20)
				break;
		} while (landscape[start] < max || dist < length);
		generateRiver(landscape, mapSize, start, finish, size);
		number--;
	}
	for (int i = 0; i < rivers.size(); i++)
		for (int j = rivers[i].size() - 1; j >= 0; j--)
			landscape[rivers[i][j]] = 0;
}
