#pragma once
#include <iostream>
#include "Node.h"
#include "Scanner.h"
#include "Marker.h"

class ParseTree
{

public:
	ParseTree(Scanner *myScanner);
	
	Node *getRootNode();
	void printXML();
	
	Scanner *getScanner(); // needed if you want to use the scanner, p.ex. for viewing the symtable

	
	void typeCheck(Node *myNode = NULL);
	
	// helper functions that match getting and setting of typification for identifiers to the symtable
	SymTableEntry *getSymTableEntryForIdentifier(std::string identifier);
	void setType(Node *myNode, typification myType);
	typification getType(Node *myNode);

	
	void makeCode(Node *myNode = NULL);

	
private:
	Node *rootNode;
	Scanner *myScanner;
	
};
