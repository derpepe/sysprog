#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "Const.h"


class Token
{
public:
	Token(tokenType myTokenType, std::string myTokenValue, int column, int line);

	int getLine();
	int getColumn();
	void setLine(int myLine);
	void setColumn(int myColumn);

	tokenType getTokenType();
	void setTokenType(tokenType myTokenType);


	std::string getLexem(); // returns string value of token
	double getFloatValue();
	std::string getFloatValueAsString();
	int getIntValue();
	std::string getIntValueAsString();

	void setTokenValue(std::string myTokenValue);
	void setTokenValue(float myTokenFloatValue);
	void setTokenValue(int myTokenIntValue);


private:
	int line;
	int column;

	tokenType myTokenType;

	std::string myTokenValue;
};
