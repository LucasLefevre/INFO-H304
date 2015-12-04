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

const char Protein::getResidue(int i) const {
	return sequence[i];
}
void Protein::loadFromFile(const string filename) {
	//load the header and the sequence of a protein from a fasta file containing only one protein
	
	ifstream file;
	file.open(filename);
	
	CodeTable coder = CodeTable();
	string seq;
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
					
					seq += value;
				}
			}
		}
		
		//cout << seq << endl << flush;
		
		int sequenceSize = seq.size();
		length = sequenceSize;
		sequence = new char[sequenceSize+1];
		sequence[0] = 0;
		for (int i = 1; i <= sequenceSize; i++) {
			//cout << seq[i-1] << " " ;
			sequence[i] = coder.encode(seq[i-1]);
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
	//return a sequence string in readable format (letters instead of binary)
	
	CodeTable coder = CodeTable();
	
	string decodedSequence = "";
	
	for (int i = 0; i < length; i++) {
		decodedSequence += coder.decode(sequence[i]);
		//cout <<  "|" << i;
	}
	
	return decodedSequence;
}

bool Protein::operator==(Protein const & a) {
	//define the == operator for Protein --> we can do : proteinA == proteinB
	//two proteins are equal if they have the same sequence
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

const int Protein::size() const {
	//return the lenght of the sequence
	return length;
}
