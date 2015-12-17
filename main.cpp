#include "database.h"
#include "scorematrix.h"
#include "threadpool.h"
#include <algorithm>
#include <iostream>
#include <utility>
#include <ctime>


using namespace std;

int max_four(int a, int b, int c, int d) {
	//return the max of four integer
	return max(max(a,b), max(c,d));
}

int gotoh(const Protein & newProt, const Protein & prot, ScoreMatrix & blosum,int R, int Q) {
	
	// R : extension gap penalty
	// Q : sum of open gap penalty and extension gap penalty
	
	unsigned int n; //length of the shortest sequence
	unsigned int m; //length of the second sequence
	
	//choosing the shortest sequence
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

void computeGotoh(Database & db, ScoreMatrix & blosum,  Protein & newProt, vector<pair<int, int>> & results, int protIndex, int R, int Q) {
	// R : extension gap penalty
	// Q : sum of open gap penalty and extension gap penalty
	
	if (protIndex % 1000 == 0) {
		cout << "Computing score for protein : " << protIndex << endl;
	}
	
	Protein & prot = db.getProtein(protIndex); //get protein i from database
	int score = gotoh(newProt, prot, blosum, R, Q); //compute its score
	
	pair<int, int> res (protIndex, score);
	results[protIndex] = res; //store the score associated with protein 'i' in the results vector 
	
}

int main(int argc, char* argv[]) {

	const clock_t begin_time = clock(); //remember the start time
	
	if (argc < 3) {
		cout << "Wrong number of argument\n";
		cout << "usage : gotoh <database> <protein> [blosum score matrix] \n";
		return EXIT_FAILURE;
	}
	char * dbPath = argv[1];
	char * protPath = argv[2];
	
	//load the database
	Database db = Database(dbPath); //"uniprot_sprot_A/uniprot_sprot_A.fasta"
	
	//load the protein to align
	Protein newProt;
	newProt.loadFromFile(protPath); //"P07327.fasta"
	cout << endl;
	cout << endl;
	cout << "Query protein :"<< endl;
	newProt.print();
	cout << endl;
	
	//loading blosum matrix
	ScoreMatrix blosum = ScoreMatrix("BLOSUM62.txt");
	if (argc > 3) {
		cout << "Loading score matrix\n";
		char * blosumPath =argv[3]; 
		blosum = ScoreMatrix(blosumPath);
	}
	
	//OpenGapPenalty
	int openGapPenalty = 11;
	if (argc > 4) {
		openGapPenalty = atoi(argv[4]);
	}
	
	//OpenGapPenalty
	int R = 1;
	if (argc > 5) {
		R = atoi(argv[5]);
	}
	
	int Q = R + openGapPenalty;
	
	int nbrSequences = db.getNbrSequences();
	vector<pair<int, int>> results; //vector with the results of Gotoh algorithm : pair<index of protein in database, score>
	results.resize(nbrSequences);
	
	int nbrThreads = thread::hardware_concurrency(); //get the number of hardware thread available on the machine
	cout << "Running with " << nbrThreads << " threads\n";
	cout << endl;
	
	ThreadPool* pool = new ThreadPool(nbrThreads);  //create the thread pool
	
	//giving jobs to the treadPool
	for (int i = 0; i < nbrSequences; i++) {
		pool->addJob(bind(computeGotoh, ref(db), ref(blosum), ref(newProt), ref(results), i, R, Q));
	}
	
	delete pool; 

	sort(results.begin(), results.end(), [](pair<int, int> &left, pair<int, int> &right) {  //sort the vector of results based on score
		return left.second > right.second;
	});
	
	for (int i = 0; i < 50; i++) { // print the first sequences with highest score
		Protein & prot = db.getProtein(results[i].first); 
		cout << "Score : " << results[i].second << " ";
		prot.print("header");
	}

	cout << endl;
	cout << "Elapsed time : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << "seconds\n";
	return EXIT_SUCCESS;
}
