/*
Created by: Xavid Ramirez
Date: 10\25\2015
*/
#ifndef _prototypes_h_
	#define _prototypes_h_

//constants
	#define TRUE    1                       /* Boolean constants */
	#define FALSE   0
	#define MSIZE   32768L                  /* maximum memory size */
	#define	MSYMBOL 600
	#define LABLEMAX 7


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
	   	unsigned code;
	} OPCODES;
	//place hash table for symbol table here
	typedef struct{
	    char label[7]; 
	    unsigned LOCCTR;
	} SymbolTable;
	void trimNewLine(char * Buffer);
	int compareCommand(char* A, char* B);
	int length(char * word);
	void printArray(char * word);
	int getWord(char * word);
	int parseStream(char* word);
	int checkCommand(char * command, int lastwordFlag);
	//phase1
	void parseFile(char * fileName);
	char *HexToDecimal(char * string);
	char *DecimalToHex(char * string);
	char *incrementLOCCTR(char * LOCCTR, int amount);
		//opcode table
		int isMnemonic(char * mnemonic);

	//phase 3 stuff
	void writeHeaderRecord(char * programName, char * startingAddress, char * programLength);
	void writeTextRecord(unsigned address, unsigned recordLength,int reserved);
	void writeEndRecord(unsigned staringAddress);

#endif