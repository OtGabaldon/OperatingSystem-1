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
	int wait;
	int turnaround;
} Process;

char algorithm[100];
int numProcs, runTime, quantum;
Process* procs;

void fcfs();
void sjf();
void rr();
void printHeader();
void printResults();
void printProcessInfo();
void readInput();

int main () {

	readInput();
	printHeader();

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

	int ordered[numProcs];
	int timer = 0, finished = 0, lastArrival, iter, arrival, burst, earliest, i, j, k;

	// Create an array for ordering the processes by arrival
	lastArrival = procs[0].arrival;
	for(i = 1; i < numProcs; i++) {
		if(lastArrival < procs[i].arrival)
			lastArrival = procs[i].arrival;
	}

	// Order the array using a histogram sort
	int arrivalBucket[lastArrival + 1];
	for(i = 0; i < lastArrival + 1; i++) {
		arrivalBucket[i] = -1;
	}
	for(i = 0; i < numProcs; i++) {
		arrivalBucket[procs[i].arrival] = i;
	}
	iter = 0;
	for (i = 0; i < lastArrival + 1; i++) {
		if(arrivalBucket[i] > -1) {
			ordered[iter] = arrivalBucket[i];
			iter++;
		}
	}

	// Runs First-Come First-Serve Algorithm until each ordered process has finished or
	// time has run out
	while(1) {
		for(i = 0; i < numProcs; i++) {
			arrival = procs[ordered[i]].arrival;
			burst = procs[ordered[i]].burst;

			// Select a queued process
			if(arrival <= timer) {
				if(arrival == timer) {
					printf("Time %d: %s arrived\n", timer, procs[ordered[i]].name);
				}

				printf("Time %d: %s selected (burst %d)\n", timer, procs[ordered[i]].name, burst);

				// Check if the next processes to run have arrived
				for(j = timer + 1; j <= timer + procs[ordered[i]].burst; j++) {
					for(k = i; k < numProcs; k++) {
						if(procs[ordered[k]].arrival == j) {
							printf("Time %d: %s arrived\n", j, procs[ordered[k]].name);
						}
					}
				}

				// Add the current processes burst time to the time
				// and break if the process has gone past the allotted run time
				timer += procs[ordered[i]].burst;
				if(timer > runTime) 
					break;

				procs[ordered[i]].burst = 0;
				printf("Time %d: %s finished\n", timer, procs[ordered[i]].name);
				finished++;

				// Break if the timer has reached the allotted run time
				if(timer == runTime) 
					break;

				if(finished == numProcs)
					printf("Time %d: Idle\n", timer++);
			} else {
				// Print Idle and set timer to the next arrival time if no process has
				// arrived yet
				printf("Time %d: Idle\n", timer);
				timer = arrival;
				i--;
			}
		}
		// Print Finished at last process completion are at run time if a process exceeded
		// its allotted time
		if(finished == numProcs) {
			printf("Finished at time %d\n", timer);
			break;
		} else if (timer >= runTime) {
			printf("Finished at time %d\n", runTime);
			break;
		}
	}
}

// Shortest Job First (preemptive)
void sjf() {
	
	int i,j,k,waitCounter=0,procCounter=0,check=0,turnCounter;
	
	Process *waiting[numProcs], temp, *running=NULL, *tempPoint,turnaround[numProcs];
	
	printHeader();
	
	//ordering the procs by arrival( Bubble Sort :( ).
	for(i=0;i<numProcs;i++){
		for(j=0;j<numProcs;j++){
		
			if(procs[j].arrival>procs[j+1].arrival){
				temp=procs[j];
				procs[j]=procs[j+1];
				procs[j+1]=temp;
			}
			else if(procs[j].arrival==procs[j+1].arrival && procs[j].burst > procs[j+1].burst){
				temp=procs[j];
				procs[j]=procs[j+1];
				procs[j+1]=temp;
			}
		}
	}
	
	for(i=0;i<=runTime;i++){
		
		//insert into waiting queue on arrival.
		while(procs[procCounter].arrival==i){
			
			// add proc into waiting queue
			waiting[waitCounter]=&procs[procCounter];
			waiting[waitCounter]->wait=0;
			waitCounter++;
			
			//print arrival 
			printf("Time %d: %s arrived\n", i, procs[procCounter].name);
			procCounter++;
			
			//sort queue if more than one waiting proccess.
			if(waitCounter>1){
				for(j=0;j<waitCounter;j++){
					for(k=0;k<waitCounter-1;k++){
						if(waiting[k]->burst>waiting[k+1]->burst){
							tempPoint=waiting[k];
							waiting[k]=waiting[k+1];
							waiting[k+1]=tempPoint;
						}
			
					}
				}
			}
			
			// Shows that there was a proc added to wait queue.
			check=1;
		}
		
			//decrement runtime.
		if(running!=NULL){
			running->burst--;
		}
		
			
		// When there is a proc added to wait make sure that it does not have a shorter job
		if(check==1){
			
			if(running==NULL){ //if empty
				running=waiting[0];
			}
			else if(running->burst>waiting[0]->burst){ // if burst of waiting is less than running.
				waiting[waitCounter]=running;
				waitCounter++;
				running=waiting[0];
				
				// Sort the waiting list after updating running proc.
				for(j=0;j<waitCounter;j++){
					for(k=0;k<waitCounter-1;k++){
						if(waiting[k]->burst>waiting[k+1]->burst){
							tempPoint=waiting[k];
							waiting[k]=waiting[k+1];
							waiting[k+1]=tempPoint;
						}
			
					}
				}
				
			}
			
			//deletes from waiting list.
			for(j=0;j<waitCounter-1;j++){
				waiting[j]=waiting[j+1];
			}
			waitCounter--;
		
			printf("Time %d: %s selected (burst %d)\n",i,running->name,running->burst);
			// toggle the check off once proper positioning is checked.
			check=0;	
		}
		
		//When the proc finishes
		if(running!=NULL && running->burst==0){
			printf("Time %d: %s finished\n",i,running->name);
			
			//assigns turnaround time to an array with name.
			strcpy(turnaround[turnCounter].name,running->name);
			turnaround[turnCounter].turnaround=i-running->arrival;
			turnaround[turnCounter].wait=running->wait;
			turnCounter++;
			//Set new proc to be running.
			if(waiting[0]->burst>0){
				
				running=waiting[0];
			
				//delete proc from list.
				for(j=0;j<waitCounter-1;j++){
					waiting[j]=waiting[j+1];
				}
				
				waitCounter--;
			
				//print out new selected proc
				printf("Time %d: %s slected (burst %d)\n",i,running->name,running->burst);
			}
			else{
				
				running=NULL;
				
			}
		}
		
		//Print out runtime.	
 		if(running==NULL && i!=runTime){
			
			printf("Time %d: IDLE\n",i);
		}
		
		// Increment wait for all those in waiting list.
		for(j=0;j<waitCounter && waitCounter>0;j++){
			waiting[j]->wait++;
		}	
		
	}
	printf("Finished at time %d\n\n",runTime);
	
	for(i=0;i<numProcs;i++){
		printf("%s wait %d turnaround %d\n",turnaround[i].name,turnaround[i].wait,turnaround[i].turnaround);
	}
}

// Round-Robin ****I need to test this and make it more efficient****
void rr() {
	
	int timer = 0, finished = 0, len = 0, running = 1, start = 0;
	int arrival, burst, i, j, k;
	
	while(running) {
		for(i = 0; i < numProcs; i++) {
			arrival = procs[i].arrival;
			burst = procs[i].burst;
		
			if(arrival <= timer && burst > 0) {
				start = 1;
				
				if(arrival == timer)
					procs[i].wait = burst;
				
				// Determine how long the process is going to run (the quantum
				// or its burst, whichever is smaller).
				if(burst < quantum)
					len = burst;
				else
					len = quantum;

				printf("Time %d: %s selected (burst %d)\n", timer, procs[i].name, burst);
			
				// Update the processes' new remaining burst time.
				procs[i].burst -= len;
				
				// Check if other processes arrived during the burst.
				for(j = timer + 1; j <= timer + len; j++) {
					for(k = 0; k < numProcs; k++) {
						if(procs[k].arrival == j)
							printf("Time %d: %s arrived\n", j, procs[k].name);
					}
				}
				
				timer += len;
				
				// Check if the current process has finished.
				if(procs[i].burst == 0) {
					printf("Time %d: %s finished\n", timer, procs[i].name);
					finished++;
					
					procs[i].turnaround = timer - procs[i].arrival;
					
					printf("I am %s and my turnaround was %d and wait %d\n", procs[i].name, procs[i].turnaround, procs[i].wait);

					
					procs[i].wait = procs[i].turnaround - procs[i].wait;
				}
				
				if(finished == numProcs || timer >= runTime)
					break;
			}
		}
		
		if(!start) {
			printf("Time %d: Idle\n", timer);
			timer++;
		}
	
		// Break the loop if all of the processes have finished or if time has
		// ran out.
		if(finished == numProcs) {		
			if(timer < runTime) {
				for(i = timer; i < runTime; i++) {
					printf("Time %d: Idle\n", i);
					timer++;
				}
			}

			printf("Finished at time %d\n", timer);
			break;
		} else if(timer >= runTime) {
			printf("Finished at time %d\n", timer);
			break;
		}
	}
	
	printResults();
}

// Prints out header for all algorithms
void printHeader() {

	printf("%d processes\n", numProcs);

	if(strcmp(algorithm, "fcfs") == 0) {
		printf("Using First-Come First-Serve\n");
	} else if(strcmp(algorithm, "sjf") == 0) {
		printf("Using Shortest Job First (Pre)\n");
	} else if(strcmp(algorithm, "rr") == 0) {
		printf("Using Round-Robin\n");
		printf("Quantum %d\n", quantum);
	}
		
	printf("\n");
}

void printResults() {
	
	int i;
	
	printf("\n");
	
	for(i = 0; i < numProcs; i++)
		printf("%s wait %d turnaround %d\n", procs[i].name, procs[i].wait, procs[i].turnaround);
}

void printProcessInfo() {
	int i;
	for(i = 0; i < numProcs; i++) {
		printf("Process: %s\n    arrival: %d\n    burst: %d\n", procs[i].name, procs[i].arrival, procs[i].burst);
	}
	printf("\n");
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
	else
		fscanf(ifp, "%*[^\n]%*c");
	
	procs = malloc(sizeof(Process) * numProcs);
	
	for(i = 0; i < numProcs; i++) {
		fscanf(ifp, "process %*s %s arrival %d burst %d\n", procs[i].name, &procs[i].arrival, &procs[i].burst);
		
		procs[i].wait = procs[i].burst;
	}
		
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
