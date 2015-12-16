#include "codeTable.h"

CodeTable::CodeTable() {
	/*
	 * Hardcoded BLAST table
	 */
	
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
	
	encodeTable['-'] = 0;
	encodeTable['A'] = 1;
	encodeTable['B'] = 2;
	encodeTable['C'] = 3;
	encodeTable['D'] = 4;
	encodeTable['E'] = 5;
	encodeTable['F'] = 6;
	encodeTable['G'] = 7;
	encodeTable['H'] = 8;
	encodeTable['I'] = 9;
	encodeTable['J'] = 27;
	encodeTable['K'] = 10;
	encodeTable['L'] = 11;
	encodeTable['M'] = 12;
	encodeTable['N'] = 13;
	encodeTable['O'] = 26;
	encodeTable['P'] = 14;
	encodeTable['Q'] = 15;
	encodeTable['R'] = 16;
	encodeTable['S'] = 17;
	encodeTable['T'] = 18;
	encodeTable['U'] = 24;
	encodeTable['V'] = 19;
	encodeTable['W'] = 20;
	encodeTable['X'] = 21;
	encodeTable['Y'] = 22;
	encodeTable['Z'] = 23;
	encodeTable['*'] = 25;	
}

unsigned int CodeTable::encode(char c) {
	return encodeTable[c];
}

char CodeTable::decode(unsigned int i) {
	return decodeTable[i];
}
