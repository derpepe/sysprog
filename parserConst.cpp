/*
 *  parserConst.cpp
 *  sysprog
 *
 *  Created by Peter Schneider on 21.02.10.
 *  Copyright 2010 Schneider Medien Services. All rights reserved.
 *
 */

#include "parserConst.h"
#include <iostream>

parserConst::parserConst()
{
}
	
std::string parserConst::nodeTypeAsString(nodeType myNodeType)
{
	std::string myResult;
	switch(myNodeType)
	{
		case NODE_ROOT: myResult = "Root"; break;
		case NODE_ERROR: myResult = "Node Error"; break;
		case NODE_PROG: myResult = "PROG"; break;
		case NODE_DECLS: myResult = "DECLS"; break;
		case NODE_DECL: myResult = "DECL"; break;
		case NODE_ARRAY: myResult = "ARRAY"; break;
		case NODE_TYPE: myResult = "TYPE"; break;
		case NODE_STATEMENTS: myResult = "STATEMENTS"; break;
		case NODE_STATEMENT: myResult = "STATEMENT"; break;
		case NODE_EXP: myResult = "EXP"; break;
		case NODE_EXP2: myResult = "EXP2"; break;
		case NODE_INDEX: myResult = "INDEX"; break;
		case NODE_OP_EXP: myResult = "OP_EXP"; break;
		case NODE_OP: myResult = "OP"; break;
		case NODE_UNKNOWN: myResult = "UNKNOWN"; break;
		case NODE_KEYWORD: myResult = "Keyword"; break;
		case NODE_IDENTIFIER: myResult = "Identifier"; break;
		default: myResult = "unknown"; break;
	}
	return myResult;
}



std::string parserConst::typificationAsString(typification myTypification)
{
	std::string myResult;
	switch(myTypification)
	{
		case TYPIFICATION_NONE			: myResult="none"; break;   
		case TYPIFICATION_ERROR			: myResult="error"; break;    

		case TYPIFICATION_INT			: myResult="int"; break;    
		case TYPIFICATION_REAL			: myResult="real"; break;    
		case TYPIFICATION_ARRAY			: myResult="array"; break;    
		case TYPIFICATION_INT_ARRAY		: myResult="int[]"; break;
		case TYPIFICATION_REAL_ARRAY	: myResult="real[]"; break;
		case TYPIFICATION_OP_PLUS		: myResult="+"; break;
		case TYPIFICATION_OP_MINUS		: myResult="-"; break;
		case TYPIFICATION_OP_MULT		: myResult="*"; break;
		case TYPIFICATION_OP_DIV		: myResult="/"; break;
		case TYPIFICATION_OP_LESS		: myResult="<"; break;
		case TYPIFICATION_OP_NEG		: myResult="!"; break;
		case TYPIFICATION_OP_AND		: myResult="&"; break;
		case TYPIFICATION_OP_EQUAL		: myResult="="; break;

		default: myResult = "unknown"; break;
	}
	return myResult;
}