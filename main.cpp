#include "database.h"
#include "scorematrix.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <thread>

using namespace std;

int max_four(int a, int b, int c, int d) {
	//return the max of four integer
	return max(max(a,b), max(c,d));
}

vector<int> bounds(int parts, int size) {
	//separate the sequences in equal parts in order to give the parts to different threads
	int rest = size % parts;
	int larger = (size - rest)/parts;
	vector<int> res;
	
	for (int i = 0; i <= parts; i++) {
		res.push_back(i*larger);	
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

int gotohLinearSpace(const Protein & newProt, const Protein & prot, ScoreMatrix & blosum) {
	//Gotoh algorithm with linear space complexity
	
	const int openGapPenalty = -11;
	const int extensionGapPenalty = -1;
	
	unsigned int n; //length of the shortest sequence
	unsigned int m; //length of the second sequence
	const unsigned int newProtSize = newProt.size();
	const unsigned int protSize = prot.size();
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
	
		Protein & prot = db.getProtein(i); //get protein i from database
		int score = gotohLinearSpace(newProt, prot, blosum); //compute its score
		
		pair<int, int> res (i, score);
		results[i] = res; //store the score associated with protein 'i' in the results vector 
		
		if (i%1000 == 0) { //print progress
			cout << begin << " " << i << endl;
		}
	}
	
	
}


int main(int argc, char* argv[]) {

	//load the database
	Database db = Database("uniprot_sprot/uniprot_sprot.fasta");
	
	cout << "Loading score matrix\n";
	ScoreMatrix blosum = ScoreMatrix("BLOSUM62.txt");
	
	
	//load the protein to align
	Protein newProt;
	newProt.loadFromFile("P00533.fasta");
	cout << "Protein to align : " << endl;
	newProt.print();
		
	//int score = gotohLinearSpace(newProt, prot, blosum);
	//cout << "Score : " << score  << endl;
	
	
	
	
	
	int nbrSequences = db.getNbrSequences();
	
	vector<pair<int, int>> results; //vector with the results of Gotoh algorithm : pair<index of protein in database, score>
	results.resize(nbrSequences);
	
	int nbrThreads = thread::hardware_concurrency(); //get the number of hardware thread available on the machine
	cout << "Running on " << nbrThreads << " threads\n";
	
	vector<int> bnd = bounds(nbrThreads, nbrSequences);  //vector with indexes of proteins, we give the proteins between to indexes (bnd[i] and bnd[i+1]) to one thread
	
	thread * threads = new thread[nbrThreads]; //array that holds the threats
	
	//creating threads
	for(int i = 0; i < nbrThreads; i++) {
		threads[i] = thread(computeGotoh, ref(db), ref(blosum), ref(newProt), ref(results), bnd[i], bnd[i+1]);
	} 
	
	//joining the threads (we wait that all threads are finished)
	for(int i = 0; i < nbrThreads; i++) {
		threads[i].join();
	}
	
	
	sort(results.begin(), results.end(), [](pair<int, int> &left, pair<int, int> &right) {  //sort the vector of results based on score
		return left.second > right.second;
	});
	
	for (int i = 0; i < 10; i++) { // print the first sequence with highest score
		Protein & prot = db.getProtein(results[i].first); 
		cout << "Score : " << results[i].second << " ";
		prot.print("header");
	}
	
	
	//cout << "Score : " << results[113555]  << endl;

	return 0;

}
