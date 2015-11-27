#include "database.h"
#include "scorematrix.h"
//#include "protein.h"
#include <iostream>

using namespace std;

int max_four(int a, int b, int c, int d) {
	return max(max(a,b), max(c,d));
}

int main(int argc, char* argv[]) {

	Database* db = new Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	cout << "Loading score matrix\n";
	
	ScoreMatrix* blosum = new ScoreMatrix("BLOSUM62.txt");
	//blosum->print();

	
	Protein newProt;
	newProt.loadFromFile("P00533.fasta");
	cout << "Protein to test : " << endl;
	newProt.print();
	
	if (db->contains(newProt)) {
		cout << "Protein found ! \n";
	}
	cout << "********************\n" << flush;
	
	
	//
	
	Protein prot = db->getProtein(113555); //this is the P00533 protein from the database
	prot.print();
	cout << "********************\n" << flush;
	
	
	//Algorithm
	
	//vector<pair<int, int>> results;
	
	
	//init matrix with correct size and zeros
	int openGapPenalty = -11;
	int extensionGapPenalty = -1;
	
	int nbrColumns = newProt.size(); 
	int nbrLines =  prot.size();
	
	cout << "nbrLines : " << nbrLines << " | nbrColumns : " << nbrColumns << endl;
	
	vector<vector<int>> D (nbrLines, vector<int>(nbrColumns)); //distance matrix
	
	vector<vector<int>> P (nbrLines, vector<int>(nbrLines)); 
	vector<vector<int>> Q (nbrLines, vector<int>(nbrColumns)); 

	int score = 0;
	
	for (int i = 0; i < nbrLines; i++) {
		D[i][0] = 0;
		P[i][0] = 0;
		Q[i][0] = 0;
	}
	
	for (int i = 0; i < nbrColumns; i++) {
		D[0][i] = 0;
		P[0][i] = 0;
		Q[0][i] = 0;
	}
	
	for (int i = 1; i < nbrLines; i++) {
		for (int j = 1; j < nbrColumns; j++) {
			
			//cout << i << "|" << j <<endl;
			P[i][j] = max(D[i-1][j] + openGapPenalty, P[i-1][j] + extensionGapPenalty);
			Q[i][j] = max(D[i][j-1] + openGapPenalty, P[i][j-1] + extensionGapPenalty);
			
			D[i][j] = max_four(
						0,
						D[i-1][j-1] + (*blosum)(prot.getResidue(i), newProt.getResidue(j)),
						P[i][j],
						Q[i][j]
			);
			
			if (D[i][j] > score) {
				score = D[i][j];
			} 
			
		}
	}
	
	cout << "Score : " << score << endl;

	return 0;

}
