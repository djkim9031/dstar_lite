#pragma once
#ifndef DSTAR_H
#define DSTAR_H

#include "dstar_env.h"
#include <gl/freeglut.h>
#include <iostream>
#include <queue>
#include <tuple>
#include <string>

using namespace std;

typedef tuple<float, float, pair<int,int>> Tuple;

class DStar_Lite : Environment {
public:

	DStar_Lite(int nR, int nC, int startRow, int startCol, int termRow, int termCol) : Environment(nC, nR) {
		nRow = nR;
		nCol = nC;
		startCoord.first = startRow;
		startCoord.second = startCol;
		terminalCoord.first = termRow;
		terminalCoord.second = termCol;

		if (startRow > nRow - 1 || startCol > nCol - 1 || termRow > nRow - 1 || termCol > nCol - 1 || nR < 1 || nC < 1) {
			throw std::invalid_argument("Invalid selection of start (or terminal) coordinates!");
		}
		if (startRow == termRow && startCol == termCol) {
			throw std::invalid_argument("Start position and terminal position are identical!");
		}

		k_m = 0;
		SWEEPING_RANGE = 5;
		stuck_flag_count = 0;
		MAX_RETRY = 20; //maximum number of retry when stuck before throwing an exception 

		PathFinder(0, 0);
	};
	
private:

	int nRow, nCol;
	pair<int, int> startCoord;
	pair<int, int> terminalCoord;
	
	//priority_queue<Tuple, vector<Tuple>, std::greater<Tuple>> openList;
	vector<Tuple> openList;
	float k_m;
	int SWEEPING_RANGE;
	int stuck_flag_count;
	int MAX_RETRY;

	void initDStarLite();
	float heuristics(pair<int, int> targetCoord, pair<int, int> currentCoord);
	pair<float, float> calculateKey(pair<int, int> targetCoord, pair<int, int> currentCoord);
	bool topKeyComparison(pair<float, float> currKey, bool popOut);
	void computeShortestPath(pair<int, int> currentCoord);
	void updateGrid(pair<int, int> targetCoord, pair<int, int> currentCoord);
	pair<int, int> nextInShortestPath(pair<int, int> currentCoord);
	bool ScanArea(pair<int, int> currentCoord, int scan_range);
	pair<int, int> MotionAndScan(pair<int, int> currentCoord, int scan_range);
	void PathFinder(int argc, char** argv);
};

#endif // !DSTAR_H
