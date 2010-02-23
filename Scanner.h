#pragma once

#include <sstream>
#include "Buffer.h"
#include "Const.h"
#include "Token.h"
#include "SymTable.h"
#include "SymTableEntry.h"


class Scanner
{
	public:
		SymTable *mySymTable;

		Scanner(char *inputFile, char *outputFile);
		Token *nextToken();
		void freeToken(Token);

		void processFile();

	private:
		Buffer *myBuffer;
		stateType currentState;
		
		char tokenSoFar[SCANNER_INTERNAL_BUFFER_SIZE];
		int internal_token_start;
		int internal_pos;
		int internal_filled_pos;
		
		int column;
		int line;
		int startColumn;
		int startLine;

		std::string intToState(int state);
		bool isPossibleEnding(int state);

		stateType Automat(int state, char nextChar); // returns nextState
		classType classOf(char theChar);

		Token *doStep();
};
