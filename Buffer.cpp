#include "Buffer.h"


Buffer::Buffer(std::string inputFile, std::string outputFile)
{
	// associate the fstream object with a filebuf pointer
	myInputBuffer = myInputFile.rdbuf();
	myInputBuffer->open(inputFile.c_str(), std::ios::in);
	
	endoffile = 0;
	
	/*
	Verwenden Sie ausschließlich die C-Funktionen CreateFile, ReadFile und WriteFile aus dem Microsoft Platform SDK (windows.h).
	 Verwenden Sie FILE_FLAG_NO_BUFFERING, um den System-Cache auszuschalten.
	*/
}

char Buffer::getChar()
{
	char c;
	if (endoffile)
	{
		c = EOF;
	}
	else
	{
		c = (char)myInputBuffer->sbumpc();
		endoffile = (c == EOF);
	}
	return c;
}

void Buffer::ungetChar()
{
	myInputBuffer->pubseekoff(-1, std::ios_base::cur, std::ios_base::in);
}

void Buffer::writeTokenToFile(Token *myToken)
{
	tokenType type = myToken->getTokenType();
	int line = myToken->getLine();
	int column = myToken->getColumn();

	std::cout	<< "Token " << std::setw(15) << std::left << Const::tokenTypeAsString(type)
				<< " Line: " << std::setw(4) << line
				<< " Column: " << std::setw(4) << column;

	switch (type)
	{
		case TOKEN_REAL:
			std::cout << " Value: " << myToken->getFloatValueAsString();
			break;
		case TOKEN_INTEGER:
			std::cout << " Value: " << myToken->getIntValueAsString();
			break;
		case TOKEN_IDENTIFIER:
			std::cout << " Lexem: " << myToken->getLexem();
			break;
		case TOKEN_ERROR:
			std::cout << " fehlerhaftes Zeichen: " << myToken->getLexem();
			break;
		case TOKEN_UNKNOWN:
			std::cout << " gelesene(s) Zeichen: " << myToken->getLexem();
			break;
	}
	
	std::cout << std::endl;
}

Buffer::~Buffer()
{
	myInputFile.close();
}