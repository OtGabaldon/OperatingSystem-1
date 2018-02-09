/* Operating System 
   Assignment 1
   2/9/2018
   COP4600
   Ot Gabaldon
   Kiara Crawford
   Nick Deleuze 
*/

// Reads input from file inside the directory
void readInput();


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main () {
	
	readInput();
	
	return 0;
}


void readInput(){
	
	int run=1,i,skip=0;
	char word[20]; // used to check what action is being preformed
	char letter[2]; // used to construct words
	
	//open file
	FILE *fp;
	fp= fopen("processes.txt","r");
	if(fp==NULL){
		printf("ERROR: Can't Open File.");
		return;
	}
		
	
	//main loop to run
	while (run==1){
			word[0]='\0';
		
		//run until letter is a space
		while((letter[0]=fgetc(fp))!=' '||skip==1){
			
			//skip everything after the # comment.
			if(letter[0]=='#'){
				skip=1;
			}
			else if(skip==0){ 
				
				//if end of file is reached kick out.
				if(letter[0]==EOF){
					return;
				}
				
				//add char to the string
				strcat(word,letter);
			}
			//Once end of line is reached stop skipping.
			if(letter[0]=='\n'){
				skip=0;
			}
		
			
		}
		
		//printf("|%s| \n",word);
		if(strcmp(word,"processcount")==0){
			printf("process\n");
		}
		else if(strcmp(word,"runfor")==0){
			printf("runfor\n");
		}
		else if(strcmp(word,"use")==0){
			printf("use\n");
		}
		else if(strcmp(word,"quantum")==0){
			printf("quantum\n");
		}
		else if(strcmp(word,"name")==0){
			printf("name\n");
		}
		else if(strcmp(word,"arrival")==0){
			printf("arrival\n");
		}
		else if(strcmp(word,"burst")==0){
			printf("burst\n");
		}
		else if(strcmp(word,"end")==0){ //program input ends
				printf("end");
				run=0;
				return;
		}
	
		
		
	}
	
}
