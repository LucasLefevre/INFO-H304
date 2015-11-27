#include "protein.h"
#include "codeTable.h"
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>


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

unsigned int Protein::getResidue(int i) {
	return sequence[i];
}
void Protein::loadFromFile(const string filename) {
	ifstream file;
	file.open(filename);
	
	CodeTable coder = CodeTable();
	string line;
	
	sequence.push_back(0); //add a gap at the beginning
	
	if (file.is_open()) {
		
		
		while (getline(file, line)) {
			
			if (line[0] == '>') {
				header = line;
			}
			else {
				
				stringstream converter;
				converter << &line[0];
				char value;
				while (converter >> value){
					
					sequence.push_back(coder.encode(value));
				}
			}
		}
	}
	else {
		cout << "Unable to open file : " << filename << "\n";
	}
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

int Protein::size() {
	return sequence.size();
}
