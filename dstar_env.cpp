#include "dstar_env.h"
#include <string>

void Environment::generateCells() {

	for (int j = 0; j < y_dim; ++j) {//row
		vector<grid> colGrids;
		for (int i = 0; i < x_dim; ++i) {//col
			string id = "R" + to_string(j) + ",C" + to_string(i);
			grid currGrid(id);

			if (j > 0) {
				currGrid.preds[pair<int, int>(j - 1, i)] = linearCost;
				currGrid.succs[pair<int, int>(j - 1, i)] = linearCost;
			}
			if (j + 1 < y_dim) {
				currGrid.preds[pair<int, int>(j + 1, i)] = linearCost;
				currGrid.succs[pair<int, int>(j + 1, i)] = linearCost;
			}
			if (i > 0) {
				currGrid.preds[pair<int, int>(j, i - 1)] = linearCost;
				currGrid.succs[pair<int, int>(j, i - 1)] = linearCost;
			}
			if (i + 1 < x_dim) {
				currGrid.preds[pair<int, int>(j, i + 1)] = linearCost;
				currGrid.succs[pair<int, int>(j, i + 1)] = linearCost;
			}
			if (j > 0 && i + 1 < x_dim) {
				currGrid.preds[pair<int, int>(j - 1, i + 1)] = diagonalCost;
				currGrid.succs[pair<int, int>(j - 1, i + 1)] = diagonalCost;
			}
			if (j + 1 < y_dim && i + 1 < x_dim) {
				currGrid.preds[pair<int, int>(j + 1, i + 1)] = diagonalCost;
				currGrid.succs[pair<int, int>(j + 1, i + 1)] = diagonalCost;
			}
			if (j + 1 < y_dim && i > 0) {
				currGrid.preds[pair<int, int>(j + 1, i - 1)] = diagonalCost;
				currGrid.succs[pair<int, int>(j + 1, i - 1)] = diagonalCost;
			}
			if (j > 0 && i > 0) {
				currGrid.preds[pair<int, int>(j - 1, i - 1)] = diagonalCost;
				currGrid.succs[pair<int, int>(j - 1, i - 1)] = diagonalCost;
			}

			colGrids.push_back(currGrid);
		}
		cells.push_back(colGrids);
	}

}