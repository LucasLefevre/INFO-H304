#include <vector>
#include <string>
#include <iostream>


using std::string;
using std::vector;

class Protein {
	
	private :
		char* sequence;
		string header;
		int length;
		
		string decode();
	
	public :
	
		Protein();
		Protein(char* sequence, int length, string header);
		~Protein();
		void loadFromFile(const string filename);
		
		char* getSequence() const;
		void setSequence(char* sequence, int length);
		
		string getHeader();
		void setHeader(string header);
		char getResidue(int i);
		
		int size();
		
		void print(string w = "all", std::ostream & out = std::cout);
		bool operator==(Protein const & a);
	
};
