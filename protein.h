#include <vector>
#include <string>
#include <iostream>

using std::out;
using std::string;
using std::vector;

class Protein {
	
	private :
		vector<char> sequence;
		string header;
	
	public :
	
		Protein();
		Protein(vector<char> sequence, string header);
		~Protein();
		
		vector<char> getSequence();
		void setSequence(vector<char> sequence);
		
		string getHeader();
		void setHeader(string header);
		
		void print(iostream out = cout);
	
};
