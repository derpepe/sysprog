#pragma once

#include "Const.h"


class Info
{
	public:
		Info(tokenType myTokenType, int myLine, int myColumn);
		~Info(void);
		
		void setTokenType(tokenType myTokenType);
		void setLine(int myLine);
		void setColumn(int myColumn);
		
		tokenType getTokenType();
		int getLine();
		int getColumn();
	
	private:
		tokenType type;
		int line;
		int column;
};
