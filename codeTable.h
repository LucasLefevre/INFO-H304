
#include <map>

using std::map;


class CodeTable {
	
	private :
		map<char, unsigned int> encodeTable;
		map<unsigned int, char> decodeTable;
	
	public :
	
		CodeTable();
		unsigned int encode(char);
		char decode(unsigned int);
	
};
