#pragma once

#define SymTableKey int

#include <iostream>
#include <String>
#include "Info.h"
#include "Lexem.h"
#include "Const.h"
#include "SymTableEntry.h"


class SymTable
{
	public:
		SymTable(void);
		~SymTable(void);
	
		SymTableKey insert(SymTableEntry *newItem);
		SymTableKey getKey(SymTableEntry *theItem);
		SymTableEntry *lookup(SymTableKey myKey);

		void printHashTable();

	protected: 
		int GetIndex(SymTableEntry *searchKey);

	private:
		void initSymbols();

		SymTableEntry *Table[HASHTABLE_MAX_SIZE];
};
