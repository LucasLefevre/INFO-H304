#include "protein.h"
#include "codeTable.h"
#include <map>


using namespace std;

Protein::Protein() {
	
}

Protein::Protein(vector<unsigned int> sequence, string header) {
	this->sequence = sequence;
	this->header = header;
}

Protein::~Protein() {}

void Protein::setSequence(vector<unsigned int> sequence) {
	this->sequence = sequence;
}

vector<unsigned int> Protein::getSequence() const {
	return sequence;
}

void Protein::setHeader(string header) {
	this->header = header;
}

string Protein::decode() {
	
	
	CodeTable coder = CodeTable();
	
	string decodedSequence = "";
	
	for (const auto & residue : sequence) {
		decodedSequence += coder.decode(residue);
		//cout << hex << "|" << (int) residue;
	}
	
	return decodedSequence;
}

bool Protein::operator==(Protein const & a) {
	return sequence == a.getSequence();
}


void Protein::print(ostream& out) {
	
	
	if (!header.empty()) {
	
		out << ">" << header << "\n";
	}
	
	
	out << decode() << "\n";
}
