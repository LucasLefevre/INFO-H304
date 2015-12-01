#include "database.h"
#include <fstream>
#include <iostream>
#include <bitset>
#include <algorithm>

using namespace std;

Database::Database(string filename) {
	
	cout << "Loading database\n";
	this->filename = filename;
	loadHeader(filename);
	printInfos();
	loadProteins(filename);
	loadHeaders(filename);
	
}

void Database::loadHeader(const string &filename) {
	
	
	ifstream dbFile;
	dbFile.open(filename + ".pin", ios::binary | ios::in);

	if (dbFile.is_open()) {
		
		//version
		char versionB[4];  //array of 4 bytes (int32) --> B(ytes), every array of bytes end with B
		dbFile.read((char*)&versionB, sizeof(versionB));
		version = bytesToIntBigEndian(versionB, 4);
		
		//type
		char typeB[4];
		dbFile.read((char*)&typeB, sizeof(typeB));
		type = bytesToIntBigEndian(typeB, 4);
		
		//title lenght
		char titleLengthB[4];
		dbFile.read((char*)&titleLengthB, sizeof(titleLengthB));
		titleLength = bytesToIntBigEndian(titleLengthB, 4);
		
		//title
		char titleB[titleLength + 1];  
		titleB[titleLength] = '\0';
		dbFile.read((char*)&titleB, sizeof(titleB)-1);
		
		title = string(titleB);//Bug : there are 3 weird characters at the end
		
		//timestamp length
		char timestampLengthB[4];
		dbFile.read((char*)&timestampLengthB, sizeof(timestampLengthB));
		timestampLength = bytesToIntBigEndian(timestampLengthB, 4);
		
		//timestamp
		char timestampB[timestampLength];
		dbFile.read((char*)&timestampB, sizeof(timestampB));
		timestamp = string(timestampB);
		
		//number of sequences
		char nbrSequencesB[4];
		dbFile.read((char*)&nbrSequencesB, sizeof(nbrSequencesB));
		nbrSequences = bytesToIntBigEndian(nbrSequencesB, 4);
		
		//residue count
		char residueCountB[8];
		dbFile.read((char*)&residueCountB, sizeof(residueCountB));
		residueCount = bytesToIntLittleEndian(residueCountB, 8);
		
		//Length of the longest sequence
		char maxLengthB[4];
		dbFile.read((char*)&maxLengthB, sizeof(maxLengthB));
		maxLength = bytesToIntBigEndian(maxLengthB, 4);
		
		//header offset table
		headerOffsets.reserve(nbrSequences+1); 		//fix the size of the vector
		char offsetB[4]; 						//the offset is get as 4 bytes before convert it into an integer
		for (int i = 0; i <= nbrSequences; i++) {
			dbFile.read((char*) &offsetB, sizeof(offsetB));
			headerOffsets.push_back(bytesToIntBigEndian(offsetB, 4)); //convert bytes and store them as integer
		}	
		
		//sequence offset table
		sequenceOffsets.reserve(nbrSequences+1); 	//fix the size of the vector
		offsetB[4]; 							//the offset is get as 4 bytes before convert it into an integer
		for (int i = 0; i <= nbrSequences; i++) {
			dbFile.read((char*) &offsetB, sizeof(offsetB));
			sequenceOffsets.push_back(bytesToIntBigEndian(offsetB, 4)); //convert bytes and store them as integer
		}
		
		dbFile.close();	

		
		
		
	}
	else cout << "Unable to open database file : " << filename << ".pin\n";
}

void Database::loadProteins(const string &filename) {
	
	
	if (!headerOffsets.empty() && !sequenceOffsets.empty()) {
		
		
		ifstream dbFile;
		dbFile.open(filename + ".psq", ios::binary | ios::in);
		dbFile.seekg(0, ios::beg);
		
		if (dbFile.is_open()) {
			
			cout << "Loading proteins\n";
	
			//char firstByte[1];
			//dbFile.read((char*) &firstByte, sizeof(firstByte));
			
			int sequenceSize;
			for (int i = 0; i <= nbrSequences-1 ; i++){  //nbrSequences + 1
				
				
				sequenceSize = sequenceOffsets[i+1] - sequenceOffsets[i];
				char sequence[sequenceSize];
				dbFile.read((char*) &sequence, sequenceSize);
			
				Protein * prot = new Protein();
				vector<unsigned int> sequenceVector(sequence, sequence + sequenceSize);
				
				//sequenceVector.pop_back(); //remove the null byte between sequences
				
				prot->setSequence(sequenceVector);
				
				proteins.push_back(*prot);
			}
			

		}
		else cout << "Unable to open database file : " << filename << ".psq\n";
		
		
	
	}
	else cout << "Header offsets or sequence offsets not yet loaded\n";

}

void Database::loadHeaders(const string &filename) {
	
	
	if (!headerOffsets.empty() && !sequenceOffsets.empty()) {
		
		
		ifstream dbFile;
		dbFile.open(filename + ".phr", ios::binary | ios::in);
		dbFile.seekg(1, ios::beg);
		
		if (dbFile.is_open()) {
			
			cout << "Loading headers\n";
			
			int headerSize;
			for (int i = 0; i <= nbrSequences-1 ; i++){  
				
				headerSize = headerOffsets[i+1] - headerOffsets[i];
				char header[headerSize];
				dbFile.read((char*) &header, headerSize);
				
				proteins[i].setHeader(string(header).substr(7, string::npos));
				
				
			}
		}
		else cout << "Unable to open database file : " << filename << ".phr\n";
		
		
	
	}
	else cout << "Header offsets or sequence offsets not yet loaded\n";

}

int Database::bytesToIntLittleEndian(char bytes[], int lenght) {
	/*convert a byte array (little endian) to an integer */
	int res = 0;
	int j = lenght-1;
	
	for(int i = lenght-1; i >= 0; i--) {
		res += (bytes[i] & 0xFF) << (8*j); 
		j--;
	}
	return res;
}
	
int Database::bytesToIntBigEndian(char bytes[], int lenght) {
	/*convert a byte array (big endian) to an integer */

	int res = 0;
	int j = 0;
	
	for(int i = lenght-1; i >= 0; i--) {
		res += (bytes[i] & 0xFF) << (8*j); 
		j++;
	}
	return res;
}

bool Database::contains(Protein protein) {
	/*check if this database contains the protein passed as parameter*/
	
	int i = -1;
	for (Protein prot : proteins) {
		i++;
		if (prot == protein) {
			cout << "protein d'index : " << i << endl; 
			return true;
		}
	}
	return false;
}

Protein & Database::getProtein(int index) {
	if (index < nbrSequences) {
		return proteins[index];
	}
}




void Database::printInfos(std::ostream & out) {
	out << " - Version : " <<  version << "\n";
	out << " - Type : " << type  << "\n";
	out << " - Title lenght : " << titleLength  << "\n";
	out << " - Title : " << title << "\n";
	out << " - Timestamp Length : " <<  timestampLength  << "\n";
	out << " - Timestamp : " << timestamp  << "\n";
	out << " - Number of sequences : "  <<  nbrSequences << "\n";
	out << " - Residue count : " <<  residueCount << "\n";
	out << " - Longuest sequence : " << maxLength << "\n";
		
}

int Database::getNbrSequences() {
	return nbrSequences;
}




