#include "Parser.h"

Parser::Parser(Scanner *myScanner)
{
	this->scanner = myScanner;
	parseTree = new ParseTree(myScanner);
}

void Parser::syntaxError(tokenType expected[], int expectedCount, Token *t)
{
	std::cerr << "Syntax Error: Expected '" << Const::tokenTypeAsString(expected[0]) << "'";
	
	for (int i = 1; i < expectedCount; i++) {
		if (i == expectedCount - 1) std::cerr << " or '"; else std::cerr << ", '";
		std::cerr << Const::tokenTypeAsString(expected[i]) << "'";
	}

	std::cerr << " at line " << t->getLine() << ", column " << t->getColumn() << " instead of '" << Const::tokenTypeAsString(t->getTokenType()) << "'." << std::endl;
	exit(1);
}

void Parser::syntaxError(tokenType expected, Token *t)
{
	int tokens[] = {expected};
	syntaxError(tokens, 1, t);
}

int Parser::processFile()
{
	//std::cout << "parsing ..." << std::endl << " " << std::endl;

	Token *myCommand[MAX_POSSIBLE_TOKENS] = {};
	Token *t;
	int tokenCount = 0;
	int tokenParsed;
	
	for (int i = 0; i < MAX_POSSIBLE_TOKENS; i++) myCommand[i] = new Token(TOKEN_ERROR, "", 0, 0);

	// Token stehen hier zur Verfügung, keine Nacharbeit mehr nötig
	while (t = scanner->nextToken()) {
		myCommand[tokenCount++] = t;
	}

	
	//std::cout << "syntax checking ..." << std::endl;
	
	tokenParsed = PROG(parseTree->getRootNode(), myCommand, 0, tokenCount);
	
	if (tokenCount == tokenParsed) {
		//std::cout << "syntax checking done" << std::endl;
	} else {
		// statement expected at tokenParsed

		#ifdef DEBUG
		std::cout << "grammar only parsed " << tokenParsed << " of " << tokenCount << "." << std::endl;
		#endif
		
		int tokens[] = {TOKEN_IDENTIFIER, TOKEN_WRITE, TOKEN_READ, TOKEN_CURLY_BRACE_OPEN, TOKEN_IF, TOKEN_WHILE};
		syntaxError(tokens, 6, myCommand[tokenParsed]);
	}
	
	
	//std::cout << " " << std::endl;
	parseTree->typeCheck();

	//std::cout << " " << std::endl;
	//parseTree->printXML();
	
	//std::cout << " " << std::endl;
	parseTree->makeCode();
	
	return 0;
}

int Parser::PROG(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "PROG " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_PROG)));
	
	// DECLS
	where = DECLS(myNode, myCommand, where, tokenCount);
	
	// STATEMENTS
	where = STATEMENTS(myNode, myCommand, where, tokenCount);
	
	return where;
}

int Parser::DECLS(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "DECLS " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_DECLS)));
	
	switch (myCommand[where]->getTokenType()) {
			
		// int, float
		case TOKEN_INT:
		case TOKEN_FLOAT:

			// DECL
			where = DECL(myNode, myCommand, where, tokenCount);

			// ;
			if ((myCommand[where]->getTokenType() != TOKEN_SEMICOLON)) syntaxError(TOKEN_SEMICOLON, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// DECLS
			where = DECLS(myNode, myCommand, where, tokenCount);
			
			break;
			
		// €
		default:
			break;
	}

	return where;
}

int Parser::DECL(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "DECL " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_DECL)));
	
	// TYPE
	where = TYPE(myNode, myCommand, where, tokenCount);
	
	// ARRAY
	where = ARRAY(myNode, myCommand, where, tokenCount);

	// identifier
	if ((myCommand[where]->getTokenType() != TOKEN_IDENTIFIER)) syntaxError(TOKEN_IDENTIFIER, myCommand[where]);
	myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
	where++;
		
	return where;
}

int Parser::ARRAY(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "ARRAY " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_ARRAY)));
	
	switch (myCommand[where]->getTokenType()) {
		
		// [
		case TOKEN_SQUARED_BRACKET_OPEN:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// integer
			if ((myCommand[where]->getTokenType() != TOKEN_INTEGER)) syntaxError(TOKEN_INTEGER, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;
	
			// ]
			if ((myCommand[where]->getTokenType() != TOKEN_SQUARED_BRACKET_CLOSE)) syntaxError(TOKEN_SQUARED_BRACKET_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			break;

		// €
		default:
			
			break;
	}
	
	return where;
}

int Parser::TYPE(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "TYPE " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_TYPE)));
	
	switch (myCommand[where]->getTokenType()) {
		
		// int
		case TOKEN_INT:
		case TOKEN_FLOAT:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			break;

		default:
			int tokens[] = {TOKEN_INT, TOKEN_FLOAT};
			syntaxError(tokens, 1, myCommand[where]);
			break;
	}
	
	return where;
}

int Parser::STATEMENTS(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "STATEMENTS " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_STATEMENTS)));
	
	switch (myCommand[where]->getTokenType()) {
		
		// identifier, write, read, {, if, while
		case TOKEN_IDENTIFIER:
		case TOKEN_WRITE:
		case TOKEN_READ:
		case TOKEN_CURLY_BRACE_OPEN:
		case TOKEN_IF:
		case TOKEN_WHILE:
			
			// STATEMENT
			where = STATEMENT(myNode, myCommand, where, tokenCount);

			// ;
			if ((myCommand[where]->getTokenType() != TOKEN_SEMICOLON)) syntaxError(TOKEN_SEMICOLON, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// STATEMENTS
			where = STATEMENTS(myNode, myCommand, where, tokenCount);

			break;
			
		// €
		default:
			break;
	}

	return where;
}

int Parser::STATEMENT(Node * node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "STATEMENT " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_STATEMENT)));
	
	switch (myCommand[where]->getTokenType()) {
			
		// identifier
		case TOKEN_IDENTIFIER:
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;
			
			// INDEX
			where = INDEX(myNode, myCommand, where, tokenCount);

			// =
			if ((myCommand[where]->getTokenType() != TOKEN_ASSIGN)) syntaxError(TOKEN_ASSIGN, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);

			break;
			
		// write
		case TOKEN_WRITE:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// (
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_OPEN)) syntaxError(TOKEN_BRACE_OPEN, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);
			
			// )
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_CLOSE)) syntaxError(TOKEN_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			break;
		
		// read
		case TOKEN_READ:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// (
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_OPEN)) syntaxError(TOKEN_BRACE_OPEN, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// identifier
			if ((myCommand[where]->getTokenType() != TOKEN_IDENTIFIER)) syntaxError(TOKEN_IDENTIFIER, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;

			// INDEX
			where = INDEX(myNode, myCommand, where, tokenCount);
			
			// )
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_CLOSE)) syntaxError(TOKEN_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			break;

		// {
		case TOKEN_CURLY_BRACE_OPEN:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// STATEMENTS
			where = STATEMENTS(myNode, myCommand, where, tokenCount);
			
			// }
			if ((myCommand[where]->getTokenType() != TOKEN_CURLY_BRACE_CLOSE)) syntaxError(TOKEN_CURLY_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			break;
			
		// if
		case TOKEN_IF:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// (
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_OPEN)) syntaxError(TOKEN_BRACE_OPEN, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);
			
			// )
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_CLOSE)) syntaxError(TOKEN_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// STATEMENT
			where = STATEMENT(myNode, myCommand, where, tokenCount);

			// else
			if ((myCommand[where]->getTokenType() != TOKEN_ELSE)) syntaxError(TOKEN_ELSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// STATEMENT
			where = STATEMENT(myNode, myCommand, where, tokenCount);
			
			break;
			
		// while
		case TOKEN_WHILE:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// (
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_OPEN)) syntaxError(TOKEN_BRACE_OPEN, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);
			
			// )
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_CLOSE)) syntaxError(TOKEN_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			// STATEMENT
			where = STATEMENT(myNode, myCommand, where, tokenCount);
			
			break;
			
		default:
			int tokens[] = {TOKEN_IDENTIFIER, TOKEN_WRITE, TOKEN_READ, TOKEN_CURLY_BRACE_OPEN, TOKEN_IF, TOKEN_WHILE};
			syntaxError(tokens, 6, myCommand[where]);
	}
			
	return where;
}

int Parser::EXP(Node * node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "EXP " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_EXP)));

	// EXP2
	where = EXP2(myNode, myCommand, where, tokenCount);
	
	// OP_EXP
	where = OP_EXP(myNode, myCommand, where, tokenCount);

	return where;
}

int Parser::EXP2(Node * node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "EXP2 " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_EXP2)));

	switch (myCommand[where]->getTokenType()) {
		
		// (
		case TOKEN_BRACE_OPEN:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);

			// )
			if ((myCommand[where]->getTokenType() != TOKEN_BRACE_CLOSE)) syntaxError(TOKEN_BRACE_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			break;
			
		// identifier
		case TOKEN_IDENTIFIER:
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;
			
			// INDEX
			where = INDEX(myNode, myCommand, where, tokenCount);
			
			break;
			
		// integer
		case TOKEN_INTEGER:
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;
			
			break;
			
		// real
		case TOKEN_REAL:
			myNode->addChild(new Node(new NodeInfo(NODE_IDENTIFIER, myCommand[where])));
			where++;
			
			break;
			
		// -
		case TOKEN_MINUS:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP2
			where = EXP2(myNode, myCommand, where, tokenCount);
			
			break;
			
		// !
		case TOKEN_EXCLAMATIONMARK:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP2
			where = EXP2(myNode, myCommand, where, tokenCount);
			
			break;
			
		// float
		case TOKEN_FLOAT:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP2
			where = EXP2(myNode, myCommand, where, tokenCount);
			
			break;
			
		default:
			int tokens[] = {TOKEN_BRACE_OPEN, TOKEN_IDENTIFIER, TOKEN_INTEGER, TOKEN_REAL, TOKEN_MINUS, TOKEN_EXCLAMATIONMARK, TOKEN_FLOAT};
			syntaxError(tokens, 7, myCommand[where]);
	}

	return where;
}

int Parser::INDEX(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "INDEX " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_INDEX)));
	
	switch (myCommand[where]->getTokenType()) {
			
		// [
		case TOKEN_SQUARED_BRACKET_OPEN:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;
			
			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);
			
			// ]
			if ((myCommand[where]->getTokenType() != TOKEN_SQUARED_BRACKET_CLOSE)) syntaxError(TOKEN_SQUARED_BRACKET_CLOSE, myCommand[where]);
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;

			break;
			
		// €
		default:
			break;
	}
	
	return where;
}

int Parser::OP_EXP(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "OP_EXP " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_OP_EXP)));
	
	switch (myCommand[where]->getTokenType()) {

		// +, -, *, /, <, =, &
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_STAR:
		case TOKEN_SLASH:
		case TOKEN_SMALLER:
		case TOKEN_ASSIGN:
		case TOKEN_AND:
			where = OP(myNode, myCommand, where, tokenCount);

			// EXP
			where = EXP(myNode, myCommand, where, tokenCount);
			
			break;
			
		// €
		default:
			break;
	}
	
	return where;
}

int Parser::OP(Node *node, Token *myCommand[], int startCount, int tokenCount)
{
	#ifdef DEBUG
	std::cout << "OP " << startCount << std::endl;
	#endif

	int where = startCount;
	Node *myNode = node->addChild(new Node(new NodeInfo(NODE_OP)));
	
	switch (myCommand[where]->getTokenType()) {
		
		// +, -, *, /, <, =, &
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_STAR:
		case TOKEN_SLASH:
		case TOKEN_SMALLER:
		case TOKEN_ASSIGN:
		case TOKEN_AND:
			myNode->addChild(new Node(new NodeInfo(NODE_KEYWORD, myCommand[where])));
			where++;			
			break;
			
		default:
			int tokens[] = {TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_SMALLER, TOKEN_ASSIGN, TOKEN_AND};
			syntaxError(tokens, 7, myCommand[where]);
	}
	
	return where;
}
