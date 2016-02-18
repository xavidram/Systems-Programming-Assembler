#ifndef _prototypes_h_
	#define _prototypes_h_

//commands
	int clearCommand();
	int dumpCommand();
	int asembleCommand();
	int directoryCommand();
	int loadCommand(char* word, int lastwordFlag);
	int helpCommand();
	int executeCommand();
	int debugCommand();

	//mnemonics table stuff

	typedef struct{
	    char Mnemonic[7];   //7 bits in sice for the op codes as advised by egle
	    short code;
	} OPCODES;

	//place hash table for symbol table here
	typedef struct{
	    char label[7]; 
	    short LOCCTR;
	} SymbolTable;


	void trimNewLine(char * Buffer);
	int compareCommand(char* A, char* B);
	int length(char * word);
	void printArray(char * word);
	int getWord(char * word);
	int parseStream(char* word);
	int checkCommand(char * command, int lastwordFlag);
#endif