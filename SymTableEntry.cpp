#include "SymTableEntry.h"

SymTableEntry::SymTableEntry(std::string myPLexem, tokenType myTokenType, int line, int column)
{
	myLexem = myPLexem;
	linkedItem = NULL;
	myInfo = new Info(myTokenType, line, column);
	myTypification = TYPIFICATION_NONE;
}

std::string SymTableEntry::getLexem()
{
	return myLexem;
}

void SymTableEntry::setLexem(std::string myPLexem)
{
	myLexem = myPLexem;
}

Info *SymTableEntry::getInfo()
{
	return myInfo;
}

void SymTableEntry::setInfo(Info *myPInfo)
{
	myInfo = myPInfo;
}

SymTableEntry *SymTableEntry::getLinkedItem()
{
	return linkedItem;
}

void SymTableEntry::setLinkedItem(SymTableEntry *newItem)
{
	linkedItem = newItem;
}


#ifdef PARSER

typification SymTableEntry::getTypification()
{
	return myTypification;
}

void SymTableEntry::setTypification(typification myTypification)
{
	this->myTypification = myTypification;
}

#endif