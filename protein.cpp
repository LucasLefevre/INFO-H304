#include "protein.h"
#include <map>


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

vector<char> Protein::getSequence() const {
	return sequence;
}

void Protein::setHeader(string header) {
	this->header = header;
}

string Protein::decode() {
	
	map<int, char> decodeTable;
	
	decodeTable[0] = '-';
	decodeTable[1] = 'A';
	decodeTable[2] = 'B';
	decodeTable[3] = 'C';
	decodeTable[4] = 'D';
	decodeTable[5] = 'E';
	decodeTable[6] = 'F';
	decodeTable[7] = 'G';
	decodeTable[8] = 'H';
	decodeTable[9] = 'I';
	decodeTable[27] = 'J';
	decodeTable[10] = 'K';
	decodeTable[11] = 'L';
	decodeTable[12] = 'M';
	decodeTable[13] = 'N';
	decodeTable[26] = 'O';
	decodeTable[14] = 'P';
	decodeTable[15] = 'Q';
	decodeTable[16] = 'R';
	decodeTable[17] = 'S';
	decodeTable[18] = 'T';
	decodeTable[24] = 'U';
	decodeTable[19] = 'V';
	decodeTable[20] = 'W';
	decodeTable[21] = 'X';
	decodeTable[22] = 'Y';
	decodeTable[23] = 'Z';
	decodeTable[25] = '*';
	
	
	
	string decodedSequence = "";
	
	for (const auto & residue : sequence) {
		decodedSequence += decodeTable[residue];
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
