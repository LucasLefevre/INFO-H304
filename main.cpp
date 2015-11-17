#include "database.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[]) {

	Database* db = new Database("uniprot_sprot/uniprot_sprot.fasta");
	
	
	//test
	cout << "looking for this potein in database : ";

	Protein & prot = db->getProtein(549644);

	prot.print();
	
	if (db->contains(prot)) {
		cout << "Protein found ! \n";
	}
	
	return 0;

}
