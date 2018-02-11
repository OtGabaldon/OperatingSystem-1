/* 
   Programming Assignment 1
   2/9/2018
   COP4600 Operating Systems
   Ot Gabaldon
   Kiara Crawford
   Nick Deleuze 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Process {
	char name[100];
	int arrival;
	int burst;
} Process;

char algorithm[100];
int numProcs, runTime, quantum;
Process* procs;

void fcfs();
void sjf();
void rr();
void printHeader();
void readInput();

int main () {

	readInput();
	
	if(strcmp(algorithm, "fcfs") == 0)
		fcfs();
	else if(strcmp(algorithm, "sjf") == 0)
		sjf();
	else if(strcmp(algorithm, "rr") == 0)
		rr();
	
	return 0;
}

// First-Come First-Serve
void fcfs() {

}

// Shortest Job First (preemptive)
void sjf() {

}

// Round-Robin
void rr() {
	
}

// Prints out header for all algorithms
void printHeader() {

	printf("%d processes\n", numProcs);
	printf("Using %s\n", algorithm);
	
	if(strcmp(algorithm, "rr") == 0)
		printf("Quantum %d\n", quantum);
		
	printf("\n\n");
}

// Processes input from input file
void readInput() {
	FILE* ifp;
	int i;
	
	ifp = fopen("processes.txt", "r");
	
	if(ifp == NULL) {
		printf("ERROR: The input file could not be opened.\n");
		exit(1);
	}

	fscanf(ifp, "processcount %d %*[^\n]%*c", &numProcs);
	fscanf(ifp, "runfor %d %*[^\n]%*c", &runTime);	
	fscanf(ifp, "use %s %*[^\n]%*c", algorithm);
	
	if(strcmp(algorithm, "rr") == 0)
		fscanf(ifp, "quantum %d %*[^\n]%*c", &quantum);
	
	procs = malloc(sizeof(Process) * numProcs);
	
	for(i = 0; i < numProcs; i++)
		fscanf(ifp, "process %*s %s arrival %d burst %d\n", procs[i].name, &procs[i].arrival, &procs[i].burst);
	
	fclose(ifp);
}

// Leaving original readInput method for now just in case we need to change it back.

/*
void readInput(int *proccessCount, int *runTime, int *quantum, char runType[10], struct process list[100]){
	
	int run=1,i,k=0,skip=0,processCounter=0;
	char word[20]; // used to check what action is being preformed
	char letter[2]; // used to construct words
	char temp[10];
	
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
			
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			
			temp[k]='\0';
			*proccessCount = atoi(temp);// turning char into int.
			
			//Cleaning up temp.
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"runfor")==0){
			
			// gets whole number because it can be multiple chars
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			
			temp[k]='\0';
			*runTime = atoi(temp);
			
			//Cleaning up temp.
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"use")==0){
			
			//Copying over whole type string into temp variable.
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			//Copying over temp to runType.
			temp[k]='\0';
			strcpy(runType,temp);
			
			//Cleaning up temp.
			temp[0]='\0';
			k=0;
			
		}
		else if(strcmp(word,"quantum")==0){
			
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			
			temp[k]='\0';
			*quantum = atoi(temp);
			
		
			//Cleaning up temp.
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"name")==0){
			
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			//create new struct.
			struct process procTemp;
			
			//copy name to process
			strcpy(procTemp.name,temp);
			
			//assign temp to array and clean up temp.
			list[processCounter]=procTemp;
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"arrival")==0){
			
			while((temp[k]=fgetc(fp))!=' '){
				k++;
			}
			
			list[processCounter].arrival=atoi(temp);
			
			
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"burst")==0){
			
			
			while((temp[k]=fgetc(fp))!='\n'){
				k++;
			}
			
			list[processCounter].burst=atoi(temp);
			
			//Burst should be the last info entered for proccess.
			processCounter++;
			
			//Clean up temp.
			temp[0]='\0';
			k=0;
		}
		else if(strcmp(word,"end")==0){ //program input ends
				printf("end");
				run=0;
				return;
		}	
	}	
}
*/