#include <iostream>

#include "dstar.h"


using namespace std;

int main(int argc, char** argv){

	int numRow, numCol, startRow, startCol, termRow, termCol;
	cout << "----------------------------------" << endl;
	cout << "D* Lite Path Planning" << endl;
	cout << "----------------------------------" << endl;
	cout << "Enter the row and the column size of a map" << endl;
	cout << "Number of Rows: ";
	cin >> numRow;
	cout << "Number of Cols: ";
	cin >> numCol;
	cout << endl << "Enter the row and col of the start position" << endl;
	cout << "Start position row: ";
	cin >> startRow;
	cout << "Start position col: ";
	cin >> startCol;
	cout << endl << "Enter the row and col of the terminal position" << endl;
	cout << "Terminal position row: ";
	cin >> termRow;
	cout << "Terminal position col: ";
	cin >> termCol;
	cout << endl << endl;

	//Test (20, 15, 1, 2, 19, 12)
	DStar_Lite dstar_lite(numRow, numCol, startRow, startCol, termRow, termCol);


	return 0;
}
