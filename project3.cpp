// Project 3 - Linux Scheduler
// Ross Rydman - Feb 2014

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// change



int main (){
	// Setup file input from redirection
	ifstream inputFile;
	char inputFileName[] = "";
	inFile.open(inputFileName, ios::in);
	if (!inFile){
		std::cerr << "Error opening file." << endl;
		exit(1);
	}
	// Handle input
	char inputexample[100]
	while (!inFile.eof()){
		inFile >> inputexample; // Reads one element per line
	}
	// Close file after done reading
	inFile.close();

	return 0;
}

