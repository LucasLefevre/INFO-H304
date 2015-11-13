#include "protein.h"


using namespace std;

Protein::Protein() {
	
}

Protein::Protein(vector<char> sequence, string header) {
	this->sequence = sequence;
	this->header = header;
}

Protein::~Protein() {}

void Protein::setSequence(vector<char> sequence) {
	this->sequence = sequence;
}

void Protein::setHeader(string header) {
	this->header = header;
}

void Protein::print(iostream out) {
	
	if (!header.empty()) {
		out << header << "\n";
	}
	for (auto residue : sequence) {
		out << hex << (int) residue << "-";
	}
	out <<"\n";
}
