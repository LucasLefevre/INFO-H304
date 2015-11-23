#include "database.h"
#include "scorematrix.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[]) {

	//Database* db = new Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	//test
	/*cout << "looking for this potein in database : ";

	Protein & prot = db->getProtein(549645);

	prot.print();
	
	if (db->contains(prot)) {
		cout << "Protein found ! \n";
	}*/
	
	cout << "Loading score matrix\n";
	
	ScoreMatrix* blosum = new ScoreMatrix("BLOSUM62.txt");
	blosum->print();
	cout << blosum->operator()(20,20);
	
	return 0;

}
