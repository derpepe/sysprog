#pragma once

#include <iostream>

#define tokenType int
#define stateType int
#define classType int


static const int HASHTABLE_MAX_SIZE			=	200000;

static const int SCANNER_INTERNAL_BUFFER_SIZE = 5*1024*1024; // 5 MB max. Identifiersize
static const int SCANNER_IDENTIFIER_MAX_SIZE = 100000;


static const tokenType TOKEN_ERROR			= -1;
static const tokenType TOKEN_UNKNOWN		= TOKEN_ERROR - 1;

static const tokenType TOKEN_IDENTIFIER		= 0;
static const tokenType TOKEN_INTEGER		= TOKEN_IDENTIFIER + 1;
static const tokenType TOKEN_SIGN			= TOKEN_INTEGER + 1;
static const tokenType TOKEN_REAL			= TOKEN_SIGN + 1;
static const tokenType TOKEN_READ			= TOKEN_REAL + 1;
static const tokenType TOKEN_WRITE			= TOKEN_READ + 1;

// Erweiterung aus Aufgabe 2
static const tokenType TOKEN_IF				= TOKEN_WRITE + 1;
static const tokenType TOKEN_ELSE			= TOKEN_IF + 1;
static const tokenType TOKEN_WHILE			= TOKEN_ELSE + 1;
static const tokenType TOKEN_INT			= TOKEN_WHILE + 1;
static const tokenType TOKEN_FLOAT			= TOKEN_INT + 1;
//

static const tokenType TOKEN_COMMENT		= TOKEN_FLOAT + 1;

static const tokenType TOKEN_PLUS			= TOKEN_COMMENT + 1;
static const tokenType TOKEN_MINUS			= TOKEN_PLUS + 1;
static const tokenType TOKEN_SLASH			= TOKEN_MINUS + 1;
static const tokenType TOKEN_STAR			= TOKEN_SLASH + 1;
static const tokenType TOKEN_SMALLER		= TOKEN_STAR + 1;
static const tokenType TOKEN_GREATER		= TOKEN_SMALLER + 1;
static const tokenType TOKEN_ASSIGN			= TOKEN_GREATER + 1;
static const tokenType TOKEN_EXCLAMATIONMARK	= TOKEN_ASSIGN + 1;
static const tokenType TOKEN_AND			= TOKEN_EXCLAMATIONMARK + 1;
static const tokenType TOKEN_SEMICOLON		= TOKEN_AND + 1;
static const tokenType TOKEN_COLON			= TOKEN_SEMICOLON + 1;
static const tokenType TOKEN_BRACE_OPEN		= TOKEN_COLON + 1;
static const tokenType TOKEN_BRACE_CLOSE	= TOKEN_BRACE_OPEN + 1;
static const tokenType TOKEN_CURLY_BRACE_OPEN	= TOKEN_BRACE_CLOSE + 1;
static const tokenType TOKEN_CURLY_BRACE_CLOSE	= TOKEN_CURLY_BRACE_OPEN + 1;
static const tokenType TOKEN_SQUARED_BRACKET_OPEN	= TOKEN_CURLY_BRACE_CLOSE + 1;
static const tokenType TOKEN_SQUARED_BRACKET_CLOSE	= TOKEN_SQUARED_BRACKET_OPEN + 1;


static const stateType STATE_UNKNOWN		= -1;
static const stateType STATE_ERROR			= STATE_UNKNOWN - 1;

static const stateType STATE_START			= 0;
static const stateType STATE_IDENTIFIER		= STATE_START + 1;
static const stateType STATE_INTEGER		= STATE_IDENTIFIER + 1;
static const stateType STATE_SIGN			= STATE_INTEGER + 1;
static const stateType STATE_DOT			= STATE_SIGN + 1;
static const stateType STATE_REAL			= STATE_DOT + 1;
static const stateType STATE_REAL_E			= STATE_REAL + 1;
static const stateType STATE_REAL_PLUSMINUS	= STATE_REAL_E + 1;
static const stateType STATE_REAL_EXTENDED	= STATE_REAL_PLUSMINUS + 1;
static const stateType STATE_SIGN_OPENBRACE	= STATE_REAL_EXTENDED + 1;
static const stateType STATE_IN_COMMENT		= STATE_SIGN_OPENBRACE + 1;
static const stateType STATE_ALMOST_COMMENT	= STATE_IN_COMMENT + 1;
static const stateType STATE_COMMENT		= STATE_ALMOST_COMMENT + 1;


static const classType CLASS_UNKNOWN		= -1;
static const classType CLASS_DIGIT			= CLASS_UNKNOWN + 1;
static const classType CLASS_LETTER			= CLASS_DIGIT + 1;
static const classType CLASS_LETTER_E		= CLASS_LETTER + 1;
static const classType CLASS_SIGN_PLUSMINUS	= CLASS_LETTER_E + 1;
static const classType CLASS_SIGN			= CLASS_SIGN_PLUSMINUS + 1;
static const classType CLASS_SIGN_OPENBRACE	= CLASS_SIGN + 1;
static const classType CLASS_SIGN_CLOSEBRACE= CLASS_SIGN_OPENBRACE + 1;
static const classType CLASS_SIGN_STAR		= CLASS_SIGN_CLOSEBRACE + 1;
static const classType CLASS_BLANK			= CLASS_SIGN_STAR + 1;
static const classType CLASS_DOT			= CLASS_BLANK + 1;


class Const
{

	public:
		static tokenType stateTypeToTokenType(stateType state);
		
		static std::string stateTypeAsString(stateType state);
		static std::string tokenTypeAsString(tokenType type);

};