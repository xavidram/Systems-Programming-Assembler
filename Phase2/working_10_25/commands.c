#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

int debugCommand(){
    printf("Debug Comming Soon. Plese enter another command.\n");
    return 1;
}

int executeCommand(){
    printf("Execute Comming Soon. Plese enter another command.\n");
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

int loadCommand(char * word, int lastwordFlag){
    if(lastwordFlag == 1){
        printf("Please enter a file to load:");
        getWord(word);
        printf("Loading "); printf("%s\n",word);
        lastwordFlag = 1;
        parseFile(word);
        return lastwordFlag;
    }
    else{
        getWord(word); 
        printf("Load is comming soon.\n");
        lastwordFlag = 1;
        parseFile(word);
        return lastwordFlag;
    }
}

int assembleCommand(){
    printf("Assemble Comming Soon. Plese enter another command.\n");
    return 1;
}

int dumpCommand(){
    printf("Comming soon\n");
    return 1;
}
int clearCommand(){
    system("clear");
    return 1;
}
/*-----------end of commands section-----------------*/
