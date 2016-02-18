#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prototypes.h"

unsigned symbolLocation(SymbolTable ** SYMTAB,char * label,int sCount){
    int i;
    for(i=0; i < sCount; i++){
        if(!strcmp(SYMTAB[i]->label,label)){
            return SYMTAB[i]->LOCCTR;
        }
    }
    unsigned r = 0000;
    return r;
}

int symbolExists(SymbolTable ** SYMTAB, char * label, int sCount){
    int i;
    for(i=0; i < sCount; i++){
        if(!strcmp(SYMTAB[i]->label,label)){
            return 1;
        }
    }
    return 0;
}

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
    unsigned currentADDRESS;                         

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
    trimNewLine(line);
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
        fprintf(fw, "%s\n",programName);
        fprintf(fw, "%s\n", opcode);
        fprintf(fw, "0\n");
        fprintf(fw, "%x\n",LOCCTR);
        fprintf(fw, "%s\n",operand);
        fprintIntArray(errors,errorCount,fw);


    NEXT_LOCATION = LOCCTR;                             //set NEXT_LOCATION to LOCCTR to I do not have to change LOCCTR.

    memset(line,'\0',127);
    memset(errors,0,sizeof(errors));
    errorCount=0;
    fgets(line,127,fr);                                                         //getLine
    


    while(!feof(fr)){
        
        //parse line
        trimNewLine(line);
        fprintf(fw,"%s\n",line);

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

            currentADDRESS = LOCCTR;
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
                LOCCTR = LOCCTR + 3;
            }

            if(LOCCTR > MSIZE){
                errors[errorCount] = 9; errorCount++; totalErrors++;
            }
            if(sCount >= MSYMBOL){
                errors[errorCount] = 8; errorCount++; totalErrors++;
            }

        //write to file
        if(label[0] == ' ')
            fprintf(fw,"\n");
        else
            fprintf(fw,"%s\n",label);
        fprintf(fw,"%s\n",opcode);
        fprintf(fw, "%x\n",mnemonicVal);
        fprintf(fw,"%x\n",currentADDRESS);
        fprintf(fw,"%s\n",operand);
        fprintIntArray(errors,errorCount,fw);

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

    //this is just to print table so we can see it on the intermediate file for now. Will change it later. 
   // printf("\nThis is the Symbol Table:\n");
    int p;
    for(p = 0; p < sCount; p++){
        //printf("%s ",SYMTAB[p]->label);printf("%x\n",SYMTAB[p]->LOCCTR);
        fprintf(fw,"%s ",SYMTAB[p]->label);fprintf(fw,"%x\n",SYMTAB[p]->LOCCTR);
    }
    //close so file gets written
    fclose(fr); fclose(fw);

    //printf("%x\n",PROGRAM_LENGTH);

    //Pass 2

    Ifr = fopen("intermediate.txt","r");
        //if the file is invalid
        if(Ifr == NULL){
            printf("Can't open the input file."); exit(1);
        }
    Ofw = fopen("program.txt","w");
        if(Ofw == NULL){
            printf("Can't open the OBJECT File"); exit(1);
        }
    Lfw = fopen("list.txt","w");
        if(Lfw == NULL){
            printf("Can not open the listing file"); exit(1);
        }

    memset(line,'\0',127);
    

    unsigned lineAddress;
    char * readMnemonicVal, * readLineAddress, * readOperand, *readErrors,* readLabel,* readOpcode;

    //use mnemonicVal for value of mnemonic
    //use error array to check errors
    line = malloc(sizeof(char*) * 128);
    readLabel = malloc(sizeof(char*) * 10);
    readOpcode = malloc(sizeof(char*) * 10);
    readMnemonicVal = malloc(sizeof(char*) * 10);
    readLineAddress = malloc(sizeof(char*) * 10);
    readOperand = malloc(sizeof(char*) * 12);
    readErrors =  malloc(sizeof(char*) * 10);
    int newSymbolCount = 0; int instruction = 1;
    unsigned operandAddress;
    unsigned lineSize = 0;
    char * instructionLines; instructionLines = malloc(sizeof(char*) * 70);
    memset(instructionLines,'\0',70);
    char* frontobjectLine; frontobjectLine = malloc(sizeof(char*) * 70);
    char * stringConvert; stringConvert = malloc(sizeof(char*) * 10);
    memset(frontobjectLine,'\0',70);
    char * check; check = malloc(sizeof(char*)*32); memset(check,'\0',32);
    char* Asc; Asc = malloc(sizeof(char*)*4); memset(Asc,'\0',4);
     unsigned deci;unsigned startAdd = 0;


    

     int first = 1;
    int lineNumber;
    for(lineNumber = 1; !feof(Ifr); lineNumber++){
        
        errorCount = 0;
        
        memset(line,'\0',127);
        fgets(line, 127, Ifr);
        trimNewLine(line);

        //printf("%s\n",line);


        if(line[0] != '.'){
            if(isspace(line[0])){
                label[0] = ' '; opcode = strtok(line," ");
                trimNewLine(opcode);
                
                if(!strcmp(opcode,"RSUB")){                                             //but check if opcoe is rsub, then it doesnt need a operand
                    strcpy(operand,"");                                                 //so set operand to a blank and it's neonic val to 0
                    mnemonicVal = 00;                                                    //mnemonic val 0.
                }else{
                    operand = strtok(NULL," ");trimNewLine(operand);                  //otherwise tokenize operand and trim it.
                }
            }else{                                                              //if all 3 exist then tokenize all 3 and trim
                label = strtok(line," ");opcode = strtok(NULL," ");operand = strtok(NULL," ");
                trimNewLine(label);trimNewLine(opcode);trimNewLine(operand);
            }

            

            memset(readMnemonicVal,'\0',10); memset(readLineAddress,'\0',10);
            memset(readOperand,'\0',12);     memset(readErrors,'\0',10); memset(readLabel,'\0',10);
            memset(readOpcode,'\0',10);

            fgets(readLabel,9,Ifr); fgets(readOpcode,9,Ifr);
            fgets(readMnemonicVal,9,Ifr); fgets(readLineAddress,9,Ifr);
            fgets(readOperand,11,Ifr); fgets(readErrors,9,Ifr);


            trimNewLine(readMnemonicVal);trimNewLine(readLineAddress); trimNewLine(readOperand);
            trimNewLine(readErrors);trimNewLine(readLabel);trimNewLine(readOpcode);

            //parse errors
            if(!isdigit(readErrors[0])){
            	errorCount = 0;
            }else{
            	int c; memset(errors,0,sizeof(errors)); 
	            char *tok, *saved;
				for (tok = strtok_r(readErrors, "%", &saved), c = 0; tok; tok = strtok_r(NULL, "%", &saved),c++)
				{
				    errors[c] = (int)strtoul(tok,NULL,10);
				}
				//printIntArray(errors,c);
				errorCount = c;
            }
            

             //if it is the first line and opcode is start
            if( !strcmp(opcode,"START")){

                writeListingLine(Lfw,readLineAddress,readLabel,readOpcode,readOperand," ", 0);
                //write object header
                fprintf(Ofw,"H");fprintf(Ofw,"%s",readLabel);fprintf(Ofw,"  ");
                val = (int)strtoul(readLineAddress,NULL,16);
                fprintf(Ofw,"%06x",val);
                startAdd = val;
                //get difference
                fprintf(Ofw,"%06x\n",(PROGRAM_LENGTH - val));



            }else{
                if(instruction == 1 ){
                        strcat(frontobjectLine,"T");
                        sprintf(stringConvert,"00%s",readLineAddress);
                        strcat(frontobjectLine,stringConvert);
                        instruction = 1;
                }   

                   
                //for every other line
                if(isMnemonic(readOpcode)){
                    if(readOperand[0] != ' '){
                        //Search symbol Table
                            int i;int exist = 0;
                            for(i=0; i < sCount; i++){
                                if(!strcmp(SYMTAB[i]->label,readOperand)){
                                    exist = 1; break;
                                }
                            }
                        //End search in symbol Table
                            if(exist == 1){
                                operandAddress = SYMTAB[i]->LOCCTR;
                            }else{
                                operandAddress = 0;
                                //\\//\\error flag here//\\//\\
                            }
                    }}else{
                        operandAddress = 0;
                    }
                    //assemble object line
                        val = symbolLocation(SYMTAB,readOperand,sCount);
                        deci = (int)strtoul(readMnemonicVal,NULL,16);
                        sprintf(stringConvert,"%02x",deci);
                        strcat(instructionLines,stringConvert);  
                        sprintf(stringConvert,"%04x",val);
                        strcat(instructionLines,stringConvert);
                        instruction++;
                        lineSize += 0x06;  

                }else if(!strcmp(readOpcode,"BYTE")){

                    memset(check,'\0',32);
                    if(readOperand[0] == 'C' && readOperand[1] == '\''){
                        int k; 
                        for(k = 2;readOperand[k] != '\'';k++){
                            sprintf(Asc,"%02x",readOperand[k]);
                            strcat(check,Asc);
                        }
                        lineSize += 0x06; 
                    }else if(readOperand[0] == 'X' && readOperand[1] == '\''){
                        int l;int i = 0;
                        char * Asc[2];Asc[1] = '\0';
                        for(l=2;readOperand[l] != '\'';l++){
                            i++;
                            Asc[0] = readOperand[l];
                            strcat(check,Asc);
                        }
                        lineSize += i; 
                    }else{
                        //error here
                    }

                    strcat(instructionLines,check);
                    instruction++;
                     

                }else if(!strcmp(readOpcode,"WORD")){
                    deci = (int)strtoul(readOperand,NULL,16);
                    sprintf(stringConvert,"%06x",deci);
                    strcat(instructionLines,stringConvert);
                    instruction++;
                    lineSize += 0x06;  
                }else if(!strcmp(readOpcode,"RSUB")){
                        sprintf(stringConvert,"%06x",val);
                        strcat(instructionLines,"4c");
                        strcat(instructionLines,"0000");
                        instruction++;
                    lineSize += 0x06;  
                }else if(!strcmp(readOperand,"BUFFER,X")){
                        val = symbolLocation(SYMTAB,readOperand,sCount);
                        sprintf(stringConvert,"%x",val);
                        stringConvert[0] = stringConvert[0] + 8;
                        strcat(instructionLines,readMnemonicVal);
                        strcat(instructionLines,stringConvert);
                        instruction++;
                    lineSize += 0x06;  
                }else if(!strcmp(readOpcode,"RESW") || !strcmp(readOpcode,"RESB")){
                        strcat(instructionLines,"      ");
                        instruction++;
                     
                }


                if(instruction == 11){
                    sprintf(stringConvert,"%x",(lineSize/2));
                    strcat(frontobjectLine,stringConvert);
                    strcat(frontobjectLine,instructionLines);
                    printf("%s\n",frontobjectLine);
                    fprintf(Ofw, "%s\n",frontobjectLine);
                    lineSize = 0;
                    instruction = 1;
                    memset(frontobjectLine,'\0',70);
                    memset(instructionLines,'\0',70);
                }  

                if(!strcmp(opcode,"END")){
                    sprintf(stringConvert,"%02x",(lineSize/2));
                    strcat(frontobjectLine,stringConvert);
                    strcat(frontobjectLine,instructionLines);
                    fprintf(Ofw, "%s\n",frontobjectLine);
                    
                    fprintf(Lfw,"%*s",16," ");
			        fprintf(Lfw,"%s\t",opcode);
			        fprintf(Lfw,"%s\t\n",operand);

                    fprintf(Ofw,"E");
                    fprintf(Ofw,"%06x\n",startAdd);

                    break;
                }

                                    //if object code will not fit into current text record
                
                    
               
                    //write listing line
                
                writeListingLine(Lfw,readLineAddress,readLabel,readOpcode,readOperand,readMnemonicVal,val);
            }            

        }else
        	fprintf(Lfw,"%s\n",line);



    }

    fclose(Ifr); fclose(Ofw); fclose(Lfw);

}
