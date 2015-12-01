#include "database.h"
#include "scorematrix.h"
//#include "protein.h"
#include <iostream>
#include <utility>

using namespace std;

int max_four(int a, int b, int c, int d) {
	return max(max(a,b), max(c,d));
}

int gotoh(Protein & newProt, Protein & prot, ScoreMatrix & blosum) {
	
	
	//init matrix with correct size and zeros
	int openGapPenalty = -11;
	int extensionGapPenalty = -1;
	
	int nbrColumns = newProt.size(); 
	int nbrLines =  prot.size();
	
	vector<vector<int>> D (nbrLines, vector<int>(nbrColumns)); 
	
	vector<vector<int>> P (nbrLines, vector<int>(nbrColumns)); 
	vector<vector<int>> Q (nbrLines, vector<int>(nbrColumns)); 

	int score = 0;
	
	/*for (int i = 0; i < nbrLines; i++) {
		D[i][0] = 0;
		P[i][0] = 0;
		Q[i][0] = 0;
	}
	
	for (int i = 0; i < nbrColumns; i++) {
		D[0][i] = 0;
		P[0][i] = 0;
		Q[0][i] = 0;
	}*/
	
	for (int i = 1; i < nbrLines; i++) {
		for (int j = 1; j < nbrColumns; j++) {
			
			//cout << i << "|" << j <<endl;
			P[i][j] = max(D[i-1][j] + openGapPenalty, P[i-1][j] + extensionGapPenalty);
			Q[i][j] = max(D[i][j-1] + openGapPenalty, P[i][j-1] + extensionGapPenalty);
			
			D[i][j] = max_four(
						0,
						D[i-1][j-1] + blosum(prot.getResidue(i), newProt.getResidue(j)),
						P[i][j],
						Q[i][j]
			);
			
			if (D[i][j] > score) {
				score = D[i][j];
			} 
			
		}
	}
	
	return score;
}

int main(int argc, char* argv[]) {

	Database db = Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	cout << "Loading score matrix\n";
	
	ScoreMatrix blosum = ScoreMatrix("BLOSUM62.txt");
	//blosum->print();

	
	
	
	/*if (db->contains(newProt)) {
		cout << "Protein found ! \n";
	}
	cout << "********************\n" << flush;*/
	
	
	//
	
	
	
	
	//Algorithm
	
	
	
	//init matrix with correct size and zeros
	/*int openGapPenalty = -11;
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
	}*/
	
	Protein newProt;
	newProt.loadFromFile("P00533.fasta");
	cout << "Protein to test : " << endl;
	newProt.print("header");

	
	
	/*Protein & prot = db.getProtein(11355);
	int score = gotoh(newProt, prot, blosum);*/
	 
	
	
	
	int nbrSequences = db.getNbrSequences();
	vector<int> results;
	results.reserve(nbrSequences);
	
	for (int i = 0; i < nbrSequences; i++) {
		
		Protein & prot = db.getProtein(i); 
		int score = gotoh(newProt, prot, blosum); 
		cout << i << " | " ;
		results.push_back(1);
	}
	
	
	
	cout << "Score : " << results[113555]  << endl;

	return 0;

}
