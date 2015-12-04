/* 
 * File:   main.c
 * Author: xavid.ramirez01
 *
 * Created on September 18, 2015, 1:13 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//get trim and trimnewline working

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
void getWord(char * word){

    memset(word,'\0',127);          //reset word to all null
    word[0] = '\0';                 //saftey measure because c is weird so set first item to null

    int i = 0;char c;       
    while((c = getchar()) != EOF){  //while current character gotten from stream is not end of file
        if(c == ' ')                //check if it is a space
            break;
        else if(c == '\n'){         //check if it is a new line
            break;
        }
        else if(c == '\0')          //check if it is null
            break;
        
        word[i] = c;                //other wise put the character in word
        i++;                        //increment counter
    }

    if(i == 0){
        printf("Please enter a new word");
    }
}
    

/*----------Commmands list area and their functions -------------------*/

void debugCommand(){
    printf("Debug Comming Soon. Plese enter another command.\n");
}

void executeCommand(){
    printf("Execute Comming Soon. Plese enter another command.\n");
}

void helpCommand(){
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
}

void directoryCommand(){
    system("ls");
}

void loadCommand(char * word){
    printf("Load is comming soon.\n");
}

void assembleCommand(){
    printf("Assemble Comming Soon. Plese enter another command.\n");
}

void dumpCommand(){
    printf("Comming soon\n");
}
void clearCommand(){
    system("clear");
}
/*-----------end of commands section-----------------*/

/*this function will check inputed command with the existing commands*/
void checkCommand(char* command){

    if(compareCommand(command,"exit") == 0)             //exit will return to main and check with while loop
        return;
    else if(compareCommand(command, "clear") == 0)
        clearCommand();
    else if(compareCommand(command, "debug") == 0)
        debugCommand();
    else if(compareCommand(command, "dump") == 0)
        dumpCommand();
    else if(compareCommand(command,"execute") == 0)
        executeCommand();
    else if(compareCommand(command, "help") == 0)
        helpCommand();
    else if(compareCommand(command, "directory") == 0)        
        directoryCommand();
    else if(compareCommand(command, "dir") == 0)
        directoryCommand();
    else if(compareCommand(command, "load") == 0)
        loadCommand(command);
    else if(compareCommand(command, "assemble") == 0)
        assembleCommand();
    else{                                                   //prompt user if the command is invalid.
        printf("Invalid command: ");
        printf("%s", command);
        printf(" :Please try again.\n");
    }
}

/*what is used to initially take user input and check with command*/
/*same functionallyti as getword but this checks with commands*/
void parseStream(char* word){

    int i = 0;char c;
    while((c = getchar()) != EOF){
        if(c == ' ')
            break;
        else if(c == '\n')
            break;
        else if(c == '\0')
            break;
        
        word[i] = c;
        i++;
    }

    if(i == 0){
        printf("Please enter a new word");
    }else{
        checkCommand(word);
    }

}


/*int main the function that started it all*/
int main(int argc, char** argv) {

    system("clear");
    printf("Welcome to the FlippinSweet Compiler\n");   //welcome display

    char * command = malloc(128);                       //allocate space

    do{      
            printf("command>");                         //prompt to put in a command
            memset(command,'\0',127);                   //set command to all null to remove trash
            command[0] = '\0';                          //saftey measure that is not needed but oh well
            parseStream(command);                       //go parse the stream and put it into command
        //checkCommand(command,userInput);
            
    }while(compareCommand(command,"exit") != 0);        // exit case, only reason why command is out here in main
    
    return (EXIT_SUCCESS);
}


