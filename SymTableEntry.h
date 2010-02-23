#pragma once

#include <iostream>
#include "Info.h"
#include "parserConst.h"



class SymTableEntry
{
	public:
		SymTableEntry(std::string myPLexem, tokenType myTokenType, int line, int column);

		Info *getInfo();
		void setInfo(Info *myPInfo);

		std::string getLexem();
		void setLexem(std::string myPLexem);
	
		SymTableEntry *getLinkedItem();
		void setLinkedItem(SymTableEntry *newItem);
	
		#ifdef PARSER
		typification getTypification();
		void setTypification(typification myTypification);
		#endif

private:
		Info *myInfo;
		std::string myLexem;
		SymTableEntry *linkedItem;

		#ifdef PARSER
		typification myTypification;
		#endif
};
