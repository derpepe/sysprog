#include "main_scanner.h"


int main(int argc, char* argv[], char* envp[])
{
	Scanner *s;
	//tests - 	s = new Scanner("C:/projekte/C++/Automat/source/scanner/scanner1.txt", "out1siehher1.txt");
	// parse arguments
	if (argc <= 1)
	{
		std::cerr << "Syntax: " << argv[0] 
		 << " Inputfile [Outputfile]" << std::endl;
		return 1;
	}
	else if (argc <= 2)
	{
		// std::cerr << "no output file given. Assuming \"out.txt\"." << std::endl;
		s = new Scanner(argv[1], (char *)"out.txt");
	}
	else
	{
		s = new Scanner(argv[1], argv[2]);
	}

	s->processFile();
	
	// s->mySymTable->printHashTable();
}
