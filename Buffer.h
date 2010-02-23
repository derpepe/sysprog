#pragma once

#include <iostream>
#include <iomanip> // for std::setw
#include <fstream>
#include "Token.h"

class Buffer
{
	public:
		Buffer(std::string inputFile, std::string outputFile);
		~Buffer();

		char getChar();
		void ungetChar();

		bool endoffile;
		
		void writeTokenToFile(Token *myToken);
		
	private:
		std::filebuf *myInputBuffer;
		std::ifstream myInputFile;
};