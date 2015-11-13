
#include <string>
#include <vector>
#include "protein.h"


using std::vector;
using std::string;

class Database {
	
	private :
	
		string filename;
	
		int32_t version;
		int32_t type;
		string title;
		int32_t timestampLength;
		int32_t nbrSequences;
		int32_t residueCount;
		int32_t maxLength;
		vector<int> headerOffsets;
		vector<int> sequenceOffsets;
		vector<Protein*> proteins;
		
		int bytesToIntBigEndian(char bytes[], int lenght);
		int bytesToIntLittleEndian(char bytes[], int lenght);
		void loadHeader(const string &filename);
		void loadProteins(const string &filename);
		void loadHeaders(const string &filename);
		
	public : 
		Database(string dbName);
		~Database();
		void loadDb(const string dbName);
};


