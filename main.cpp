#include "database.h"
#include "scorematrix.h"
//#include "protein.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[]) {

	Database* db = new Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	cout << "Loading score matrix\n";
	
	ScoreMatrix* blosum = new ScoreMatrix("BLOSUM62.txt");
	//blosum->print();

	
	Protein newProt;
	newProt.loadFromFile("P00533.fasta");
	
	if (db->contains(newProt)) {
		cout << "Protein found ! \n";
	}
	
	
	//
	
	Protein prot = db->getProtein(0);
	
	vector<vector<int>> D; //distance matrix
	
	//init matrix with correct size and zeros
	int nbrColumns = newProt.size(); 
	int nbrLines =  prot.size();
	D.resize(nbrColumns);
	for (vector<int> line : D) {
		line.assign(nbrLines, 0);
	} 
	
	int openGapPenalty = 11;
	int extensionGapPenalty = 1;
	
	for (int m = 1; m < nbrLines; m++) {
		for (int n = 1; n < nbrColumns; n++) {
			cout << m << " " << n << "\n";
			D[m][n] = 
				D[m-1][n-1] + (*blosum)(0,0);
						
			
		}
	}
	return 0;

}
