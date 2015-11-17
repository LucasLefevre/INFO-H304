#include <vector>
#include <string>
#include <iostream>


using std::string;
using std::vector;

class Protein {
	
	private :
		vector<char> sequence;
		string header;
		
		string decode();
	
	public :
	
		Protein();
		Protein(vector<char> sequence, string header);
		~Protein();
		
		vector<char> getSequence() const;
		void setSequence(vector<char> sequence);
		
		string getHeader();
		void setHeader(string header);
		
		void print(std::ostream & out = std::cout);
		bool operator==(Protein const & a);
	
};
