#pragma once
#include "Scanner.h"
#include "Token.h"
#include "parserConst.h"
#include "Node.h"
#include "NodeInfo.h"
#include "ParseTree.h"

class Parser
{
	
public:
	Parser(Scanner *myScanner);
	int processFile();
	
private:
	void syntaxError(tokenType expected, Token *t);
	void syntaxError(tokenType expected[], int expectedCount, Token *t);

	// implement grammar
	int PROG(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int DECLS(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int DECL(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int ARRAY(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int TYPE(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int STATEMENTS(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int STATEMENT(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int EXP(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int EXP2(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int INDEX(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int OP_EXP(Node *node, Token *myCommand[], int startCount, int tokenCount);
	int OP(Node *node, Token *myCommand[], int startCount, int tokenCount);
	
	// other things
	Scanner *scanner;
	ParseTree *parseTree;
	
};