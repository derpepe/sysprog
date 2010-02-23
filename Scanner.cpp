#include "Scanner.h"
//#define DEBUG

//
// main constructor for our scanner. inputFile and outputFile needed for buffer
Scanner::Scanner(char* inputFile, char* outputFile)
{
	// Initialisierung
	myBuffer = new Buffer(inputFile, outputFile);
	mySymTable = new SymTable();
	currentState = STATE_START;
	
	for (int i = 0; i < SCANNER_INTERNAL_BUFFER_SIZE; i++) tokenSoFar[i] = '\0';
	internal_token_start = 0;
	internal_pos = 0;
	internal_filled_pos = 0;
	
	column = 1;
	line = 1;
	startColumn = 1;
	startLine = 1;
}

// Loops through file and calls "writeTokenFile()" of the buffer
void Scanner::processFile()
{
	Token *t;

	while (t = this->nextToken()) {
		myBuffer->writeTokenToFile(t);
	}
}

//
// returns a state as a text-string (for debugging output purposes only)
std::string Scanner::intToState(int state)
{
	std::string myResult = "UNKNOWN";

	switch (state)
	{
		case STATE_IDENTIFIER:		myResult = "IDENTIFIER"; break;

		case STATE_INTEGER:			myResult = "INTEGER"; break;

		case STATE_SIGN:
		case STATE_SIGN_OPENBRACE:	myResult = "SIGN"; break;

		case STATE_DOT:				myResult = "DOT"; break;
		case STATE_REAL:
		case STATE_REAL_E:
		case STATE_REAL_PLUSMINUS:
		case STATE_REAL_EXTENDED:	myResult = "REAL"; break;

		case STATE_COMMENT:			myResult = "COMMENT"; break;
		default:
			std::ostringstream temp;
			temp << "UNNAMED_STATE";
			myResult = temp.str();

	}
	return myResult;
}
//
// gets the next token and returns "nil" if EOF
Token *Scanner::nextToken()
{
	Token *myResult;
	do
	{
		myResult = doStep();
		
		// Ignore comments
		if (myResult != NULL) 
		{
			if (myResult->getTokenType() == TOKEN_COMMENT) myResult = NULL;
		}
		
	} while ((internal_pos < internal_filled_pos-1) && (myResult == NULL));
	
	// fill symbol table
	if (myResult != NULL)
	{
		if (myResult->getTokenType() == TOKEN_IDENTIFIER)
		{
			SymTableEntry *theItem = new SymTableEntry(myResult->getLexem(), myResult->getTokenType(), myResult->getLine(), myResult->getColumn());
			
			// search for identifier in hashtable
			bool alreadyInserted = false;
			SymTableKey myKey = mySymTable->getKey(theItem);
		
			SymTableEntry *currentObject = mySymTable->lookup(myKey);
			while (currentObject != NULL)
			{
				if (currentObject->getLexem().compare(myResult->getLexem()) == 0)
				{
					alreadyInserted = true;
					// if symbol is present, use tokenType of symbol in symbol table
					myResult->setTokenType(currentObject->getInfo()->getTokenType());
					break;
				}
				currentObject = currentObject->getLinkedItem();
			}
			
			// insert identifier in hashtable if it is not yet present
			if (!alreadyInserted) {
				mySymTable->insert(theItem);
			}
		}
	}
	
	return myResult;
}

char *s(void *pointer, const int count)
{
	const int c = count + 1;
	char *temp = (char *)malloc(c);
	for (int i = 0; i < c; i++) temp[i] = '\0';
	memcpy(temp, pointer, count);
	return temp;
}

//
// continues one step 
Token * Scanner::doStep()
{
	// build a new token
	Token *myResult = NULL;
	
	//if (myBuffer == NULL) return NULL;

	#ifdef DEBUG
	std::cerr << "Buffer status:" << std::endl;
	std::cerr << "=============================" << std::endl;
	std::cerr << "internal_pos = " << internal_pos << std::endl;
	std::cerr << "internal_filled_pos = " << internal_filled_pos << std::endl;
	std::cerr << "myBuffer->endoffile = " << myBuffer->endoffile << std::endl << std::endl;
	#endif

	// fill buffer if it is not filled yet
	// buffer has to contain at least SCANNER_IDENTIFIER_MAX_SIZE chars
	while ((!myBuffer->endoffile) && ((internal_filled_pos - internal_pos) < SCANNER_IDENTIFIER_MAX_SIZE) && (internal_filled_pos < SCANNER_INTERNAL_BUFFER_SIZE))
	{
		tokenSoFar[internal_filled_pos++] = myBuffer->getChar();
	}
	
	// if buffer reaches it's end, we'll fuckup :)
	if (internal_filled_pos == SCANNER_INTERNAL_BUFFER_SIZE)
	{
		std::cerr << "Scanner:doStep() has filled up the buffer :-/" << std::endl;
	}
	
	#ifdef DEBUG
	std::cerr << "internal_pos = " << internal_pos << std::endl;
	std::cerr << "internal_filled_pos = " << internal_filled_pos << std::endl;
	std::cerr << "myBuffer->endoffile = " << myBuffer->endoffile << std::endl;
	std::cerr << "=============================" << std::endl << std::endl;
	#endif
	
	// EOF-test
	if (internal_pos >= (internal_filled_pos-1)) {
		internal_pos++;
		return NULL;
	}
	

	// begin to get things rolling
	column++;
	if (tokenSoFar[internal_pos] == '\n')
	{
		line++;
		column = 1;
	}

	// switch to new state
	#ifdef DEBUG
	std::cerr << "internal_token_start = " << internal_token_start << std::endl;
	std::cerr << "current state: " << currentState << " (" << Const::stateTypeAsString(currentState) << ") "<< " at internal_pos " << internal_pos << std::endl;
	std::cerr << "input: " << tokenSoFar[internal_pos] << " (ASCII "<< (int)(tokenSoFar[internal_pos]) << ")" << std::endl;
	#endif
	currentState = Automat(currentState, tokenSoFar[internal_pos]);
	#ifdef DEBUG
	std::cerr << "next state: " << currentState << " (" << Const::stateTypeAsString(currentState) << ")" << std::endl << std::endl;
	#endif
	internal_pos++;
	
	// did we reach an error_state?
	if (currentState == STATE_UNKNOWN)
	{
		std::cerr << "Scanner::nextToken() found an unexpected char at line " << line << ", column " << (column - 1) << " "
					<< "\"" << s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start) << "\"" << std::endl;

		// Bei „Zeichen“, die nicht zur Sprache gehören, soll ein Fehlertoken erzeugt werden.
		myResult = new Token(
			TOKEN_ERROR,
			s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start),
			startColumn,
			startLine);
		currentState = STATE_START;
	}
	else
	if (currentState == STATE_ERROR)
	{
		std::cerr << "Scanner::nextToken() ran into an ERROR at line " << line << ", column " << (column - 1) << " "
					<< "\"" << s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start + 1) << "\"" << std::endl;
	}

	// evaluate new state
	if (isPossibleEnding(currentState))
	{
		#ifdef DEBUG
		std::cerr << "-- this is a possible ending:" << std::endl;
		#endif
		// we reached a possible ending state.
		// so here comes the tricky part: We have to look ahead to determin if we
		// can get further (greedy!), or if this *has* to be the end for the
		// current token.

		stateType tempState = currentState;
		int howFarInTheFuture = 0;
		do
		{
			howFarInTheFuture++;
			#ifdef DEBUG
			std::cerr << "--  howFarInTheFuture: " << howFarInTheFuture << std::endl;
			std::cerr << "--  current state: " << currentState << " (" << Const::stateTypeAsString(currentState) << ") " << " at future_pos " << internal_pos + howFarInTheFuture - 1 << std::endl;
			std::cerr << "--  input: " << tokenSoFar[internal_pos + howFarInTheFuture - 1]  << " (ASCII "<< (int)(tokenSoFar[internal_pos]) << ")" << std::endl;
			#endif
			tempState = Automat(tempState, tokenSoFar[internal_pos + howFarInTheFuture - 1]);
			#ifdef DEBUG
			std::cerr << "--  next state: " << currentState << " (" << Const::stateTypeAsString(currentState) << ") " << std::endl << std::endl;
			#endif
		}
		while (!(	// stop if...
			(tempState == STATE_ERROR)			// ...we will get an error state
			|| (isPossibleEnding(tempState)) 	// ...or we find another possible ending
			|| (tokenSoFar[internal_pos + howFarInTheFuture - 1] == '\0')			// ...or we're at the end
			));
		bool theFutureWILLGiveUsOtherPossibilities = isPossibleEnding(tempState);
		#ifdef DEBUG
		std::cerr << "-- finished with result: theFutureWILLGiveUsOtherPossibilities: " << theFutureWILLGiveUsOtherPossibilities << std::endl << std::endl;
		#endif

		if (!theFutureWILLGiveUsOtherPossibilities)
		{
			#ifdef DEBUG
			std::cerr << "++++ Token gefunden!" << std::endl;
			// glassbowl tells us that the future will lead to an error
			// so we will use the current ending possibility.
			std::cerr << "++++ Token Type: " << Const::stateTypeToTokenType(currentState) << " (" << Const::tokenTypeAsString(Const::stateTypeToTokenType(currentState)) << ") " << std::endl;
			std::cerr << "++++ Lexem: " << s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start) << std::endl;
			std::cerr << "++++ Column: " << startColumn << std::endl;
			std::cerr << "++++ Line: " << startLine << std::endl;
			#endif
			myResult = new Token(
				Const::stateTypeToTokenType(currentState),
				s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start),
				startColumn,
				startLine);
			#ifdef DEBUG
			std::cerr << "++++" << std::endl;
			std::cerr << "internal_pos = " << internal_pos << std::endl;
			#endif
			currentState = STATE_START;
		}
		else
		{
			#ifdef DEBUG
			std::cerr << "This is a possible token, but we don't end here. Token type " << intToState(currentState) << " (" << currentState << ")" << " with value \"" << s(&tokenSoFar[internal_token_start], internal_pos - internal_token_start) << "\"" << std::endl;
			#endif
		}
	}

	// reset if necessary
	if (currentState == STATE_START)
	{
		internal_token_start = internal_pos;
		startColumn = column;
		startLine = line;
	}

	return myResult;
}
//
// returns weather a state is a possible ending
bool Scanner::isPossibleEnding(int state)
{
	bool myResult = false;
	switch (state)
	{
		case STATE_IDENTIFIER:
		case STATE_INTEGER:
		case STATE_SIGN:
		case STATE_REAL:
		case STATE_REAL_EXTENDED:
		case STATE_SIGN_OPENBRACE:
		case STATE_COMMENT:
			myResult = true;
	}
	return myResult;
}

//
// returns the classType for any char
classType Scanner::classOf(char theChar)
{
	int myClass = CLASS_UNKNOWN;
	switch(theChar)
	{
		case '0': case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9':
			myClass = CLASS_DIGIT;
			break;
		case 'A': case 'B': case 'C': case 'D': /* E has it's seperate class */
		case 'F': case 'G': case 'H': case 'I': case 'J': case 'K':
		case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q':
		case 'R': case 'S': case 'T': case 'U': case 'V': case 'W':
		case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
		case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x':
		case 'y': case 'z':
			myClass = CLASS_LETTER;
			break;
		case 'E':
			myClass = CLASS_LETTER_E;
			break;
		case '+': case '-':
			myClass = CLASS_SIGN_PLUSMINUS;
			break;
		case '*':
			myClass = CLASS_SIGN_STAR;
			break;
		case '/': case '<': case '>': case '=': case '!': case '&':
		case ';': case ':': case '[': case ']': case '{': case '}':
			myClass = CLASS_SIGN;
			break;
		case '(':
			myClass = CLASS_SIGN_OPENBRACE;
			break;
		case ')':
			myClass = CLASS_SIGN_CLOSEBRACE;
			break;
		case ' ': case '\t': case '\n': case '\r':
			myClass = CLASS_BLANK;
			break;
		case '.':
			myClass = CLASS_DOT;
			break;
	}
	return myClass;
}

//
// returns nextState
stateType Scanner::Automat(stateType state, char nextChar)
{
	int nextState = STATE_UNKNOWN;
	switch(state)
	{
	case STATE_START:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_INTEGER;
				break;
			case CLASS_LETTER:
			case CLASS_LETTER_E:
				nextState = STATE_IDENTIFIER;
				break;
			case CLASS_SIGN:
			case CLASS_SIGN_PLUSMINUS:
			case CLASS_SIGN_CLOSEBRACE:
			case CLASS_SIGN_STAR:
				nextState = STATE_SIGN;
				break;
			case CLASS_SIGN_OPENBRACE:
				nextState = STATE_SIGN_OPENBRACE;
				break;
			case CLASS_BLANK:
				nextState = STATE_START;
				break;
			default:
				state = STATE_ERROR;
		}
		break;
	case STATE_IDENTIFIER:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_IDENTIFIER;
				break;
			case CLASS_LETTER:
			case CLASS_LETTER_E:
				nextState = STATE_IDENTIFIER;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_INTEGER:
		switch(classOf(nextChar))
		{
			case CLASS_DOT:
				nextState = STATE_DOT;
				break;
			case CLASS_DIGIT:
				nextState = STATE_INTEGER;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_SIGN_OPENBRACE:
		switch(classOf(nextChar))
		{
			case CLASS_SIGN_STAR:
				nextState = STATE_IN_COMMENT;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_IN_COMMENT:
		switch(classOf(nextChar))
		{
			case CLASS_SIGN_STAR:
				nextState = STATE_ALMOST_COMMENT;
				break;
			default:
				nextState = STATE_IN_COMMENT;
		}
		break;
	case STATE_ALMOST_COMMENT:
		switch(classOf(nextChar))
		{
			case CLASS_SIGN_CLOSEBRACE:
				nextState = STATE_COMMENT;
				break;
			case CLASS_SIGN_STAR:
				nextState = STATE_ALMOST_COMMENT;
				break;
			default:
				nextState = STATE_IN_COMMENT;
		}
		break;
	case STATE_DOT:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_REAL;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_REAL:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_REAL;
				break;
			case CLASS_LETTER_E:
				nextState = STATE_REAL_E;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_REAL_PLUSMINUS:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_REAL_EXTENDED;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_REAL_E:
		switch(classOf(nextChar))
		{
			case CLASS_SIGN_PLUSMINUS:
				nextState = STATE_REAL_PLUSMINUS;
				break;
			case CLASS_DIGIT:
				nextState = STATE_REAL_EXTENDED;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_REAL_EXTENDED:
		switch(classOf(nextChar))
		{
			case CLASS_DIGIT:
				nextState = STATE_REAL_EXTENDED;
				break;
			default:
				nextState = STATE_ERROR;
		}
		break;
	case STATE_ERROR:
		nextState = STATE_START;
		break;
	default:
		nextState = STATE_ERROR;
	}
	return nextState;
}
