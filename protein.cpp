#include "protein.h"
#include "codeTable.h"
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

Protein::Protein() {
	
}

Protein::Protein(char* sequence, int length, string header) {
	this->length = length;
	this->sequence = sequence;
	this->header = header;
}

Protein::~Protein() {}

void Protein::setSequence(char* sequence, int length) {
	this->length = length;
	this->sequence = sequence;
	
}

char Protein::getResidue(int i) {
	return sequence[i];
}
void Protein::loadFromFile(const string filename) {
	ifstream file;
	file.open(filename);
	
	CodeTable coder = CodeTable();
	string line;
	
	
	
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
					
					//sequence.push_back(coder.encode(value));
				}
			}
		}
		
		
	}
	else {
		cout << "Unable to open file : " << filename << "\n";
	}
}

char* Protein::getSequence() const {
	return sequence;
}

void Protein::setHeader(string header) {
	this->header = header;
}

string Protein::decode() {
	
	
	CodeTable coder = CodeTable();
	
	string decodedSequence = "";
	
	for (int i = 0; i < length; i++) {
		decodedSequence += coder.decode(sequence[i]);
		//cout << hex << "|" << (int) residue;
	}
	
	return decodedSequence;
}

bool Protein::operator==(Protein const & a) {
	return sequence == a.getSequence();
}


void Protein::print(string w, ostream& out) {
	
	
	if (w == "header" && !header.empty()) {
		out << ">" << header << "\n";
	}
	else if (w == "sequence") {
		out << decode() << "\n";
	}
	else {
		out << ">" << header << "\n";
		out << decode() << "\n";
	}
	
	
}

int Protein::size() {
	return length;
}
