#include "Const.h"


tokenType Const::stateTypeToTokenType(stateType state)
{
	tokenType myResult = TOKEN_UNKNOWN;
	switch(state)
	{
		case STATE_IDENTIFIER:
			myResult = TOKEN_IDENTIFIER;
			break;
		case STATE_INTEGER:
			myResult = TOKEN_INTEGER;
			break;
		case STATE_SIGN:
		case STATE_SIGN_OPENBRACE:
			myResult = TOKEN_SIGN;
			break;
		case STATE_REAL:
		case STATE_REAL_EXTENDED:
			myResult = TOKEN_REAL;
			break;
		case STATE_COMMENT:
			myResult = TOKEN_COMMENT;
			break;
	}
	return myResult;
}


std::string Const::tokenTypeAsString(tokenType type)
{
	std::string myResult;
	switch(type)
	{
		case TOKEN_ERROR:		myResult = "Error"; break;

		case TOKEN_IDENTIFIER:	myResult = "Identifier"; break;
		case TOKEN_INTEGER:		myResult = "Integer"; break;
		case TOKEN_SIGN:		myResult = "Sign"; break;
		case TOKEN_REAL:		myResult = "Real"; break;
		case TOKEN_READ:		myResult = "Read"; break;
		case TOKEN_WRITE:		myResult = "Write"; break;
		case TOKEN_COMMENT:		myResult = "Comment"; break;

		case TOKEN_PLUS:		myResult = "Plus"; break;
		case TOKEN_MINUS:		myResult = "Minus"; break;
		case TOKEN_SLASH:		myResult = "Slash"; break;
		case TOKEN_STAR:		myResult = "Star"; break;
		case TOKEN_SMALLER:		myResult = "Lesser"; break;
		case TOKEN_GREATER:		myResult = "Greater"; break;
		case TOKEN_ASSIGN:		myResult = "Assign"; break;
		case TOKEN_EXCLAMATIONMARK:			myResult = "Exclamation Mark"; break;
		case TOKEN_AND:						myResult = "And"; break;
		case TOKEN_SEMICOLON:				myResult = "Semicolon"; break;
		case TOKEN_COLON:					myResult = "Colon"; break;
		case TOKEN_BRACE_OPEN:				myResult = "Opening Bracket"; break;
		case TOKEN_BRACE_CLOSE:				myResult = "Closing Bracket"; break;
		case TOKEN_CURLY_BRACE_OPEN:		myResult = "Opening Brace"; break;
		case TOKEN_CURLY_BRACE_CLOSE:		myResult = "Closing Brace"; break;
		case TOKEN_SQUARED_BRACKET_OPEN:	myResult = "Opening Square Bracket"; break;
		case TOKEN_SQUARED_BRACKET_CLOSE:	myResult = "Closing Square Bracket"; break;

		// Erweiterung aus Aufgabe 2
		case TOKEN_IF:			myResult = "if"; break;
		case TOKEN_ELSE:		myResult = "else"; break;
		case TOKEN_WHILE:		myResult = "while"; break;
		case TOKEN_INT:			myResult = "int"; break;
		case TOKEN_FLOAT:		myResult = "float"; break;
		//
			
		default:				myResult = "UNKNOWN"; break;
	}
	return myResult;
}



std::string Const::stateTypeAsString(stateType state)
{
	std::string myResult;
	switch(state)
	{
		case STATE_UNKNOWN		: myResult = "Unknown"; break;
		case STATE_ERROR			: myResult = "Error"; break;

		case STATE_START			: myResult = "Start"; break;
		case STATE_IDENTIFIER		: myResult = "Identifier"; break;
		case STATE_INTEGER		: myResult = "Integer"; break;
		case STATE_SIGN			: myResult = "Sign"; break;
		case STATE_DOT			: myResult = "Dot"; break;
		case STATE_REAL			: myResult = "Real"; break;
		case STATE_REAL_E			: myResult = "Real E"; break;
		case STATE_REAL_PLUSMINUS	: myResult = "Real Plus/Minus"; break;
		case STATE_REAL_EXTENDED	: myResult = "Real Extended"; break;
		case STATE_SIGN_OPENBRACE	: myResult = "Sign Openbrace"; break;
		case STATE_IN_COMMENT		: myResult = "In Comment"; break;
		case STATE_ALMOST_COMMENT	: myResult = "Almost Comment"; break;
		case STATE_COMMENT		: myResult = "Comment"; break;

		default:				myResult = "UNKNOWN"; break;
	}
	return myResult;
}
