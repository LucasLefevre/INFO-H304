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
	
	CodeTable coder = CodeTable();  //blast code table
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
		
		int sequenceSize = seq.size();
		length = sequenceSize;
		sequence = new char[sequenceSize+1];
		for (int i = 0; i <= sequenceSize; i++) {
			sequence[i] = coder.encode(seq[i]);
		}
	}
	else {
		cout << "Unable to open file : " << filename << "\n";
		throw string("Unable to load file");
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
		//cout <<  "|" << hex << (int) sequence[i];
	}
	
	return decodedSequence;
}

bool Protein::operator==(Protein const & a) {
	//define the == operator for Protein --> we can do : proteinA == proteinB
	//two proteins are equal if they have the same sequence
	return sequence == a.getSequence();
}

void Protein::print(string w, ostream& out) {
	/*
	 * Print a protein.  Print only the header, the sequence or both 
	 * based on the parameter w
	 */
	
	if (w == "header" && !header.empty()) { //only print the header
		
		if (header[0] != '>') { //if there is already a >, don't print another !
			out << ">";
		}
		out << header << "\n";
	}
	else if (w == "sequence") { //only print the sequence
		out << decode() << "\n";
	}
	else {  // print the header and the sequence
		if (header[0] != '>') {
			out << ">";
		}
		out << header << "\n";
		out << decode() << "\n";
	}
}

const int Protein::size() const {
	//return the lenght of the sequence
	return length;
}
