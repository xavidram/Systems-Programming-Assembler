/* 
 * File:   main.c
 * Author: xavid.ramirez01
 * Created on September 18, 2015, 1:13 PM
 */
#include "prototypes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*int main the function that started it all*/
int main(int argc, char** argv) {

    system("clear");
    printf("Welcome to the FlippinSweet Compiler\n");   //welcome display
    printf("This is contains phase 2. Please type assemble followed by the file name to begin phase 2.\n");
    int lastwordFlag = 1;
    char * command = malloc(128);                       //allocate space

    do{     
            if(lastwordFlag == 1) 
                printf("command>");                      //prompt to put in a command
            memset(command,'\0',127);                   //set command to all null to remove trash
            command[0] = '\0';                          //saftey measure that is not needed but oh well
            lastwordFlag = parseStream(command);                       //go parse the stream and put it into command
        //checkCommand(command,userInput);
            
    }while(compareCommand(command,"exit") != 0);        // exit case, only reason why command is out here in main
     return (EXIT_SUCCESS);
}