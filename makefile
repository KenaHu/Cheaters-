final: main.o
	g++ -o plagiarismCatcher main.o
main.o: main.cpp Table.h
	g++ -c main.cpp

