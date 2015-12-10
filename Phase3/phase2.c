#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototypes.h"

/*---------PHASE 2 --------------------------------------*/
void parseFile(char * fileName){

    FILE *fr,*fw,*Ifr,*Ofw, *Lfw;
    SymbolTable *SYMTAB[MSYMBOL];
    int sCount = 0;

    char *line;                             //Initialize the line array
    int LINELENGTH = 128;
    line = malloc(sizeof(char*) * LINELENGTH);

    //allocate memory to a couple of arrays
    char *label, *opcode, *operand, *programName, *programStart;                               
    unsigned LOCCTR = 0;                                  

    //maxprogramsize
    unsigned MAX_PROGRAM_SIZE = 0x7FFF;                     //maxmimum pogram size, in hexidecimal
    unsigned PROGRAM_LENGTH, NEXT_LOCATION, STARTING_ADDRESS;

    //initialize errors array
    int * errors;
    int MAXERRORS = 20;
    errors = (int*)calloc(MAXERRORS,sizeof(int));
    int errorCount = 0; int totalErrors = 0;
    unsigned mnemonicVal;                                   //usigned varable to hold hex value of mnemonic
    unsigned val;                                           //val to do the mathimatical arithmetic

    //defining output file 
    //opening the file
    
    fr = fopen(fileName,"r");
        //if the file is invalid
        if(fr == NULL){
            printf("Can't open the input file."); exit(1);
        }
    fw = fopen("intermediate.txt","w");
        //if output file is not there
        if(fw == NULL){
            printf( "Output file not located."); exit(1);
        }
    
    
    memset(line,'\0',sizeof(line));                                   //reset line
    fgets(line,127,fr);
    fprintf(fw,"%s\n",line);

    //For First Line

        if(line[0] == '.'){
            //this is a comment line;
        }
        else if(isspace(line[0])){                            //if not a labael then set label first character to a space for flag check
            label[0] = ' ';
            opcode = strtok(line," ");operand = strtok(NULL," ");   //tokenize opcode and line
            trimNewLine(opcode);trimNewLine(operand);                   //trim each token to ensre no new line in each of them
        }
        else{                                                 //if labbel exist then tokenize all 3 variables
            programName = strtok(line," ");opcode = strtok(NULL," ");operand = strtok(NULL," ");  //but instead of being called label, it is programname
            trimNewLine(programName);trimNewLine(opcode);trimNewLine(operand);  //trim any new lines
        }

        //START rror flag, if START found thn set LOCCTR to operand value
        if(!strcmp(opcode,"START")){
            STARTING_ADDRESS = (int)strtoul(operand,NULL,16);
            LOCCTR = (int)strtoul(operand,NULL,16);
        }else{                                              //otherwise set LOCCTR to 0, set error flag for this 
            LOCCTR = 0; errors[errorCount] = 1; errorCount++; totalErrors++;
        }

        //print first line -> The source line;
        fprintf(fw, "%x \n",LOCCTR);
        fprintf(fw, "%s \n",operand);
        fprintIntArray(errors,errorCount,fw);


    NEXT_LOCATION = LOCCTR;                             //set NEXT_LOCATION to LOCCTR to I do not have to change LOCCTR.

    memset(line,'\0',127);
    memset(errors,0,sizeof(errors));
    errorCount=0;
    fgets(line,127,fr);                                                         //getLine
    printf("HERE\n");
    


    while(!feof(fr)){
        
        //parse line
        trimNewLine(line);
        fprintf(fw,"%s\n",line);
        printf("%s\n",line);

        if(line[0] == '.'){ //this is a comment line;
            //
        }
        else if(isspace(line[0])){ 
            label[0] = ' '; opcode = strtok(line," ");
            trimNewLine(opcode);
            
            if(!strcmp(opcode,"RSUB")){                                             //but check if opcoe is rsub, then it doesnt need a operand
                strcpy(operand,"");                                                 //so set operand to a blank and it's neonic val to 0
                mnemonicVal = 0;                                                    //mnemonic val 0.
            }else{
                operand = strtok(NULL," ");trimNewLine(operand);                  //otherwise tokenize operand and trim it.
            }
        }
        else{                                                                       //if all 3 exist then tokenize all 3 and trim
            label = strtok(line," ");opcode = strtok(NULL," ");operand = strtok(NULL," ");
            trimNewLine(label);trimNewLine(opcode);trimNewLine(operand);
        }

        if(line[0] != '.'){
            //if label feild is not empty then search for symbol.
            if(label[0] != ' '){
                //Search symbol Table
                    int i;int exist = 0;
                    for(i=0; i < sCount; i++){
                        if(!strcmp(SYMTAB[i]->label,label)){
                            exist = 1; break;
                        }
                    }
                //End search in symbol Table

                if(exist == 1){
                    errors[errorCount] = 2; errorCount++; totalErrors++;
                }else{
                    SymbolTable *newLabel = malloc(sizeof(SymbolTable));
                    strcpy(newLabel->label,label);
                    newLabel->LOCCTR = LOCCTR;
                    SYMTAB[sCount] = newLabel;
                    sCount++;
                }
            }

            if(isMnemonic(opcode)){
                mnemonicVal = getMnemonicVal(opcode);
                LOCCTR = LOCCTR + 3;
            }
            else if(!strcmp(opcode,"WORD")){
                LOCCTR = LOCCTR + 3;
            }
            else if(!strcmp(opcode,"RESW")){
                val = (int)strtoul(operand,NULL,16); val = val * 3;
                LOCCTR = LOCCTR + val;
            }
            else if(!strcmp(opcode,"RESB")){
                val = (int)strtoul(operand,NULL,10);
                LOCCTR = LOCCTR + val;
            }
            else if(!strcmp(opcode,"BYTE")){
                int len = length(operand); int X = 2; int SIZE = 0;

                printf("%d\n",(operand[0] == 'C') && (operand[1] == '\''));

                if( (operand[0] == 'C') && (operand[1] == '\'')){
                    while(X < len - 1){
                        SIZE++; X++;
                    }
                }else if( (operand[0] == 'X') && (operand[1] == '\'')){
                    while(X < len - 1){
                        SIZE++,X++;
                    }
                        if(SIZE % 2 != 0){
                            errors[errorCount] = 4; errorCount++; totalErrors++;
                        }
                        else
                            SIZE = SIZE/2;
                }
                else{
                    errors[errorCount] = 4; errorCount++; totalErrors++;
                }

                LOCCTR = LOCCTR + SIZE;
            }else{
                if(strcmp(opcode,"RSUB")){
                    errors[errorCount] = 5; errorCount++; totalErrors++;
                }
                    
            }

            if(LOCCTR > MSIZE){
                errors[errorCount] = 9; errorCount++; totalErrors++;
            }
            if(sCount >= MSYMBOL){
                errors[errorCount] = 8; errorCount++; totalErrors++;
            }

        //write to file
        fprintf(fw, "%x\n",mnemonicVal);
        fprintf(fw,"%x\n",LOCCTR);
        fprintf(fw,"%s\n",operand);
        fprintIntArray(errors,errorCount,fw);

        printf("%x\n",mnemonicVal);
        printf("%x\n",LOCCTR);
        printf("%s\n",operand);
        printIntArray(errors,errorCount,fw);

        }//End if not comment/
        //Reset the data



        if(!strcmp(opcode,"END")){
            break;
        }

        memset(line,'\0',127);
        memset(errors,0,sizeof(errors));
        errorCount=0;
        fgets(line,127,fr);
        PROGRAM_LENGTH = LOCCTR;

    }//end while not END

    fprintf(fw,"%s\n",line);
    PROGRAM_LENGTH = LOCCTR;
    printf("%d\n",totalErrors);

    //this is just to print table so we can see it on the intermediate file for now. Will change it later. 
    printf("\nThis is the Symbol Table:\n");
    int p;
    for(p = 0; p < sCount; p++){
        printf("%s ",SYMTAB[p]->label);printf("%x\n",SYMTAB[p]->LOCCTR);
        fprintf(fw,"%s ",SYMTAB[p]->label);fprintf(fw,"%x\n",SYMTAB[p]->LOCCTR);
    }
    //close so file gets written
    fclose(fr); fclose(fw);

    //Pass 2

    Ifr = fopen("intermediate.txt","r");
        //if the file is invalid
        if(Ifr == NULL){
            printf("Can't open the input file."); exit(1);
        }
    Ofw = fopen("program.obj","w");
        if(Ofw == NULL){
            printf("Can't open the OBJECT File"); exit(1);
        }
    Lfw = fopen("list.txt","w");
        if(Lfw == NULL){
            printf("Can not open the listing file"); exit(1);
        }

    memset(line,'\0',127);
    

    unsigned lineAddress;
    char * readMnemonicVal, * readLineAddress, * readOperand, *readErrors;

    //use mnemonicVal for value of mnemonic
    //use error array to check errors
    line = malloc(sizeof(char*) * 128);
    readMnemonicVal = malloc(sizeof(char*) * 10);
    readLineAddress = malloc(sizeof(char*) * 10);
    readOperand = malloc(sizeof(char*) * 10);
    readErrors =  malloc(sizeof(char*) * 10);

    int lineNumber;
    for(lineNumber = 1; !feof(Ifr); lineNumber++){
        
        errorCount = 0;

        memset(line,'\0',127);
        fgets(line, 127, Ifr);
        trimNewLine(line);

        printf("%s\n",line);


        if(line[0] != '.'){
            if(isspace(line[0])){
                label[0] = ' '; opcode = strtok(line," ");
                trimNewLine(opcode);
                
                if(!strcmp(opcode,"RSUB")){                                             //but check if opcoe is rsub, then it doesnt need a operand
                    strcpy(operand,"");                                                 //so set operand to a blank and it's neonic val to 0
                    mnemonicVal = 0;                                                    //mnemonic val 0.
                }else{
                    operand = strtok(NULL," ");trimNewLine(operand);                  //otherwise tokenize operand and trim it.
                }
            }else{                                                              //if all 3 exist then tokenize all 3 and trim
                label = strtok(line," ");opcode = strtok(NULL," ");operand = strtok(NULL," ");
                trimNewLine(label);trimNewLine(opcode);trimNewLine(operand);
            }

            if(!strcmp(opcode,"END")){
                break;
            }

            memset(readMnemonicVal,'\0',10); memset(readLineAddress,'\0',10);
            memset(readOperand,'\0',10);     memset(readErrors,'\0',10);

            fgets(readMnemonicVal,9,Ifr); fgets(readLineAddress,9,Ifr);
            fgets(readOperand,9,Ifr); fgets(readErrors,9,Ifr);

            trimNewLine(readMnemonicVal);trimNewLine(readLineAddress); trimNewLine(readOperand);
            trimNewLine(readErrors);

            printf("%s\n",readMnemonicVal);
            printf("%s\n",readLineAddress);
            printf("%s\n",readOperand);

            printf("%s\n",readErrors);

        }

        /*
        


        //if it is the first line and opcode is start
        if(lineNumber == 1 && !strcmp(opcode,"START")){

        }else if(lineNumber == 1){

        }

        //if not a label
        if(label[0] != '.'){
            if(isMnemonic(opcode))
        }  
*/


    }

    fclose(Ifr); fclose(Ofw); fclose(Lfw);

    
}