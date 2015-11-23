all : database.o protein.o scorematrix.o codeTable.o main.cpp 
	g++ -std=c++11 database.o protein.o codeTable.o scorematrix.o main.cpp -o proj
	
database.o : database.cpp protein.h
	g++ -std=c++11 -c database.cpp
	
protein.o : protein.cpp codeTable.h
	g++ -std=c++11 -c protein.cpp 
	
scorematrix.o : scorematrix.cpp
	g++ -std=c++11 -c scorematrix.cpp
	
codeTable.o : codeTable.cpp
	g++ -std=c++11 -c codeTable.cpp
	
clean :
	rm -rf *.o
