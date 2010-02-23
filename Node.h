#pragma once
#include "parserConst.h"
#include "NodeInfo.h"
#include <iostream>

class Node
{
	
public:
	Node(NodeInfo *myInfo);

	NodeInfo *getInfo();
	void setInfo(NodeInfo *myInfo);

	int getChildrenCount();
	Node *addChild(Node* myChild);
	Node *getChild(int which);
	
	void print();
	
	bool isLeave();
	
private:
	NodeInfo *myInfo;

	int childrenCount;
	Node *childNodes[MAX_CHILD_NODES];
	
};
