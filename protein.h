#include <vector>
#include <string>
#include <iostream>


using std::string;
using std::vector;

class Protein {
	
	private :
		vector<unsigned int> sequence;
		string header;
		
		string decode();
	
	public :
	
		Protein();
		Protein(vector<unsigned int> sequence, string header);
		~Protein();
		void loadFromFile(const string filename);
		
		vector<unsigned int> getSequence() const;
		void setSequence(vector<unsigned int> sequence);
		
		string getHeader();
		void setHeader(string header);
		
		void print(std::ostream & out = std::cout);
		bool operator==(Protein const & a);
	
};
