#include "database.h"
#include "scorematrix.h"
#include "threadpool.h"
#include <algorithm>
#include <iostream>
#include <utility>


using namespace std;

int max_four(int a, int b, int c, int d) {
	//return the max of four integer
	return max(max(a,b), max(c,d));
}


int gotoh(const Protein & newProt, const Protein & prot, ScoreMatrix & blosum) {
	
	
	unsigned int n; //length of the shortest sequence
	unsigned int m; //length of the second sequence
	
	const unsigned int newProtSize = newProt.size();
	const unsigned int protSize = prot.size();
	
	Protein shorterProt;
	Protein longerProt;
	if (newProtSize < protSize) {
		n = newProtSize;
		m = protSize;
		shorterProt = newProt;
		longerProt = prot;
		
	}
	else {
		n = protSize;
		m = newProtSize;
		shorterProt = prot;
		longerProt = newProt;
	}
	
	
	int H[n+1];
	int F[n+1];
	
	int R = 1;
	int Q = 12;
	int H_diag;
	int H_diag_temp;
	int E;
	
	int score = 0;
	
	for (int j = 0; j <= n; j++) {
		H[j] = 0;
		F[j] = 0;
	}
	
	
	for (int i = 1; i <= m; i++) { //line
		
		E = 0;
		H_diag = 0;
		
		for (int j = 1; j <= n; j++) { //column
		
		
			E = max(H[j-1] - Q,  E - R);  
			F[j] = max(H[j] - Q, F[j] - R);
			
			H_diag_temp = H[j];
			
			H[j] = max_four(H_diag + blosum(shorterProt.getResidue(j-1), longerProt.getResidue(i-1)) , E, F[j], 0);
				
			H_diag = H_diag_temp;
			
			if (H[j] > score)
				score = H[j];
			
		}
	}
	
	return score;
}

void computeGotoh(Database & db, ScoreMatrix & blosum,  Protein & newProt, vector<pair<int, int>> & results, int protIndex) {
	
	
	if (protIndex % 1000 == 0) {
		cout << "Computing score for protein : " << protIndex << endl;
	}
	
	Protein & prot = db.getProtein(protIndex); //get protein i from database
	int score = gotoh(newProt, prot, blosum); //compute its score
	
	pair<int, int> res (protIndex, score);
	results[protIndex] = res; //store the score associated with protein 'i' in the results vector 
	
}


int main(int argc, char* argv[]) {

	//load the database
	Database db = Database("uniprot_sprot_A/uniprot_sprot_A.fasta");
	
	cout << "Loading score matrix\n";
	ScoreMatrix blosum = ScoreMatrix("BLOSUM62.txt");
	
	
	
	//load the protein to align
	Protein newProt;
	newProt.loadFromFile("P07327.fasta");
	cout << "Protein to align : " << endl;
	newProt.print();
	
	Protein prot0 = db.getProtein(0);
	cout << "Protein 0 : " << endl;
	prot0.print();
	
	if (newProt == prot0) 
		cout << "Prot found\n";
	
		
	//int score = gotohLinearSpace(newProt, prot, blosum);
	//cout << "Score : " << score  << endl;
	
	int res = 0;
	for (int i = 0 ; i < newProt.size(); i++) {
		res += blosum(newProt.getResidue(i), newProt.getResidue(i));
	}
	cout << "Score should be : " << res << " length : " << newProt.size() << endl;
	
	int nbrSequences = db.getNbrSequences();
	vector<pair<int, int>> results; //vector with the results of Gotoh algorithm : pair<index of protein in database, score>
	results.resize(nbrSequences);
	
	int nbrThreads = thread::hardware_concurrency(); //get the number of hardware thread available on the machine
	cout << "Running with " << nbrThreads << " threads\n";
	
	ThreadPool* pool = new ThreadPool(nbrThreads);
	
	//giving jobs to the treadPool
	cout << "nbr : "<< nbrSequences << endl << flush;
	
	for (int i = 0; i < nbrSequences; i++) {
		pool->addJob(bind(computeGotoh, ref(db), ref(blosum), ref(newProt), ref(results), i));
		
	}
	
	
	delete pool;

	sort(results.begin(), results.end(), [](pair<int, int> &left, pair<int, int> &right) {  //sort the vector of results based on score
		return left.second > right.second;
	});
	
	for (int i = 0; i < 25; i++) { // print the first sequences with highest score
		Protein & prot = db.getProtein(results[i].first); 
		cout << "Score : " << results[i].second << " ";
		prot.print("header");
	}
	
	
	//cout << "Score : " << results[113555]  << endl;

	return 0;

}
