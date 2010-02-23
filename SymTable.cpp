#include "SymTable.h"

SymTable::SymTable()
{
	for(int x = 0; x< HASHTABLE_MAX_SIZE; x++){
		Table[x] = NULL;
	}
	this->initSymbols();
}

void SymTable::initSymbols()
{
	this->insert(new SymTableEntry("write", TOKEN_WRITE, 0, 0));
	this->insert(new SymTableEntry("read", TOKEN_READ, 0, 0));
	this->insert(new SymTableEntry("if", TOKEN_IF, 0, 0));// "if" aus Aufgabe 2
	this->insert(new SymTableEntry("else", TOKEN_ELSE, 0, 0));// "else" aus Aufgabe 2
	this->insert(new SymTableEntry("while", TOKEN_WHILE, 0, 0));// "while" aus Aufgabe 2
	this->insert(new SymTableEntry("int", TOKEN_INT, 0, 0));// "int" aus Aufgabe 2
	this->insert(new SymTableEntry("float", TOKEN_FLOAT, 0, 0));// "float" aus Aufgabe 2
}

SymTableKey SymTable::insert(SymTableEntry *newItem)
{
	int index = GetIndex(newItem);
	SymTableEntry *itemAtIndex = Table[index];
	if (itemAtIndex == NULL)
	{
		// no item yet, so this is the first one
		Table[index] = newItem;
	}
	else
	{
		// get last item
		
		// itemAtIndex ist hier != NULL
		SymTableEntry *nextItem = itemAtIndex->getLinkedItem();
		while (nextItem != NULL)
		{
			itemAtIndex = nextItem;
			nextItem = itemAtIndex->getLinkedItem();
		}
		// nextItem ist hier "NULL"
		// Wir fügen das neue Element am Ende der Liste der Einträge unter diesem Index hinzu
		itemAtIndex->setLinkedItem(newItem);
	}
	return index;
}

// Hash-Formel wird hier implementiert. Wir benutyen mal das Lexem und den TokenType zum hashen.
// Kommentare sind ausdruecklich erwuenscht :)
SymTableKey SymTable::GetIndex(SymTableEntry *searchKey) 
{
	int hashIndex = 0;

	std::string myLexem = searchKey->getLexem();
	const char *lexem = myLexem.c_str();
	for (unsigned int i = 0; i < strlen(lexem); i++)
	{
		hashIndex += (int)lexem[i] * (10^i);
	}

	return hashIndex % (HASHTABLE_MAX_SIZE - 1); // is needed so the index is always in range
} 

SymTableKey SymTable::getKey(SymTableEntry *theItem)
{
	return GetIndex(theItem);
}

SymTableEntry *SymTable::lookup(SymTableKey myKey)
{
	return Table[myKey]; // always returns first item of list
}

void SymTable::printHashTable()
{
	for (int i = 0; i < HASHTABLE_MAX_SIZE; i++)
	{
		std::cout << "Now the item at index " << i << ":" << std::endl;

		SymTableEntry *currentItem = Table[i];
		if (currentItem != NULL)
		{
			std::cout << "  '" << currentItem->getLexem() << "' of tokentype '" << Const::tokenTypeAsString(currentItem->getInfo()->getTokenType())
				<< "' with typification '" << parserConst::typificationAsString(currentItem->getTypification())
				<< "' at " << currentItem->getInfo()->getColumn() << " x " << currentItem->getInfo()->getLine() << std::endl;
			
			SymTableEntry *nextItem = currentItem->getLinkedItem();		
			while (nextItem != NULL)
			{
				currentItem = nextItem;
				if (currentItem == NULL)
				{
					std::cerr << "NULL item :-(" << std::endl;
					return;
				}
				else
				{
					std::cout << "  '" << currentItem->getLexem() << "' of tokentype '" << Const::tokenTypeAsString(currentItem->getInfo()->getTokenType())
						<< "' with typification '" << parserConst::typificationAsString(currentItem->getTypification())
						<< "' at " << currentItem->getInfo()->getColumn() << " x " << currentItem->getInfo()->getLine() << std::endl;
					nextItem = currentItem->getLinkedItem();
				}
			}
		}
		std::cout << "no more items at this point" << std::endl << std::endl;
	}
}

SymTable::~SymTable(void)
{
}
