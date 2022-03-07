#pragma once
#ifndef DSTAR_ENV_H
#define DSTAR_ENV

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>

using namespace std;

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(
		const pair<T1, T2>& x)
		const {
		return x.first ^ x.second;
	}
};

struct grid {
public:
	grid(string id_) :id(id_), g(FLT_MAX), rhs(FLT_MAX), status(0) {};
	
	string id;
	unordered_map<pair<int,int>, float, hash_pair> preds;
	unordered_map<pair<int,int>, float, hash_pair> succs;
	float g;
	float rhs;
	float status;
	

};


class Environment {

protected:
	int x_dim;
	int y_dim;
	vector<vector<grid>> cells;
	float linearCost = 1.0;
	float diagonalCost = sqrt(2.0);

	Environment(int x_dim_, int y_dim_) :x_dim(x_dim_), y_dim(y_dim_) { generateCells(); };

private:
	void generateCells();

};



#endif // !DSTAR_ENV_H

