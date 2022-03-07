#include "dstar.h"

void DStar_Lite::initDStarLite() {
	
	cells[terminalCoord.first][terminalCoord.second].rhs = 0;
	pair<float, float> keys = calculateKey(terminalCoord, startCoord);
	openList.push_back({ keys.first, keys.second, terminalCoord });
	sort(openList.begin(), openList.end());
	computeShortestPath(startCoord);
}

float DStar_Lite::heuristics(pair<int, int> targetCoord, pair<int, int> currentCoord) {
	return sqrtf(powf(targetCoord.first - currentCoord.first, 2) + powf(targetCoord.second - currentCoord.second, 2));
}

pair<float, float> DStar_Lite::calculateKey(pair<int, int> targetCoord, pair<int, int> currentCoord) {
	pair<float, float> keys;
	keys.first = min(cells[targetCoord.first][targetCoord.second].g, cells[targetCoord.first][targetCoord.second].rhs) + heuristics(targetCoord, currentCoord) + k_m;
	keys.second = min(cells[targetCoord.first][targetCoord.second].g, cells[targetCoord.first][targetCoord.second].rhs);
	return keys;
}

bool DStar_Lite::topKeyComparison(pair<float, float> currKey, bool popOut) {
	float K1, K2;
	if (!openList.empty()) {
		const Tuple& topKey = openList[0];
		K1 = get<0>(topKey);
		K2 = get<1>(topKey);
		pair<int, int> coords = get<2>(topKey);
		if (popOut) {
			openList.erase(openList.begin());
		}

	}
	else {
		K1 = K2 = FLT_MAX;
	}



	if (K1 < currKey.first) {
		return true;
	}
	else if (K1 > currKey.first) {
		return false;
	}
	else {
		if (K2 < currKey.second) {
			return true;
		}
	}

	return false;

}

void DStar_Lite::computeShortestPath(pair<int, int> currentCoord) {

	while (cells[currentCoord.first][currentCoord.second].rhs != cells[currentCoord.first][currentCoord.second].g || topKeyComparison(calculateKey(currentCoord, currentCoord), false)) {


		const Tuple& topKey = openList[0];
		float K1 = get<0>(topKey);
		float K2 = get<1>(topKey);
		pair<int, int> U = get<2>(topKey);
		//cout << "[" << U.first << "," << U.second << "]: " << K1 << ", " << K2 << endl;
		if (topKeyComparison(calculateKey(U, currentCoord), true)) {
			pair<float, float> keys = calculateKey(U, currentCoord);
			openList.push_back({ keys.first, keys.second, U });
			sort(openList.begin(), openList.end());
		}
		else if (cells[U.first][U.second].g > cells[U.first][U.second].rhs) {
			cells[U.first][U.second].g = cells[U.first][U.second].rhs;
			for (auto it = cells[U.first][U.second].preds.begin(); it != cells[U.first][U.second].preds.end(); ++it) {
				updateGrid(it->first, currentCoord);
			}
		}
		else {
			cells[U.first][U.second].g = FLT_MAX;
			updateGrid(U, currentCoord);
			for (auto it = cells[U.first][U.second].preds.begin(); it != cells[U.first][U.second].preds.end(); ++it) {
				updateGrid(it->first, currentCoord);
			}
		}
	}

}


void DStar_Lite::updateGrid(pair<int, int> targetCoord, pair<int, int> currentCoord) {
	if (targetCoord != terminalCoord) {
		float min_rhs = FLT_MAX;
		for (auto it = cells[targetCoord.first][targetCoord.second].succs.begin(); it != cells[targetCoord.first][targetCoord.second].succs.end(); ++it) {
			min_rhs = min(min_rhs, cells[it->first.first][it->first.second].g + it->second);
		}

		cells[targetCoord.first][targetCoord.second].rhs = min_rhs;
	}

	vector<pair<int, int>> gridInOpenList;
	for (int i = 0; i < openList.size(); ++i) {
		const Tuple& vals = openList[i];
		if (get<2>(vals) == targetCoord) {
			gridInOpenList.push_back(targetCoord);
		}
	}

	if (gridInOpenList.size() > 0) {
		if (gridInOpenList.size() > 1) {
			throw std::invalid_argument("More than one targetCoord in the queue!");
		}
		for (int i = 0; i < openList.size(); ++i) {
			const Tuple& vals = openList[i];
			if (get<2>(vals) == gridInOpenList[0]) {
				openList.erase(openList.begin() + i);
			}
		}
	}

	if (cells[targetCoord.first][targetCoord.second].rhs != cells[targetCoord.first][targetCoord.second].g) {
		pair<float, float> keys = calculateKey(targetCoord, currentCoord);
		openList.push_back({ keys.first,keys.second,targetCoord });
		sort(openList.begin(), openList.end());
	}

}

pair<int, int> DStar_Lite::nextInShortestPath(pair<int, int> currentCoord) {
	float min_rhs = FLT_MAX;
	pair<int, int> s_next(-1, -1);
	if (cells[currentCoord.first][currentCoord.second].rhs == FLT_MAX) {
		cout << "You are stuck at the moment, Let's explore some more to see if there's a new opening" << endl;
		stuck_flag_count++;
		if (stuck_flag_count == MAX_RETRY) {
			cout << "It seems there's no way out" << endl;
			throw;
		}
		else {
			return currentCoord;
		}
	}
	else {
		for (auto it = cells[currentCoord.first][currentCoord.second].succs.begin(); it != cells[currentCoord.first][currentCoord.second].succs.end(); ++it) {
			float child_cost = cells[it->first.first][it->first.second].g + it->second;

			if (child_cost < min_rhs) {
				min_rhs = child_cost;
				s_next = it->first;
			}
		}
		if (s_next.first >= 0 && s_next.second >= 0) {

			return s_next;
		}
		throw std::invalid_argument("Could not find child for transition!");

	}

}

bool DStar_Lite::ScanArea(pair<int, int> currentCoord, int scan_range) {

	int range_checked = 0;
	unordered_map<pair<int, int>, float, hash_pair> states_to_update;
	if (scan_range >= 1) {
		for (auto it = cells[currentCoord.first][currentCoord.second].succs.begin(); it != cells[currentCoord.first][currentCoord.second].succs.end(); ++it) {
			states_to_update[it->first] = cells[it->first.first][it->first.second].status;
		}
		range_checked = 1;
	}

	while (range_checked < scan_range) {
		unordered_map<pair<int, int>, float, hash_pair> new_set;
		for (auto it = states_to_update.begin(); it != states_to_update.end(); ++it) {
			new_set[it->first] = it->second;
			for (auto it2 = cells[it->first.first][it->first.second].succs.begin(); it2 != cells[it->first.first][it->first.second].succs.end(); ++it2) {
				if (new_set.find(it2->first) == new_set.end()) {
					new_set[it2->first] = cells[it2->first.first][it2->first.second].status;
				}
			}
		}
		range_checked += 1;
		states_to_update = new_set;
	}

	bool new_obstacle = false;
	for (auto it = states_to_update.begin(); it != states_to_update.end(); ++it) {
		if (it->second < 0) {
			//found a cell with obstacle
			for (auto it2 = cells[it->first.first][it->first.second].succs.begin(); it2 != cells[it->first.first][it->first.second].succs.end(); ++it2) {

				if (it2->second != FLT_MAX) {
					//first time to observe this obstacle that wasn't there before
					cells[it->first.first][it->first.second].status = -2;
					cells[it2->first.first][it2->first.second].succs[it->first] = FLT_MAX;

					cells[it->first.first][it->first.second].succs[it2->first] = FLT_MAX;
					updateGrid(it->first, currentCoord);
					new_obstacle = true;
				}
			}
		}
		else if(it->second==1) {
			//detected an obstacle clearance
			for (auto it2 = cells[it->first.first][it->first.second].succs.begin(); it2 != cells[it->first.first][it->first.second].succs.end(); ++it2) {
				if (it2->second == FLT_MAX) {
					//This is the previous obstacle cell that is cleared within the sweeping range
					cells[it->first.first][it->first.second].status = 0;
					cells[it2->first.first][it2->first.second].succs[it->first] = heuristics(it->first,it2->first);

					cells[it->first.first][it->first.second].succs[it2->first] = heuristics(it->first, it2->first);
					updateGrid(it->first, currentCoord);
				}
			}
		}
	}
	return new_obstacle;
}



pair<int, int> DStar_Lite::MotionAndScan(pair<int, int> currentCoord, int scan_range) {
	if (currentCoord == terminalCoord) {
		return terminalCoord;
	}

	pair<int, int> s_new = nextInShortestPath(currentCoord);
	if (cells[s_new.first][s_new.second].status == -1) {
		//Just ran into new obstacle
		s_new = currentCoord;
	}

	bool result = ScanArea(s_new, scan_range);
	if (result) {
		k_m += heuristics(currentCoord, s_new);
		computeShortestPath(currentCoord);
	}

	return s_new;
}

