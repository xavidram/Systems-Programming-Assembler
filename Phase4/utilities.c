/*
Created by: Xavid Ramirez
Date: 10\25\2015
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"



//opCodes are already in base 10, would have to convert to hex later on.
OPCODES OPTAB[] = {{"ADD", 0x18},{"AND",0x58},{"COMP",0x28},{"DIV",0x24},{"J",0x3c},
                   {"JEQ",0x30},{"JGT",0x34},{"JLT",0x38},{"JSUB",0x48},{"LDA",0x00},
                   {"LDCH",0x50},{"LDL", 0x08},{"LDX", 0x04},{"MUL",0x20},{"OR",0x44},
                   {"RD",0xd8},{"RSU",0x4c},{"STA",0x0c},{"STCH",0x54},{"STL",0x14},
                   {"STX",0x10},{"SUB",0x1c},{"TD",0xe0},{"TIX",0x2c},{"WD",0xdc}};
/*trim line will trim the last character if it is a new line of a string*/
 /*I left this in here for if I need it later on*/
 /*got this from egle's example*/
void trimNewLine(char* Buffer){
    if(Buffer[strlen(Buffer)-1] == '\n')
            Buffer[strlen(Buffer)-1] = '\0';
}

int compareCommand( char* A, char* B){
    while(*A && (*A == *B))         // while a and b same and A sdtill valid
        A++,B++;                    // increment counter
    return *(char*)A-*( char*)B;    // return a 0 if true and a negative 1 if false;
}

int length(char * word){
    int i;
    for(i=0; i<sizeof(word); i++)   //while current caracter word point at is not null keep counting
        if(word[i] == '\0')
            break;
    return i;                       //return the counter number that is the size of the word.
}

/*this is for testing purposes, print out a character array per character*/
void printArray(char * word){
    int i;
    for(i = 0; word[i] != '\0'; i++){
        printf("%c",word[i]);printf(",");
    }
}

/*this will take word from stream at some point in a fuction, parseinput will be initial*/
int getWord(char * word){

    int lastwordFlag = 0;
    memset(word,'\0',127);          //reset word to all null
    word[0] = '\0';                 //saftey measure because c is weird so set first item to null

    int i = 0;char c;       
    while((c = getchar()) != EOF){  //while current character gotten from stream is not end of file
        if(c == ' ')                //check if it is a space
            break;
        else if(c == '\n'){  
            lastwordFlag = 1;       //check if it is a new line
            break;
        }
        else if(c == '\0'){          //check if it is null
            lastwordFlag = 1;
            break;
        }
        
        word[i] = c;                //other wise put the character in word
        i++;                        //increment counter
    }
    return lastwordFlag;
}    

/*----------Commmands list area and their functions -------------------*/
/*this function will check inputed command with the existing commands*/
int checkCommand(char* command, int lastwordFlag){

    if(compareCommand(command,"exit") == 0)             //exit will return to main and check with while loop
        return 1;
    else if(compareCommand(command, "clear") == 0)
        lastwordFlag = clearCommand();
    else if(compareCommand(command, "debug") == 0)
        lastwordFlag = debugCommand();
    else if(compareCommand(command, "dump") == 0)
        lastwordFlag = dumpCommand(lastwordFlag);
    else if(compareCommand(command,"execute") == 0)
        lastwordFlag = executeCommand(lastwordFlag);
    else if(compareCommand(command, "help") == 0)
        lastwordFlag = helpCommand();
    else if(compareCommand(command, "directory") == 0)        
        lastwordFlag = directoryCommand();
    else if(compareCommand(command, "dir") == 0)
        lastwordFlag = directoryCommand();
    else if(compareCommand(command, "load") == 0)
        lastwordFlag = loadCommand(command,lastwordFlag);
    else if(compareCommand(command, "assemble") == 0)
        lastwordFlag = assembleCommand(command,lastwordFlag);
    else{                                                   //prompt user if the command is invalid.
        printf("Invalid command: ");
        printf("%s", command);
        printf(" :Please try again.\n");
    }

    return lastwordFlag;
}

/*what is used to initially take user input and check with command*/
/*same functionallyti as getword but this checks with commands*/
int parseStream(char* word){

    int lastwordFlag;

    int i = 0;char c;
    while((c = getchar()) != EOF){
        if(c == ' ')
            break;
        else if(c == '\n'){
            lastwordFlag = 1;
            break;
        }
        else if(c == '\0'){
            lastwordFlag = 1;
            break;
        }
        
        word[i] = c;
        i++;
    }

    if(i == 0){
        printf("Please enter a ");
    }else{
        lastwordFlag = checkCommand(word, lastwordFlag);
    }

    return lastwordFlag;
}

//phase 1 prototypes
int isMnemonic(char * mnemonic){
    int i;
    for (i=0; i < sizeof(OPTAB); i++){
        if(!strcmp(OPTAB[i].Mnemonic,mnemonic))
            return 1;
    }
    return 0;
}

unsigned getMnemonicVal(char * mnemonic){
    int i;
    for(i=0; i<sizeof(OPTAB);i++){
        if(!strcmp(OPTAB[i].Mnemonic,mnemonic))
            return OPTAB[i].code;
    }
    return 0;
}

/*This function will hold all errors so we can reference them later*/
int errorDecode(int errorVal){
    switch(errorVal){
        case 1:
            printf("Missing or illegal operand on START Directive");
            break;
        case 2:
            printf("Duplicate Labels");
            break;
        case 3:
            printf("Illegal Label");
            break;
        case 4:
            printf("Illegal Operation");
            break;
        case 5:
            printf("Missing or illegal operand on data storage directive.");
            break;
        case 6:
            printf("Missing or illegal operand on END Directive");
            break;
        case 7:
            printf("Too many symbols in source program");
            break;
        case 8:
            printf("Program too long");
            break;
        case 9:
            printf("Program Exceeds memory allocation");
            break;
        case 10:
            printf("No Program Name");
            break;
        case 11:
            printf("Missing End Operand");
            break;
        case 12:
            printf("Label too large");
            break;
        case 13:
            printf("OpCode too large");
            break;
        case 14:
            printf("Operand too large");
            break;
        case 15:
            printf("Illegal Operand in START statement");
            break;
        case 16:
            printf("Illegal Operand in BYTE statement");
            break;
        case 17:
            printf("Illegal Operand in WORD statement");
            break;
        case 18:
            printf("Illegal Operand in RESW statement");
            break;
        case 19:
            printf("Illegal Operand in RESB statement");
            break;
        case 20:
            printf("Odd length hex string in BYTE statement");
            break;
    }
}
//regular print of error array
void printIntArray(int *Array,int size){
    if(size == 0){
        printf("\n");
    }
    int i;
    for(i = 0; i < size; i++){
        printf("%d ",Array[i]);
    printf("\n");
    }
    
}

//print to file error array
void fprintIntArray(int *Array,int size,FILE *fw){
    if(size == 0){
        fprintf(fw,"\n");
    }
    else{
        int i;
        for(i = 0; i < size; i++){
            fprintf(fw, "%d ",Array[i]);
        }
        fprintf(fw,"\n");
    }
}

void writeListingLine(FILE * Lfw,char* LOCCTR, char* label, char* opcode, char* operand, char* mnemonicVal, unsigned operandVal){
        
        fprintf(Lfw,"%s\t",LOCCTR);

        if(!isspace(label[0]))
            fprintf(Lfw,"%*s\t",2,label);
        else
            fprintf(Lfw,"%s\t","----");


        fprintf(Lfw,"%s\t",opcode);

        //cases for operand
        if(!strcmp(opcode,"RSUB")){
            fprintf(Lfw,"%s\t"," ");
        }else{
            fprintf(Lfw,"%s\t",operand);
        }
    
        if(!strcmp(opcode,"BYTE")){
            char * check; check = malloc(sizeof(char*)*32); memset(check,'\0',32);
            char* Asc; Asc = malloc(sizeof(char*)*4); memset(Asc,'\0',4);

            if(operand[0] == 'C' && operand[1] == '\''){
                int k; 
                for(k = 2;operand[k] != '\'';k++){
                    sprintf(Asc,"%02x",operand[k]);
                    strcat(check,Asc);
                }
            }else if(operand[0] == 'X' && operand[1] == '\''){
                int l;int i = 0;
                char * Asc[2];Asc[1] = '\0';
                for(l=2;operand[l] != '\'';l++){
                    i++;
                    Asc[0] = operand[l];
                    strcat(check,Asc);
                }
            }
            fprintf(Lfw,"%s\n",check);
        }else if(!strcmp(opcode,"RESW") || !strcmp(opcode,"RESB")){
            fprintf(Lfw,"\n");
        }else if(!strcmp(opcode,"WORD")){
            int x = (int)strtoul(operand,NULL,10);
            fprintf(Lfw,"%06d\n",x);
        }else{
            fprintf(Lfw,"%s",mnemonicVal);
            fprintf(Lfw,"%x\n",operandVal);
        }
}