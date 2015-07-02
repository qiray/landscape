#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y,  int &finish) {
	if (x < 0 || y < 0 || x >= mapSize || y >= mapSize)
		return false;
	if(landscape[y*mapSize + x] <= 0) {
		for (int i = x - 1; i <= x + 1; i += 2)
			if (i >= 0 && i < mapSize && landscape[y*mapSize + i] > 0) {
				//printf("!!!x: diff = %d %d\n", i, y);
				finish = y*mapSize + i;
				return true;
			}
		for(int i = y - 1; i <= y + 1; i += 2)
			if (i >= 0 && i < mapSize && landscape[i*mapSize + x] > 0) {
				//printf("!!!y: diff = %d %d\n", x, i);
				finish = i*mapSize + x;
				return true;
			}
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

void midpoint(int mapSize, int *vector, int left, int right, int len, float r) {
    if (right - left < 2)
        return;
    int hl = vector[left], leftx = hl%mapSize, lefty = hl/mapSize;
    int hr = vector[right], rightx = hr%mapSize, righty = hr/mapSize;
    int xdiff = abs(leftx - rightx), ydiff = abs(lefty - righty);
    int xresult = (leftx + rightx)/2 + minMaxRandom(-r*xdiff, r*xdiff);
    int yresult = (lefty + righty)/2 + minMaxRandom(-r*ydiff, r*ydiff);
    while (xresult < 0 || xresult >= mapSize || yresult < 0 || yresult >= mapSize) {
       xresult = (leftx + rightx)/2 + minMaxRandom(-r*xdiff, r*xdiff);
       yresult = (lefty + righty)/2 + minMaxRandom(-r*ydiff, r*ydiff);
    }
    int h = yresult*mapSize + xresult;
    printf("new: x = %d y = %d\n", xresult, yresult);
    int index = left + (right - left)/2; //ищем середину
    vector[index] = h;
    //выполняем алгоритм для получившихся половин
    midpoint(mapSize, vector, left, index, len / 2, r);
    midpoint(mapSize, vector, index, right, len / 2, r);
}

void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish) {
	int len = pow(2, 0);
	int heights[len + 1];
	heights[0] = start;
	heights[len] = finish;
	printf("finish: x = %d y = %d\n", finish%mapSize, finish/mapSize);
	float midpointFactor = 0.9, astarFactor = 0.3;
	midpoint(mapSize, heights, 0, len, len, midpointFactor);
	vector<int> river, temp;
	for (int i = 0; i < len; i++) {
		node startNode(heights[i]%mapSize, heights[i]/mapSize, 0);
		node finishNode(heights[i + 1]%mapSize, heights[i + 1]/mapSize, 0);
		m.Astar(startNode, finishNode, temp, astarFactor, 0);
		river.insert(river.end(), temp.begin(), temp.end());
	}
	printf("River done! Length = %d\n", river.size());
	for (int i = 0; i < river.size(); i++) //TODO: river must change landscape
		landscape[river[i]] = 0;
	//rivers.push_back(river);
	
	/*node startNode(start%mapSize, start/mapSize, 0);
	node finishNode(finish%mapSize, finish/mapSize, 0);
	printf("start: x = %d y = %d, finish: x = %d, y = %d\n",start%mapSize, start/mapSize,finish%mapSize, finish/mapSize);
	vector<int> river;
	if (m.Astar(startNode, finishNode, river, 0.3, 0)) {
		rivers.push_back(river);
		printf("River done! Length = %d\n", river.size());
	}*/
}

void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int size) {
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
	//generateRiverAstar(landscape, mapSize, m, 0*mapSize + 10, 25*mapSize + 10);
	//generateRiverAstar(landscape, mapSize, m, 181*mapSize + 139, 17*mapSize + 14);
	//generateRiverAstar(landscape, mapSize, m, 261*mapSize + 57, 17*mapSize + 14);
	while (number > 0) {
		printf("Making river\n");
		int start = -1, finish = -1, dist, count = 0;
		do {
			start = highlands[rand()%highlandsSize];
			dist = distanceToWater(landscape, mapSize, start, finish, 1);
			if (count++ > 20)
				break;
		} while (landscape[start] < max || dist < length);
		//printf("%d %d, %d %d\n", start, finish, landscape[start], landscape[finish]);
		generateRiverAstar(landscape, mapSize, m, start, finish);
		number--;
	}
	for (int i = 0; i < rivers.size(); i++)
		for (int j = rivers[i].size() - 1; j >= 0; j--)
			landscape[rivers[i][j]] = 0;
	delete [] tempMap;
}
