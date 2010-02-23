/*
 *  NodeInfo.cpp
 *  sysprog
 *
 *  Created by Peter Schneider on 21.02.10.
 *  Copyright 2010 Schneider Medien Services. All rights reserved.
 *
 */

#include "NodeInfo.h"


NodeInfo::NodeInfo(nodeType myType, Token *myToken, typification myTypification)
{
	setType(myType);
	setToken(myToken);
	setTypification(myTypification);
}
	
void NodeInfo::setType(nodeType myType)
{
	this->myType = myType;
}

nodeType NodeInfo::getType()
{
	return myType;
}

void NodeInfo::setToken(Token *myToken)
{
	this->myToken = myToken;
}

Token *NodeInfo::getToken()
{
	return myToken;
}

void NodeInfo::setTypification(typification myTypification)
{
	this->myTypification = myTypification;
}

typification NodeInfo::getTypification()
{
	return myTypification;
}

