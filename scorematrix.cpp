#include "scorematrix.h"
#include "codeTable.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

ScoreMatrix::ScoreMatrix(const string & filename, int nbrCols) {
	
	ifstream file;
	file.open(filename);
	this->nbrCols = nbrCols;
	cout << "size of matrix : " << nbrCols << "\n";
	matrix.assign(27 * 27, -1000);
	string line;
	
	CodeTable coder = CodeTable();
	
	if (file.is_open()) {
		
		int value;
		 
		string residues = "";
		int y = 0;
		while (getline(file, line)) {
			
			if (line[0] == '#') {
				continue;
			}
			else if (line[0] == ' ') {
				//loading residues in the order of the score matrix file
				stringstream converter;
				converter << &line[1];
				
				
				char residue;
				while (converter >> residue) {
					residues += residue;
				}
				continue;
			}
			int x = 0;
			

			stringstream converter;
			converter << &line[1];
			while (converter >> value){
				
				if (x < residues.size() && y < residues.size()) {
					
					int residue_x = coder.encode(residues[x]); 
					int residue_y = coder.encode(residues[y]); 
					
					this->operator()(residue_x, residue_y) = (int) value;

					x++;
				}
				
			}
			++y;
		}
		
	}
	else {
		cout << "Unable to load score matrix file : " << filename << endl;
		throw string("Unable to load score matrix file");
	}
	
}


int & ScoreMatrix::operator()(char i, char j) {
	//return element (i,j) of the matrix
	
	return matrix[nbrCols * i + j]; 
}

void ScoreMatrix::print() {
	
	int x; 
	int y;
	
	string s;
	for (int i = 0; i< matrix.size(); i++) {
		if (i % nbrCols == 0) {
			cout << endl;
		}
		cout << matrix[i] << "|";
	} 
	//cout << "matrix : " << s << "\n";
	cout << "\n";
}

