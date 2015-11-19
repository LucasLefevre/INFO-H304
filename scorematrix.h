#include <vector>
#include <string>

using std::vector;
using std::string;

class ScoreMatrix {
	
	
	private :
	
		vector<int> matrix;
		int nbrCols;
		
		
		
	public :
	
		ScoreMatrix();
		ScoreMatrix(const string & filename, int nbrCols = 24);
		int &operator()(int i, int j);
		void print();
	
};
