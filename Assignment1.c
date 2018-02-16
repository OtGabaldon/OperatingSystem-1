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

				// Select the current process and set its wait time to the current time minus
				// the time it arrived
				printf("Time %d: %s selected (burst %d)\n", timer, procs[ordered[i]].name, burst);
				procs[ordered[i]].wait = timer - procs[ordered[i]].arrival;

				// Set the process's turnaround time to the time it waited plus the time it ran
				procs[ordered[i]].turnaround = procs[ordered[i]].wait + procs[ordered[i]].burst;

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
					printf("Time %d: IDLE\n", timer++);
			} else {
				// Print IDLE and set timer to the next arrival time if no process has
				// arrived yet
				printf("Time %d: IDLE\n", timer);
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

	// Print each process wait and turn around time
	printf("\n");
	for(i=0; i<numProcs; i++) {
		printf("%s wait %d turnaround %d\n",procs[i].name,procs[i].wait,procs[i].turnaround);
	}
}
// Shortest Job First (preemptive)
void sjf() {
	
	int i,j,k,arriveCounter=0,waitCounter=0,finishedCounter=0;
	
	Process temp, running;
	running.burst=-1;
		
	Process *order = malloc(sizeof(Process)*numProcs);
	Process *waiting= malloc(sizeof(Process)*numProcs);
	Process *finished= malloc(sizeof(Process)*numProcs);
	
	//copying procs to an array so we can keep original order later.
	for(i=0;i<numProcs;i++){
		
		order[i]=procs[i];
		
	}
	
	//ordering the procs by arrival( Bubble Sort :( ).
	for(i=0;i<numProcs-1;i++){
		for(j=0;j<numProcs-1;j++){
		
			if(order[j].arrival>order[j+1].arrival){
				temp=order[j];
				order[j]=order[j+1];
				order[j+1]=temp;
			}
			else if(order[j].arrival==order[j+1].arrival && order[j].burst > order[j+1].burst){
				temp=order[j];
				order[j]=order[j+1];
				order[j+1]=temp;
			}
		}
	}
	
	//simulates running through the time.
	for(i=0;i<runTime;i++){
		
		//add to waiting list.
		while(order[arriveCounter].arrival==i){
			
			waiting[waitCounter]=order[arriveCounter];
			waiting[waitCounter].wait=0;
			printf("Time %d: %s arrived\n",i,order[arriveCounter].name);
			//process is added to wait queue, thus we need to increment counter.
			arriveCounter++;
			
			//process has arrived to cpu so we need to increment posiiton.
			waitCounter++;
			
			//if there is more than one proc then order them based on burst length.
			if(waitCounter>1){
				for(j=0;j<waitCounter-1;j++){
					for(k=0;k<waitCounter-1;k++){
						if(waiting[k].burst>waiting[k+1].burst){
							temp=waiting[k];
							waiting[k]=waiting[k+1];
							waiting[k+1]=temp;
						}
					}
				}
			}
		}
		
		//Decrement counters and see if programs are finished.
		if(running.burst!=-1){
			running.burst--;
			
			if(running.burst==0){
				
				printf("Time %d: %s finished\n",i,running.name);
				running.turnaround=i-running.arrival;
				finished[finishedCounter]=running;
				finishedCounter++;
				//signal that the program has been removed and running is empty.
				running.burst=-1;
			}
			
		}
		
		
	//	for(j=0;j<waitCounter;j++){
	//		printf("Proc %s burst %d\n",waiting[j].name,waiting[j].burst);
	//	}
		
		
		
		
		
		
		//choose which program to run
		
		// if there is no program running.
		if(running.burst==-1){
			
			// insert shortest burst program into running position.
			if(waitCounter!=0){
				running=waiting[0];
				printf("Time %d: %s selected (burst %d)\n",i,running.name,running.burst);
				// if there is more than one program waiting move the rest down to proper positions.
				if(waitCounter>1){
					for(j=0;j<waitCounter-1;j++){
						waiting[j]=waiting[j+1];
					}
					waiting[j].burst=-1;
				}
				//Decrement waiting counter since one proc was sent to running.
				waitCounter--;
			}
			else{
				printf("Time %d: IDLE\n",i);
			}
		
			
		}
		else if(running.burst>waiting[0].burst){// if program running has longer burst than shortest in watiing.
			
			// insert shortest burst program into running position and then the running program back into waiting list.
			temp=running;
			running=waiting[0];
			waiting[0]=temp;
			
			printf("Time %d: %s selected (burst %d)\n",i,running.name,running.burst);
			
			// resort the waiting list to have the shortest program in front.
			for(j=0;j<waitCounter-1;j++){
				for(k=0;k<waitCounter-1;k++){
					if(waiting[k].burst>waiting[k+1].burst){
						temp=waiting[k];
						waiting[k]=waiting[k+1];
						waiting[k+1]=temp;
					}
				}
			}
			
			
		}
		
		//Increment wait time for those processes in wait array.
		for(j=0;j<waitCounter;j++){
			waiting[j].wait++;
		}
		
	}
	printf("Finished at time %d\n\n",runTime);\
	
	// put all the turnaround times and wait times in the correct order in the procs array.
	for(i=0;i<finishedCounter;i++){
		for(j=0;j<numProcs;j++){
			if(strcmp(finished[i].name,procs[j].name)==0){
				procs[j].wait=finished[i].wait;
				procs[j].turnaround=finished[i].turnaround;	
			}
			
		}
		
	}
	//print out results.
	for(i=0;i<numProcs;i++){
		printf("%s wait %d turnaround %d\n",procs[i].name,procs[i].wait,procs[i].turnaround);;	
	}
	free(order);
	free(waiting);
	free(finished);
	
	/*for(i=0;i<=runTime;i++){
		
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
	}*/
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
				
				if(arrival == timer) {
					printf("Time %d: %s arrived\n", timer, procs[i].name);
					procs[i].wait = burst;	
				}
				
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
					procs[i].wait = procs[i].turnaround - procs[i].wait;
				}
				
				if(finished == numProcs || timer >= runTime)
					break;
			}
		}
		
		if(!start) {
			printf("Time %d: IDLE\n", timer);
			timer++;
		}
	
		// Break the loop if all of the processes have finished or if time has
		// ran out.
		if(finished == numProcs) {		
			if(timer < runTime) {
				for(i = timer; i < runTime; i++) {
					printf("Time %d: IDLE\n", i);
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
		printf("Using First Come First Served\n");
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

void readInput()
{
	FILE* ifp;
	char string[100];
	int i;
	
	ifp = fopen("processes.txt", "r");
	
	if(ifp == NULL) {
		printf("ERROR: The input file could not be opened.\n");
		exit(1);
	}
	
	while(fscanf(ifp, "%s", string) != EOF)
	{
		if(strcmp(string, "processcount") == 0)
		{
			fscanf(ifp, "%d", &numProcs);
			fscanf(ifp, "%*[^\n]%*c");
		}
		else if(strcmp(string, "runfor") == 0)
		{
			fscanf(ifp, "%d", &runTime);
			fscanf(ifp, "%*[^\n]%*c");
		}
		else if(strcmp(string, "use") == 0)
		{
			fscanf(ifp, "%s", algorithm);
			fscanf(ifp, "%*[^\n]%*c");
		}
		else if(strcmp(string, "quantum") == 0)
		{
			fscanf(ifp, "%d", &quantum);
			fscanf(ifp, "%*[^\n]%*c");
		}
		else if(strcmp(string, "process") == 0)
		{
			i = 0;
			
			procs = malloc(sizeof(Process) * numProcs);
			
			fscanf(ifp, "%*s %s %*s %d %*s %d%*[^\n]%*c", procs[i].name, &procs[i].arrival, &procs[i].burst);
			
			for(i = 1; i < numProcs; i++)
			{
				fscanf(ifp, "%*s %*s %s %*s %d %*s %d%*[^\n]%*c", procs[i].name, &procs[i].arrival, &procs[i].burst);
				fscanf(ifp, "%*[^\n]%*c");
			}
		}
		else if(strcmp(string, "end") == 0)
		{
			return;
		}
		else
		{
			fscanf(ifp, "%*[^\n]%*c");
		}
	}
}

/*
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
*/

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
