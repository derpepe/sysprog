#include "Info.h"

Info::Info(tokenType myTokenType, int myLine, int myColumn)
{
	this->setTokenType(myTokenType);
	this->setLine(myLine);
	this->setColumn(myColumn);
}

void Info::setTokenType(tokenType myTokenType)
{
	type = myTokenType;
}

void Info::setLine(int myLine)
{
	line = myLine;
}

void Info::setColumn(int myColumn)
{
	column = myColumn;
}

tokenType Info::getTokenType()
{
	return type;
}

int Info::getLine()
{
	return line;
}

int Info::getColumn()
{
	return column;
}

Info::~Info(void)
{
}
