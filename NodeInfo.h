/*
 *  NodeInfo.h
 *  sysprog
 *
 *  Created by Peter Schneider on 21.02.10.
 *  Copyright 2010 Schneider Medien Services. All rights reserved.
 *
 */
#pragma once

#include "parserConst.h"
#include "Token.h"
#include <iostream>


class NodeInfo
{
	
public:
	NodeInfo(nodeType myType = NODE_EMPTY, Token *myToken = NULL, typification myTypification = TYPIFICATION_NONE);
	
	void setType(nodeType myType);
	nodeType getType();
	
	void setToken(Token *myToken);
	Token *getToken();
	
	void setTypification(typification myTypification);
	typification getTypification();
	
private:
	nodeType myType;
	Token *myToken;
	typification myTypification;
};