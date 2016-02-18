/*
Created by: Xavid Ramirez
Date: 10\25\2015
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

int debugCommand(){
    printf("Debug Comming Soon. Plese enter another command.\n");
    return 1;
}
int executeCommand(int locctr){
    SICRun(locctr,0);
    return 1;
}
int helpCommand(){
    printf("-- assembly filename");
        printf(" will assemble an SIC assembly language program into a load module and store it in a file.\n");
    printf("-- debug");
        printf(" will allow you to execute in debug mode.\n");
    printf("-- directory");
        printf(" will list the files stored in the current directory\n");
    printf("-- dump start end");
        printf(" will call the dump function, passing the values of start and end.\n");
    printf("-- execute");
        printf(" Will call the computer simulation program to execute the program that was previosly loaded in memory.\n");
    printf("-- exit");
        printf(" will exit the simulator.\n");
    printf("-- help");
        printf(" list all supported commands.\n");
    printf("-- load filename");
        printf(" The load command will take a file name and load it onto the system.\n");    

    return 1;
}
int directoryCommand(){
    system("ls");
    return 1;
}

ADDRESS* loadCommand(char * word, int lastwordFlag){
    if(lastwordFlag == 1){
        printf("Please enter a file to load:");
        getWord(word);
        
        lastwordFlag = 1;
    }
    else{
        getWord(word); 
        
        lastwordFlag = 1;
    }
    FILE *Ofile;
    Ofile = fopen(word,"r");

    char * OBJECTS; OBJECTS = malloc(sizeof(char*)*500);
    BYTE byte;
    char byteVal[2];
    char Linelength[2];
    //unsigned  intLength;
    int intLength;
    char StartAddress[6];
    //unsigned intStartAddress;
    int intStartAddress;
    char EndAddress[6];
    ADDRESS * endAddressReturn; endAddressReturn = (ADDRESS*)malloc(sizeof(int));
    int returnAddresReference;

    int objectCounter;

    while(!feof(Ofile)){
        //read the line
        fgets(OBJECTS,200,Ofile);

        //ignore header
        if(OBJECTS[0] == 'H'){
            //ignore
        }
        //process text records
        else if(OBJECTS[0] == 'T'){
            //process
            //ignore first two zeros and header character  so start at like 3 and you take 4 characters
            //substring didnt work,
            strncpy(StartAddress,OBJECTS+3,4);
            //doesnt stop, think missing null
            StartAddress[4] = '\0';
            //next two is the size of line so it should be from 7 to 9
            strncpy(Linelength,OBJECTS+7,2);
            //printf("breaks here");
            Linelength[2] = '\0';
            //from 10 to 69 is objects every 6

            //convert to decimal
            intLength = (int)strtoul(Linelength,NULL,16);
            intStartAddress = (int)strtoul(StartAddress,NULL,16);

            //first two bytes at 9
            objectCounter = 9;

            int i;
            for(i =0; i < intLength;i++){
                //printf("works up to here");
                strncpy(byteVal,OBJECTS + objectCounter,2);
                byte = strtoul(byteVal,NULL,16);
                //update counter position
                intStartAddress++;
            }
        }
        //if end record then finish up
        else if(OBJECTS[0] == 'E'){
            //printf("break/work");
            //end object starts after first char and two usless zero
            strncpy(EndAddress,OBJECTS+3,4);
            //printf("breaks here why");
            EndAddress[4] = '\0';
            returnAddresReference = (int)strtoul(EndAddress,NULL,16);
        }
    }
    //endAddressReturn = returnAddressReference;
    *endAddressReturn = returnAddresReference;
    //return endAddressReturn;
    return *endAddressReturn;
    fclose(Ofile);
}

int assembleCommand(char * word, int lastwordFlag){
    if(lastwordFlag == 1){
        //system("ls");printf("\n");
        printf("Please enter a file to assemble:");
        getWord(word);
        printf("Loading "); printf("%s\n",word);
        lastwordFlag = 1;
        parseFile(word);
        return lastwordFlag;
    }
    else{
        getWord(word); 
        lastwordFlag = 1;
        parseFile(word);
        return lastwordFlag;
    }
}


int dumpCommand(int lastwordFlag){
    /*
    char * word;
    printf("please enter your start and end");
    getWord(word);
    int start = (int)strtoul(word,NULL,16);
    getWord(word);
    int end = (int)strtoul(word,NULL,16);

        BYTE MEMORY;

    int c = 0;
    printf("START");printf("%d\t\n",start);

    for(c = start;c < end; c++){
        if(c == start){
            printf("%x=>",c);
        }

        GetMem(c,&MEMORY,0);

        if( ( c - start ) %16 == 0 && c != start){
            printf("%d\n",c);
        }
        printf("%02x",MEMORY);
    }

    printf("END");printf("%d\t\n",end);

*/
    return 1;
}
int clearCommand(){
    system("clear");
    return 1;
}
/*-----------end of commands section-----------------*/
