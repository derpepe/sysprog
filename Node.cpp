#include "Node.h"

Node::Node(NodeInfo *myInfo)
{
	for(int x = 0; x < MAX_CHILD_NODES; x++){
		childNodes[x] = NULL;
	}
	setInfo(myInfo);
}

NodeInfo *Node::getInfo()
{
	return myInfo;
}

void Node::setInfo(NodeInfo *myInfo)
{
	this->myInfo = myInfo;
}

int Node::getChildrenCount()
{
	return childrenCount;
}

Node *Node::addChild(Node* myChild)
{
	childNodes[childrenCount] = myChild;
	childrenCount++;
	return myChild;
}

Node *Node::getChild(int which)
{
	return childNodes[which];
}
	
void Node::print()
{
	if (myInfo->getType() == NODE_ROOT) {
		std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
	} else {
		std::cout << "<" << parserConst::nodeTypeAsString(myInfo->getType());
		if (myInfo->getTypification() != TYPIFICATION_NONE) {
			std::cout << " typification=\"" << parserConst::typificationAsString(myInfo->getTypification()) << "\"";
		}
		std::cout << ">";
	}
	
	if (childrenCount == 0) {
		if (myInfo->getToken() != NULL) {
			if (myInfo->getToken()->getTokenType() == TOKEN_IDENTIFIER) {
				// Identifier
				std::cout << myInfo->getToken()->getLexem();
			} else {
				// Keyword
				std::cout << Const::tokenTypeAsString(myInfo->getToken()->getTokenType());
			}
		}
	}

	for (int i = 0; i < childrenCount; i++) childNodes[i]->print();
	
	if (myInfo->getType() != NODE_ROOT) std::cout << "</" << parserConst::nodeTypeAsString(myInfo->getType()) << ">";
}

bool Node::isLeave()
{
	return (childrenCount == 0);
}