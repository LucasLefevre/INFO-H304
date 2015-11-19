#include "scorematrix.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

ScoreMatrix::ScoreMatrix(const string & filename, int nbrCols) {
	
	ifstream file;
	file.open(filename);
	this->nbrCols = nbrCols;
	cout << "size of matrix : " << nbrCols << "\n";
	matrix.resize(nbrCols * nbrCols);
	string line;
	
	
	if (file.is_open()) {
		while (getline(file, line)) {
			cout << "get line : " << line << "\n";
			if (line[0] == '#' || line[0] == ' ') {
				continue;
			}
			
			int value;
			int x = 0;
			int y = 0;
			
			stringstream converter; 
			converter << &line[1];
			while (converter >> value){
				
				cout << "get value : " << value << "\n";
				cout << "test1" << flush << "\n";
				this->operator()(x, y) = value;
				cout << "test2" << flush << "\n";
				x++;
			}
			++y;
		}
		
	}
	else {
		cout << "Unable to load score matrix file : " << filename << "\n";
	}
	
}



int & ScoreMatrix::operator()(int i, int j) {
	/*return element (i,j) of the matrix*/
	return matrix[nbrCols * i + j]; 
}

void ScoreMatrix::print() {
	
	int x; 
	int y;
	
	string s;
	for (int i = 0; i< matrix.size(); i++) {
		cout << matrix[i] << "|";
	} 
	//cout << "matrix : " << s << "\n";
}

