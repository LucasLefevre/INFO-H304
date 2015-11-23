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
	matrix.assign(nbrCols * nbrCols, 0);
	string line;
	
	
	if (file.is_open()) {
		
		int value;
		
		int y = 0;
		while (getline(file, line)) {
			
			if (line[0] == '#' || line[0] == ' ') {
				continue;
			}
			int x = 0;
			
			
			stringstream converter; 
			converter << &line[1];
			while (converter >> value){
				
				

				this->operator()(x, y) = (int) value;

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

