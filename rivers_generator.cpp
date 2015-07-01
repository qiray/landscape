#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "rivers_generator.h"

inline bool isWater(landscapeCell *landscape, int mapSize, int x, int y,  int &finish) {
	if (x < 0 || y < 0 || x > mapSize || y > mapSize)
		return false;
	if(landscape[y*mapSize + x] <= 0) {
		for (int i = x - 1; i <= x + 1; i++)
			for(int j = y - 1; j <= y + 1; j++)
				if (i >= 0 && j >= 0 && i < mapSize && j < mapSize && landscape[j*mapSize + i] > 0) {
					finish = j*mapSize + i;
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
/*
// vector — массив точек, изначально заполнены только первая и последняя
// left, right — индексы первого и последнего элемента
// len = длина изначального отрезка(в пикселах)
// r -  шероховатость. Чем меньше тем более плавная кривая будет в результате.
function midpoint(vector, left, right, len, r) {
    if (right - left < 2)
        return;
    var hl = vector[left]; //высота левой точки
    var hr = vector[right]; //высота правой
    var h = (hl + hr) / 2 + rand(-r * len, +r * len); //считаем высоту
    var index = Math.floor(left + (right - left) / 2); //ищем середину
    vector[index] = h;
    //выполняем алгоритм для получившихся половин
    midpoint(vector, left, index, len / 2, r);
    midpoint(vector, index, right, len / 2, r);
}
*/
void generateRiverAstar(landscapeCell *landscape, int mapSize, mapField &m, int start, int finish) {
	node startNode(start%mapSize, start/mapSize, 0);
	node finishNode(finish%mapSize, finish/mapSize, 0);
	printf("start: x = %d y = %d, finish: x = %d, y = %d\n",start%mapSize, start/mapSize,finish%mapSize, finish/mapSize);
	vector<int> river;
	if (m.Astar(startNode, finishNode, river, 0)) {
		rivers.push_back(river);
		printf("River done! Length = %d\n", river.size());
	}
}

void generateRivers(landscapeCell *landscape, int mapSize, int number, int length, int size) {
	vector<int> highlands;
	int len = mapSize*mapSize, max = 0, min = MAX_LANDSCAPE_CELL;
	int *tempMap = new int [len];
	double average = 0.0, factor = 0.5;
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
