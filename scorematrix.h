#include <vector>
#include <string>

using std::vector;
using std::string;

class ScoreMatrix {
	
	
	private :
	
		vector<int> matrix;
		int nbrCols; //number of columns 
		
		
		
	public :
	
		ScoreMatrix();
		ScoreMatrix(const string & filename, int nbrCols = 27);  //27 is the size of a standard Blosum matrix
		
		//return element (i,j) of the matrix
		int &operator()(char i, char j);
		void print();
	
};
