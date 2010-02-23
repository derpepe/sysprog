#include "ParseTree.h"

ParseTree::ParseTree(Scanner *myScanner)
{
	rootNode = new Node(new NodeInfo(NODE_ROOT));
	this->myScanner = myScanner;
}

Node *ParseTree::getRootNode()
{
	return rootNode;
}

void ParseTree::printXML()
{
	rootNode->print();
	std::cout << std::endl;
}

Scanner *ParseTree::getScanner()
{
	return myScanner;
}


SymTableEntry *ParseTree::getSymTableEntryForIdentifier(std::string identifier)
{
	SymTableEntry *theItem = new SymTableEntry(identifier, TOKEN_IDENTIFIER, 0, 0);
			
	// search for identifier in hashtable
	SymTableKey myKey = myScanner->mySymTable->getKey(theItem);
		
	SymTableEntry *currentObject = myScanner->mySymTable->lookup(myKey);
	while (currentObject != NULL)
	{
		if (currentObject->getLexem().compare(identifier) == 0)
		{
			return currentObject;
		}
	}
	std::cerr << "Internal Error: Identifier " << identifier << " not found in Symbol Table." << std::endl;
	return currentObject;
}


typification ParseTree::getType(Node *myNode)
{
	if (myNode->getInfo()->getType() == NODE_IDENTIFIER) {
		// lookup in SymTable
		return getSymTableEntryForIdentifier(myNode->getInfo()->getToken()->getLexem())->getTypification();

	} else {
		// lookup in NodeInfo
		return myNode->getInfo()->getTypification();
	}
}

void ParseTree::setType(Node *myNode, typification myType)
{
	if (myNode->getInfo()->getType() == NODE_IDENTIFIER) {
		// set in SymTable
		getSymTableEntryForIdentifier(myNode->getInfo()->getToken()->getLexem())->setTypification(myType);
		myNode->getInfo()->setTypification(myType);

	} else {
		// set in NodeInfo
		myNode->getInfo()->setTypification(myType);
	}
}

void ParseTree::typeCheck(Node *myNode)
{
	// start with root node
	if (myNode == NULL) myNode = rootNode;
	
	//std::cout << parserConst::nodeTypeAsString(myNode->getInfo()->getType()) << std::endl;
		
	// typecheck the node itself
	switch (myNode->getInfo()->getType()) {

		case NODE_ROOT:
			// PROG
			//std::cout << "type checking ..." << std::endl;
			typeCheck(myNode->getChild(0));
			setType(myNode, TYPIFICATION_NONE);
			//std::cout << "type checking done" << std::endl;
			break;
			
		case NODE_PROG:
			// DECLS STATEMENTS
			typeCheck(myNode->getChild(0));
			typeCheck(myNode->getChild(1));
			setType(myNode, TYPIFICATION_NONE);
			break;

		case NODE_DECLS:
			if (myNode->getChild(0) != NULL) {
				// DECL ; DECLS
				typeCheck(myNode->getChild(0));
				typeCheck(myNode->getChild(2));
				setType(myNode, TYPIFICATION_NONE);
			} else {
				// €
				setType(myNode, TYPIFICATION_NONE);
			}
			break;

		case NODE_DECL:
			// TYPE ARRAY identifier
			typeCheck(myNode->getChild(0));
			typeCheck(myNode->getChild(1));
			if (getType(myNode->getChild(2)) != TYPIFICATION_NONE || getType(myNode->getChild(1)) == TYPIFICATION_ERROR) {
				std::cerr << "Typification Error: identifier '" << myNode->getChild(2)->getInfo()->getToken()->getLexem() << "' is already defined";
				std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
				setType(myNode, TYPIFICATION_ERROR);
			} else {
				setType(myNode, TYPIFICATION_NONE);
				if (getType(myNode->getChild(0)) == TYPIFICATION_INT) {
					if (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY) {
						setType(myNode->getChild(2), TYPIFICATION_INT_ARRAY);
					} else {
						setType(myNode->getChild(2), TYPIFICATION_INT);
					}
				} else {
					if (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY) {
						setType(myNode->getChild(2), TYPIFICATION_REAL_ARRAY);
					} else {
						setType(myNode->getChild(2), TYPIFICATION_REAL);
					}
				}
			}
			break;

		case NODE_ARRAY:
			if (myNode->getChildrenCount() > 0) {
				// [ integer ]
				if (myNode->getChild(1)->getInfo()->getToken()->getIntValue() > 0) {
					setType(myNode, TYPIFICATION_ARRAY);
				} else {
					setType(myNode, TYPIFICATION_ERROR);
				}
			} else {
				// €
				setType(myNode, TYPIFICATION_NONE);
			}
			break;
			
		case NODE_TYPE:
			if (myNode->getChild(0)->getInfo()->getToken()->getTokenType() == TOKEN_INT) {
				setType(myNode, TYPIFICATION_INT);
			} else {
				setType(myNode, TYPIFICATION_REAL);
			}
			break;

		case NODE_STATEMENTS:
			if (myNode->getChildrenCount() > 0) {
				// STATEMENT ; STATEMENTS
				typeCheck(myNode->getChild(0));
				typeCheck(myNode->getChild(2));
				setType(myNode, TYPIFICATION_NONE);
			} else {
				// €
				setType(myNode, TYPIFICATION_NONE);
			}
			break;
		
		case NODE_STATEMENT:
			if (myNode->getChild(0)->getInfo()->getType() == NODE_IDENTIFIER) {
				// identifier INDEX = EXP
				typeCheck(myNode->getChild(3));
				typeCheck(myNode->getChild(1));
				if (getType(myNode->getChild(0)) == TYPIFICATION_NONE) {
					std::cerr << "Typification Error: identifier '" << myNode->getChild(2)->getInfo()->getToken()->getLexem() << "' is not defined";
					std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
					setType(myNode, TYPIFICATION_ERROR);
				} else {
					if ( (getType(myNode->getChild(3)) == TYPIFICATION_INT) && (
						  ( (getType(myNode->getChild(0)) == TYPIFICATION_INT) && (getType(myNode->getChild(1)) == TYPIFICATION_NONE))
						  || ((getType(myNode->getChild(0)) == TYPIFICATION_INT_ARRAY) && (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY)))
						|| (getType(myNode->getChild(3)) == TYPIFICATION_REAL) && (
						  ((getType(myNode->getChild(0)) == TYPIFICATION_REAL) && (getType(myNode->getChild(1)) == TYPIFICATION_NONE))
						  || ((getType(myNode->getChild(0)) == TYPIFICATION_REAL_ARRAY) && (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY))) )
						 {
						setType(myNode, TYPIFICATION_NONE);
					} else {
						std::cerr << "Typification Error: identifier '" << myNode->getChild(0)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(0))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
						std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
						setType(myNode, TYPIFICATION_ERROR);
					}
						
				}
			} else {
				// NODE_KEYWORD
				switch (myNode->getChild(0)->getInfo()->getToken()->getTokenType()) {
					case TOKEN_WRITE:
						// write ( EXP )
						typeCheck(myNode->getChild(2));
						setType(myNode, TYPIFICATION_NONE);
						break;
					case TOKEN_READ:
						// read ( identifier INDEX )
						if (getType(myNode->getChild(2)) == TYPIFICATION_NONE) {
							std::cerr << "Typification Error: identifier '" << myNode->getChild(2)->getInfo()->getToken()->getLexem() << "' is not defined";
							std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
							setType(myNode, TYPIFICATION_ERROR);							
						} else {
							if ( ((getType(myNode->getChild(2)) == TYPIFICATION_INT) || ((getType(myNode->getChild(2)) == TYPIFICATION_REAL)) && (getType(myNode->getChild(3)) == TYPIFICATION_NONE))
								  || ((getType(myNode->getChild(2)) == TYPIFICATION_INT_ARRAY) || ((getType(myNode->getChild(2)) == TYPIFICATION_REAL_ARRAY)) && (getType(myNode->getChild(3)) == TYPIFICATION_ARRAY)) ) {
								setType(myNode, TYPIFICATION_NONE);							
							} else {
								std::cerr << "Typification Error: identifier '" << myNode->getChild(2)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
								std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
								setType(myNode, TYPIFICATION_ERROR);
							}
						}
						break;
					case TOKEN_CURLY_BRACE_OPEN:
						// { STATEMENTS }
						typeCheck(myNode->getChild(1));
						setType(myNode, TYPIFICATION_NONE);
						break;
					case TOKEN_IF:
						// if ( EXP ) STATEMENT else STATEMENT
						typeCheck(myNode->getChild(2));
						typeCheck(myNode->getChild(4));
						typeCheck(myNode->getChild(6));
						if (getType(myNode->getChild(2)) != TYPIFICATION_INT) {
							std::cerr << "Typification Error: integer required as expression for 'if' statement";
							std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
							setType(myNode, TYPIFICATION_ERROR);
						} else {
							setType(myNode, TYPIFICATION_NONE);
						}
						break;
					case TOKEN_WHILE:
						// while ( EXP ) STATEMENT
						typeCheck(myNode->getChild(2));
						typeCheck(myNode->getChild(4));
						if (getType(myNode->getChild(2)) != TYPIFICATION_INT) {
							std::cerr << "Typification Error: integer required as expression for 'while' statement";
							std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
							setType(myNode, TYPIFICATION_ERROR);
						} else {
							setType(myNode, TYPIFICATION_NONE);
						}
						break;
					default:
						setType(myNode, TYPIFICATION_ERROR);
						break;
				}
					
			}
			break;

		case NODE_INDEX:
			if (myNode->getChildrenCount() > 0) {
				// [ EXP ]
				typeCheck(myNode->getChild(1));
				if (getType(myNode->getChild(1)) == TYPIFICATION_INT) {
					setType(myNode, TYPIFICATION_ARRAY);
				} else {
					std::cerr << "Typification Error: integer required as index";
					std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
					setType(myNode, TYPIFICATION_ERROR);
				}
			} else {
				// €
				setType(myNode, TYPIFICATION_NONE);
			}
			break;
		
		case NODE_EXP:
			// EXP2 OP_EXP
			typeCheck(myNode->getChild(0));
			typeCheck(myNode->getChild(1));
			if (getType(myNode->getChild(1)) == TYPIFICATION_NONE) {
				setType(myNode, getType(myNode->getChild(0)));
			} else if (getType(myNode->getChild(0)) != getType(myNode->getChild(1))) {
				// incompatible types
				std::cerr << "Typification Error: incompatible types" << std::endl;
				//TODO
				//std::cerr << "Typification Error: expression '" << myNode->getChild(0)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(0))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
				//std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
				setType(myNode, TYPIFICATION_ERROR);
			} else if ((getType(myNode->getChild(0)) == TYPIFICATION_REAL) && (getType(myNode->getChild(1)->getChild(0)) == TYPIFICATION_OP_AND)) {
				std::cerr << "'&' operator requires integer" << std::endl;
				//TODO
				//std::cerr << "Typification Error: expression '" << myNode->getChild(0)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(0))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
				//std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
				setType(myNode, TYPIFICATION_ERROR);
			} else if ((getType(myNode->getChild(1)->getChild(0)) == TYPIFICATION_OP_LESS) || (getType(myNode->getChild(1)->getChild(0)) == TYPIFICATION_OP_EQUAL)) {
				setType(myNode, TYPIFICATION_INT);
			} else {
				setType(myNode, getType(myNode->getChild(0)));
			}
			break;
			
		case NODE_EXP2:
			switch (myNode->getChild(0)->getInfo()->getToken()->getTokenType()) {
				case TOKEN_BRACE_OPEN:
					// ( EXP )
					typeCheck(myNode->getChild(1));
					setType(myNode, getType(myNode->getChild(1)));
					break;
				case TOKEN_IDENTIFIER:
					// identifier INDEX
					typeCheck(myNode->getChild(1));
					if (getType(myNode->getChild(0)) == TYPIFICATION_NONE) {
						std::cerr << "Typification Error: identifier '" << myNode->getChild(0)->getInfo()->getToken()->getLexem() << "' is not defined";
						std::cerr << " at line " << myNode->getChild(0)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(0)->getInfo()->getToken()->getColumn() << "." << std::endl;
						setType(myNode, TYPIFICATION_ERROR);							
					} else if ((getType(myNode->getChild(0)) == TYPIFICATION_INT) || ((getType(myNode->getChild(0)) == TYPIFICATION_REAL)) && (getType(myNode->getChild(1)) == TYPIFICATION_NONE)) {
						setType(myNode, getType(myNode->getChild(0)));
					} else if ((getType(myNode->getChild(0)) == TYPIFICATION_INT_ARRAY) && (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY)) {
						setType(myNode, TYPIFICATION_INT);
					} else if ((getType(myNode->getChild(0)) == TYPIFICATION_REAL_ARRAY) && (getType(myNode->getChild(1)) == TYPIFICATION_ARRAY)) {
						setType(myNode, TYPIFICATION_REAL);
					} else {
						std::cerr << "no primitive type" << std::endl;;
						//std::cerr << "Typification Error: identifier '" << myNode->getChild(2)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
						//std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
						//TODO
						setType(myNode, TYPIFICATION_ERROR);
					}
					break;
				case TOKEN_INTEGER:
					// integer
					setType(myNode, TYPIFICATION_INT);
					break;
				case TOKEN_REAL:
					// real
					setType(myNode, TYPIFICATION_REAL);
					break;
				case TOKEN_MINUS:
					// - EXP2
					typeCheck(myNode->getChild(1));
					setType(myNode, getType(myNode->getChild(1)));
					break;
				case TOKEN_EXCLAMATIONMARK:
					// ! EXP2
					typeCheck(myNode->getChild(1));
					if (getType(myNode->getChild(1)) != TYPIFICATION_INT) {
						std::cerr << "'!' operator requires integer" << std::endl;
						//TODO
						//std::cerr << "Typification Error: expression '" << myNode->getChild(0)->getInfo()->getToken()->getLexem() << "' of type '" << parserConst::typificationAsString(getType(myNode->getChild(0))) << "' is incompatible to type '" << parserConst::typificationAsString(getType(myNode->getChild(3))) << "'";
						//std::cerr << " at line " << myNode->getChild(2)->getInfo()->getToken()->getLine() << ", column " << myNode->getChild(2)->getInfo()->getToken()->getColumn() << "." << std::endl;
						setType(myNode, TYPIFICATION_ERROR);
					} else {
						setType(myNode, getType(myNode->getChild(1)));
					}
					break;
				case TOKEN_FLOAT:
					// float EXP2
					typeCheck(myNode->getChild(1));
					setType(myNode, TYPIFICATION_REAL);
					break;
			}

			break;

		case NODE_OP_EXP:
			if (myNode->getChildrenCount() > 0) {
				// OP EXP
				typeCheck(myNode->getChild(0));
				typeCheck(myNode->getChild(1));
				setType(myNode, myNode->getChild(1)->getInfo()->getTypification());
			} else {
				// €
				setType(myNode, TYPIFICATION_NONE);
			}
			break;
			
		case NODE_OP:
			// OP
			switch (myNode->getChild(0)->getInfo()->getToken()->getTokenType()) {
				case TOKEN_PLUS:
					setType(myNode, TYPIFICATION_OP_PLUS);
					break;
				case TOKEN_MINUS:
					setType(myNode, TYPIFICATION_OP_MINUS);
					break;
				case TOKEN_STAR:
					setType(myNode, TYPIFICATION_OP_MULT);
					break;
				case TOKEN_SLASH:
					setType(myNode, TYPIFICATION_OP_DIV);
					break;
				case TOKEN_SMALLER:
					setType(myNode, TYPIFICATION_OP_LESS);
					break;
				case TOKEN_ASSIGN:
					setType(myNode, TYPIFICATION_OP_EQUAL);
					break;
				case TOKEN_AND:
					setType(myNode, TYPIFICATION_OP_AND);
					break;
				default:
					setType(myNode, TYPIFICATION_ERROR);
					break;
			}
			break;

		
		case NODE_UNKNOWN:
		case NODE_ERROR:
		case NODE_EMPTY:
		case NODE_IDENTIFIER:
		case NODE_KEYWORD:
		default:
			break;
	}
}


void ParseTree::makeCode(Node *myNode)
{
	Marker *m1;
	Marker *m2;
	
	// start with root node
	if (myNode == NULL) myNode = rootNode;
		
	// typecheck the node itself
	switch (myNode->getInfo()->getType()) {

		case NODE_ROOT:
			// PROG
			//std::cout << "code generation ..." << std::endl;
			makeCode(myNode->getChild(0));
			//std::cout << "code generation done" << std::endl;
			break;
			
		case NODE_PROG:
			// DECLS STATEMENTS
			makeCode(myNode->getChild(0));
			makeCode(myNode->getChild(1));
			std::cout << "STP" << std::endl;
			break;

		case NODE_DECLS:
			if (myNode->getChild(0) != NULL) {
				// DECL ; DECLS
				makeCode(myNode->getChild(0));
				makeCode(myNode->getChild(2));
			} else {
				// €
			}
			break;

		case NODE_DECL:
			// TYPE ARRAY identifier
			std::cout << "DS " << myNode->getChild(2)->getInfo()->getToken()->getLexem() << " ";
			makeCode(myNode->getChild(1));
			break;

		case NODE_ARRAY:
			if (myNode->getChildrenCount() > 0) {
				// [ integer ]
				std::cout << myNode->getChild(1)->getInfo()->getToken()->getIntValue() << std::endl;
			} else {
				// €
				std::cout << "1" << std::endl;
			}
			break;
			
		case NODE_TYPE:
			break;

		case NODE_STATEMENTS:
			if (myNode->getChildrenCount() > 0) {
				// STATEMENT ; STATEMENTS
				makeCode(myNode->getChild(0));
				makeCode(myNode->getChild(2));
			} else {
				// €
				std::cout << "NOP" << std::endl;
			}
			break;
		
		case NODE_STATEMENT:
			if (myNode->getChild(0)->getInfo()->getType() == NODE_IDENTIFIER) {
				// identifier INDEX = EXP
				makeCode(myNode->getChild(3));
				std::cout << "LA " << myNode->getChild(0)->getInfo()->getToken()->getLexem() << std::endl;
				makeCode(myNode->getChild(1));
				std::cout << "STR" << std::endl;
			} else {
				// NODE_KEYWORD
				switch (myNode->getChild(0)->getInfo()->getToken()->getTokenType()) {
					case TOKEN_WRITE:
						// write ( EXP )
						makeCode(myNode->getChild(2));
						if (getType(myNode->getChild(2)) == TYPIFICATION_INT) {
							std::cout << "PRI" << std::endl;
						} else {
							std::cout << "PRF" << std::endl;
						}
						break;
					case TOKEN_READ:
						// read ( identifier INDEX )
						if ((getType(myNode->getChild(2)) == TYPIFICATION_INT) || (getType(myNode->getChild(2)) == TYPIFICATION_INT_ARRAY)) {
							std::cout << "RDI" << std::endl;
						} else {
							std::cout << "RDF" << std::endl;
						}
						std::cout << "LA " << myNode->getChild(2)->getInfo()->getToken()->getLexem() << std::endl;
						makeCode(myNode->getChild(3));
						std::cout << "STR" << std::endl;
						break;
					case TOKEN_CURLY_BRACE_OPEN:
						// { STATEMENTS }
						makeCode(myNode->getChild(1));
						break;
					case TOKEN_IF:
						// if ( EXP ) STATEMENT else STATEMENT
						makeCode(myNode->getChild(2));
						m1 = new Marker();
						m2 = new Marker();
						std::cout << "JIN *marker" << m1->getName() << std::endl;
						makeCode(myNode->getChild(4));
						std::cout << "JMP *marker" << m2->getName() << std::endl;
						std::cout << "*marker" << m1->getName() << " NOP" << std::endl;
						makeCode(myNode->getChild(6));
						std::cout << "*marker" << m2->getName() << " NOP" << std::endl;
						break;
					case TOKEN_WHILE:
						// while ( EXP ) STATEMENT
						m1 = new Marker();
						m2 = new Marker();
						std::cout << "*marker" << m1->getName() << " NOP" << std::endl;
						makeCode(myNode->getChild(2));
						std::cout << "JIN *marker" << m2->getName() << std::endl;
						makeCode(myNode->getChild(4));
						std::cout << "JMP *marker" << m1->getName() << std::endl;
						std::cout << "*marker" << m2->getName() << " NOP" << std::endl;
						break;
					default:
						break;
				}
					
			}
			break;

		case NODE_INDEX:
			if (myNode->getChildrenCount() > 0) {
				// [ EXP ]
				makeCode(myNode->getChild(1));
				std::cout << "ADI" << std::endl;
			} else {
				// €
			}
			break;
		
		case NODE_EXP:
			// EXP2 OP_EXP
			makeCode(myNode->getChild(0));
			if (getType(myNode->getChild(1)) == TYPIFICATION_NONE) break;
			makeCode(myNode->getChild(1));
			break;
			
		case NODE_EXP2:
			switch (myNode->getChild(0)->getInfo()->getToken()->getTokenType()) {
				case TOKEN_BRACE_OPEN:
					// ( EXP )
					makeCode(myNode->getChild(1));
					break;
				case TOKEN_IDENTIFIER:
					// identifier INDEX
					std::cout << "LA " << myNode->getChild(0)->getInfo()->getToken()->getLexem() << std::endl;
					makeCode(myNode->getChild(1));
					std::cout << "LV" << std::endl;
					break;
				case TOKEN_INTEGER:
					// integer
					std::cout << "LC " << myNode->getChild(0)->getInfo()->getToken()->getIntValue() << std::endl;
					break;
				case TOKEN_REAL:
					// real
					std::cout << "LC " << myNode->getChild(0)->getInfo()->getToken()->getFloatValue() << std::endl;
					break;
				case TOKEN_MINUS:
					// - EXP2
					if (getType(myNode->getChild(1)) == TYPIFICATION_INT) {
						std::cout << "LC 0" << std::endl;
						makeCode(myNode->getChild(1));
						std::cout << "SBI" << std::endl;
					} else {
						std::cout << "LC 0.0" << std::endl;
						makeCode(myNode->getChild(1));
						std::cout << "SBF" << std::endl;
					}
					break;
				case TOKEN_EXCLAMATIONMARK:
					// ! EXP2
					makeCode(myNode->getChild(1));
					std::cout << "NOT" << std::endl;
					break;
				case TOKEN_FLOAT:
					// float EXP2
					makeCode(myNode->getChild(1));
					std::cout << "FLT" << std::endl;
					break;
			}

			break;

		case NODE_OP_EXP:
			if (myNode->getChildrenCount() > 0) {
				// OP EXP
				makeCode(myNode->getChild(1));
				bool isInt;
				if (getType(myNode->getChild(1)) == TYPIFICATION_INT) isInt = true; else isInt = false;
				switch (getType(myNode->getChild(0))) {
					case TYPIFICATION_OP_PLUS:
						if (isInt)
							std::cout << "ADI" << std::endl;
						else
							std::cout << "ADF" << std::endl;
						break;
					case TYPIFICATION_OP_MINUS:
						if (isInt)
							std::cout << "SBI" << std::endl;
						else
							std::cout << "SBF" << std::endl;
						break;
					case TYPIFICATION_OP_MULT:
						if (isInt)
							std::cout << "MLI" << std::endl;
						else
							std::cout << "MLF" << std::endl;
						break;
					case TYPIFICATION_OP_DIV:
						if (isInt)
							std::cout << "DVI" << std::endl;
						else
							std::cout << "DVF" << std::endl;
						break;
					case TYPIFICATION_OP_LESS:
						if (isInt)
							std::cout << "LSI" << std::endl;
						else
							std::cout << "LSF" << std::endl;
						break;
					case TYPIFICATION_OP_EQUAL:
						if (isInt)
							std::cout << "EQI" << std::endl;
						else
							std::cout << "EQF" << std::endl;
						break;
					case TYPIFICATION_OP_AND:
						std::cout << "AND" << std::endl;
						break;
					case TYPIFICATION_OP_NEG:
						std::cout << "AND" << std::endl;
						break;
				}
			} else {
				// €
			}
			break;
			
		case NODE_OP:
			// should never be reached
			break;

		
		case NODE_UNKNOWN:
		case NODE_ERROR:
		case NODE_EMPTY:
		case NODE_IDENTIFIER:
		case NODE_KEYWORD:
		default:
			break;
	}
}

