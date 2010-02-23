#include "Token.h"

std::string StringToUpper(std::string strToConvert)
{//change each element of the string to upper case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
      strToConvert[i] = toupper(strToConvert[i]);
   }
   return strToConvert;//return the converted string
}

Token::Token(tokenType myTokenType, std::string myTokenValue, int column, int line)
{
	if (myTokenType == TOKEN_SIGN)
	{
		switch (myTokenValue[0])
		{
		case '+': myTokenType = TOKEN_PLUS; myTokenValue = ""; break;
		case '-': myTokenType = TOKEN_MINUS; myTokenValue = ""; break;
		case '/': myTokenType = TOKEN_SLASH; myTokenValue = ""; break;
		case '*': myTokenType = TOKEN_STAR; myTokenValue = ""; break;
		case '<': myTokenType = TOKEN_SMALLER; myTokenValue = ""; break;
		case '>': myTokenType = TOKEN_GREATER; myTokenValue = ""; break;
		case '=': myTokenType = TOKEN_ASSIGN; myTokenValue = ""; break;
		case '!': myTokenType = TOKEN_EXCLAMATIONMARK; myTokenValue = ""; break;
		case '&': myTokenType = TOKEN_AND; myTokenValue = ""; break;
		case ';': myTokenType = TOKEN_SEMICOLON; myTokenValue = ""; break;
		case ':': myTokenType = TOKEN_COLON; myTokenValue = ""; break;
		case '(': myTokenType = TOKEN_BRACE_OPEN; myTokenValue = ""; break;
		case ')': myTokenType = TOKEN_BRACE_CLOSE; myTokenValue = ""; break;
		case '{': myTokenType = TOKEN_CURLY_BRACE_OPEN; myTokenValue = ""; break;
		case '}': myTokenType = TOKEN_CURLY_BRACE_CLOSE; myTokenValue = ""; break;
		case '[': myTokenType = TOKEN_SQUARED_BRACKET_OPEN; myTokenValue = ""; break;
		case ']': myTokenType = TOKEN_SQUARED_BRACKET_CLOSE; myTokenValue = ""; break;
		}
	}
	
	/*
	if (myTokenType == TOKEN_IDENTIFIER)
	{
		// Sprachsymbole
		std::string upperCaseToken = StringToUpper(myTokenValue);
		if (upperCaseToken.compare("WRITE") == 0)
		{
			myTokenType = TOKEN_WRITE;
		}
		else
		if (upperCaseToken.compare("READ") == 0)
		{
			myTokenType = TOKEN_READ;
		}
	}
	*/
	
	Token::myTokenType = myTokenType;
	Token::myTokenValue = myTokenValue;
	Token::column = column;
	Token::line = line;
}


int Token::getLine()
{
	return line;
}

void Token::setLine(int line)
{
	Token::line = line;
}

int Token::getColumn()
{
	return column;
}

void Token::setColumn(int column)
{
	Token::column = line;
}

tokenType Token::getTokenType()
{
	return myTokenType;
}

void Token::setTokenType(tokenType myTokenType)
{
	Token::myTokenType = myTokenType;
}

std::string Token::getLexem()
{
	return myTokenValue;
}

double Token::getFloatValue()
{
	char *stopstring;

	//TODO: fix float value interpretation
	/*
	Wandeln Sie Int-Zahlen hierzu nach long und Real-Zahlen nach double.
	Bereichsüberschreitungen stellen Fehler dar und müssen erkannt werden (nutzen Sie hierzu strtol bzw. strtod und error.h)
	*/
	return strtod(myTokenValue.c_str(), &stopstring);
}

std::string Token::getFloatValueAsString()
{
	std::ostringstream o;
	o << this->getFloatValue();
	return o.str();
}

int Token::getIntValue()
{
	std::istringstream i(Token::myTokenValue);
	int myResult;
	i >> myResult;
	return myResult;
}

std::string Token::getIntValueAsString()
{
	return this->getLexem();
}

void Token::setTokenValue(std::string myTokenValue)
{
	Token::myTokenValue = myTokenValue;
}

void Token::setTokenValue(float myFloatValue)
{
	std::ostringstream o;
	o << myFloatValue;
	Token::myTokenValue = o.str();
}

void Token::setTokenValue(int myIntValue)
{
	std::ostringstream o;
	o << myIntValue;
	Token::myTokenValue = o.str();
}