#include "scorematrix.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

ScoreMatrix::ScoreMatrix(const string & filename) {
	
	ifstream file;
	file.open(filename);
	
	string line;
	
	
	if (file.is_open()) {
		while (getline(file, line)) {
		
			if (line[0] == '#' || line[0] == ' ') {
				continue;
			}
			
			int value;
			int x = 0;
			int y = 0;
			stringstream converter; 
			converter << line[1];
			while (converter >> value){
				
				this->operator()(x, y) = value;
				x++;
			}
			y++;
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

