#include "database.h"
#include "scorematrix.h"
#include <algorithm>
//#include "protein.h"
#include <iostream>
#include <utility>
#include <thread>

using namespace std;

int max_four(int a, int b, int c, int d) {
	return max(max(a,b), max(c,d));
}

vector<int> bounds(int parts, int size) {
	
	int rest = size % parts;
	int larger = (size - rest)/parts;
	cout << "larger : " << larger << endl; 
	cout << "rest : " << rest << endl;
	vector<int> res;
	
	
	for (int i = 0; i <= parts; i++) {
		res.push_back(i*larger);
		cout << "i : " << res[i] << endl; 
	}
	res[parts-1] += rest; 
	
	return res;
	
}

int gotoh(Protein & newProt, Protein & prot, ScoreMatrix & blosum) {
	//Gotoh algorithm with quadratic space complexity
	
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

int gotohLinearSpace(Protein & newProt, Protein & prot, ScoreMatrix & blosum) {
	//Gotoh algorithm with linear space complexity
	
	int openGapPenalty = -11;
	int extensionGapPenalty = -1;
	
	unsigned int n; //length of the shortest sequence
	unsigned int m; //length of the second sequence
	unsigned int newProtSize = newProt.size();
	unsigned int protSize = prot.size();
	/*if (newProtSize < protSize) {
		n = newProtSize;
		m = protSize;
	}
	else {
		
	}*/
	n = protSize;
	m = newProtSize;
	
	
	unsigned int CC[n];
	unsigned int DD[n];
	int e;
	int c;
	int s;
	int t;
	
	CC[0] = 0;
	t = openGapPenalty;
	
	for (int j = 1; j < n; j++) {
		t = t + extensionGapPenalty;
		CC[j] = t;
		DD[j] = t + openGapPenalty;
	}
	
	t = openGapPenalty;
	
	for (int i = 1; i < m; i++) {
		
		s = CC[0];
		t = t + extensionGapPenalty;
		c = t;
		CC[0] = c;
		e = t + openGapPenalty;
		
		for (int j = 1; j < n; j++) {
			//cout << "(" << j << ", " << i << ") " << flush;
			e = max(e, c + openGapPenalty) + extensionGapPenalty;
			DD[j] = max(DD[j], CC[j] + openGapPenalty) + extensionGapPenalty;
			c = max_four(DD[j], e, s + blosum(prot.getResidue(j), newProt.getResidue(i)), 0);
			s = CC[j];
			CC[j] = c;
		}
	}
	
	return c;
	
}
void computeGotoh(Database & db, ScoreMatrix & blosum,  Protein & newProt, vector<pair<int, int>> & results, int begin, int end) {
	for (int i = begin; i < end; i++) {
	
		Protein & prot = db.getProtein(i); 
		//cout << i << " " << prot.size() <<"\n" << flush;
		//prot.print();
		//cout << "size : " << prot.size() << endl;
		int score = gotohLinearSpace(newProt, prot, blosum); 
		//cout << i << \n" ;
		
		pair<int, int> res (i, score);
		results[i] = res;
		
		if (i%1000 == 0) {
			cout << begin << " " << i << endl;
		}
	}
	
	
}


int main(int argc, char* argv[]) {

	Database db = Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	cout << "Loading score matrix\n";
	
	ScoreMatrix blosum = ScoreMatrix("BLOSUM62.txt");
	//blosum->print();

	
	
	
	
	
	
	//
	
	
	Protein newProt;
	newProt.loadFromFile("P00533.fasta");
	if (db.contains(newProt)) {
		cout << "Protein found ! \n";
	}
	cout << "********************\n" << flush;
	
	
	cout << "Protein to test : " << endl;
	newProt.print();

	//cout << "dsfsé" << endl << flush;
	
	/*Protein & prot = db.getProtein(1933);
	
	if (db.contains(prot)) {
		cout << "Protein found ! \n";
	}*/
	cout << "********************\n" << flush;
	
	//prot.print();
	//cout << "size : " << prot.size() << endl;
	
		
	//int score = gotohLinearSpace(newProt, prot, blosum);
	//cout << "Score : " << score  << endl;
	
	
	
	
	
	int nbrSequences = db.getNbrSequences();
	vector<pair<int, int>> results;
	results.resize(nbrSequences);
	
	int nbrThreads = thread::hardware_concurrency();
	cout << "Running on " << nbrThreads << " threads\n";
	
	vector<int> bnd = bounds(nbrThreads, nbrSequences);  
	
	thread * threads = new thread[nbrThreads];
	
	//creating threads
	for(int i = 0; i < nbrThreads; i++) {
		threads[i] = thread(computeGotoh, ref(db), ref(blosum), ref(newProt), ref(results), bnd[i], bnd[i+1]);
	} 
	
	//joining the threads (wait)
	for(int i = 0; i < nbrThreads; i++) {
		threads[i].join();
	}
	
	
	sort(results.begin(), results.end(), [](pair<int, int> &left, pair<int, int> &right) {
		return left.second > right.second;
	});
	
	for (int i = 0; i < 10; i++) {
		Protein & prot = db.getProtein(results[i].first); 
		cout << "Score : " << results[i].second << " ";
		prot.print("header");
	}
	
	
	//cout << "Score : " << results[113555]  << endl;

	return 0;

}
