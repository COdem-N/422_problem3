/*
	10/12/2017
	Authors: Connor Lundberg, Carter Odem

	In this project we will be making a simple Round Robin scheduling algorithm
	that will take a single ReadyQueue of PCBs and run them through our scheduler.
	It will simulate the "running" of the process by randomly changing the PC value
	of the process as well as incorporating various interrupts to show the effects
	it has on the scheduling simulator.

	This file holds the defined functions declared in the scheduler.h header file.



*/
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global variables
unsigned int sysstack;
int switchCalls;
int quantoms[16];
int privileged_size;
PCB privileged[4];

//gcc pcb.c fifo_queue.c priority_queue.c scheduler.c

/*
	This function is our main loop. It creates a Scheduler object and follows the
	steps a normal Round Robin Scheduler would to "run" for a certain length of time,
	push new PC onto stack when timer interrupt occurs, then call the ISR, scheduler,
	dispatcher, and eventually an IRET to return to the top of the loop and start
	with the new process.
*/
void timer () {
	unsigned int pc = 0;
	unsigned int processesCreated = 0;
	unsigned int totalpcb = 0;
	unsigned int current_quant = 0;
	Scheduler thisScheduler = schedulerConstructor();

	for (;;) {

		if(current_quant > 1000){
			break;
		}

		// creating PCBs
		processesCreated = makePCBList(thisScheduler);
		totalpcb += processesCreated;
		switchCalls = processesCreated;
		printf("Created %d pcbs\n",processesCreated);

		if(totalpcb > 0)
		{
			// increment the pc
			pc = runProcess(thisScheduler,pc);
			sysstack = pc;

			//possibly turminate
			terminate(thisScheduler->running, thisScheduler);

			// call timer interupt
			//printf("Timmer Interupt\n");
			pseudoISR(thisScheduler);
			pc = thisScheduler->running->context->pc;

			// printing out state of the scheduling
			printf("Iteration: %d\n",current_quant);

			char *readyqueue = toStringPriorityQueue(thisScheduler->ready);

			printf("created readyqueue string\n");
			printf("%s", readyqueue);

			free(readyqueue);

			print_privileged();


		}





		printf("\n");
		current_quant++;
	}

	schedulerDeconstructor(thisScheduler);
}
/*
Tthis function generates a random number between 0 and 100, if the number is 15
or less the function will mark the current pcb for termination.
*/
int terminate(PCB thepcb, Scheduler theScheduler)
{

	int randval = rand() % 100;
	if(randval <= 15 && thepcb->privlage == 0)
	{
		printf("Marked for Termination\n");
		theScheduler->running->priority += DEATH_MARK; //marked for death!!!!
	}

}


void print_privileged()
{
	int i;
	printf("Privileged few:\n");
	if(privileged_size > 0)
	{
		for(i=0;i<privileged_size;i++)
		{
			char *PCBState = toStringPCB(privileged[i], 0);
			printf("%s\n", PCBState);
			free(PCBState);
		}
	}

}

/*
	This creates the list of new PCBs for the current loop through. It simulates
	the creation of each PCB, the changing of state to new, enqueueing into the
	list of created PCBs, and moving each of those PCBs into the ready queue.
*/
int makePCBList (Scheduler theScheduler) {
	//	printf(" making pcbs\n");
	int newPCBCount = rand() % MAX_PCB_IN_ROUND;

	int randval;

	for (int i = 0; i < newPCBCount; i++) {

		randval = rand() % 100;
		PCB newPCB = NULL;
		while(newPCB == NULL){
			newPCB = PCB_create();
		}
		newPCB->state = STATE_NEW;
		newPCB->priority = 0;
		newPCB->privlage = 0;
		if(randval <= 15 && privileged_size < 4)
		{
			printf("you are special \n");
			newPCB->privlage = 1;

			privileged[privileged_size] = newPCB;

			privileged_size++;

		}

		q_enqueue(theScheduler->created, newPCB);
	}

	if (newPCBCount) {
		if (theScheduler->isNew) {
			theScheduler->running = q_dequeue(theScheduler->created);
			theScheduler->running->state = STATE_RUNNING;
			theScheduler->isNew = 0;
		}

	}

	return newPCBCount;
}


/*
	Creates a random number between 0 and the prioritys quantom and adds it to the current PC.
	It then returns that new PC value.
*/
unsigned int runProcess (Scheduler theScheduler, unsigned int pc) {

	if(theScheduler->running->priority <= 15)
	{
		unsigned int jump = rand() % quantoms[theScheduler->running->priority];
		pc += jump;
	}
	else
	{
		unsigned int jump = rand() % quantoms[(theScheduler->running->priority - DEATH_MARK)];
		pc += jump;
	}

	return pc;
}


/*
	This acts as an Interrupt Service Routine, but only for the Timer interrupt.
	It handles changing the running PCB state to Interrupted, moving the running
	PCB to interrupted, saving the PC to the SysStack and calling the scheduler.
*/
void pseudoISR (Scheduler theScheduler) {

	theScheduler->running->state = STATE_INT;
	theScheduler->running->context->pc = sysstack;

	scheduling(IS_TIMER, theScheduler);
	pseudoIRET(theScheduler);
}


/*
	If the interrupt that occurs was a Timer interrupt, it will simply set the
	interrupted PCBs state to Ready and enqueue it into the Ready queue. It then
	calls the dispatcher to get the next PCB in the queue.
*/
void scheduling (int isTimer, Scheduler theScheduler) {
	//printf("scheduling\n");
	int term = 0;

	// moves the pcbs from the created queue to the ready queue
	if(switchCalls > 0)
	{
		while(!q_is_empty(theScheduler->created)) {
			PCB readyPCB = q_dequeue(theScheduler->created);
			readyPCB->state = STATE_READY;
			pq_enqueue(theScheduler->ready,readyPCB);
		}
	}


	// checks if the termincating queue is full
	if(theScheduler->zombies->size >= 10)
	{
		printf("destroying zombies queue\n");
		q_destroy(theScheduler->zombies);
		theScheduler->zombies = q_create();
	}

	printf("priority check\n");
	if(theScheduler->running->priority == 15)
	{
		theScheduler->running->priority = 0;
	}
	else if (theScheduler->running->priority >= 20)
	{
		printf("moving PCB to zombie queue\n");
		theScheduler->running->state = STATE_HALT;
		q_enqueue(theScheduler->zombies, theScheduler->running);
		term = 1;
	}
	else
	{
		printf("inc priority\n");
		theScheduler->running->priority++;
	}


	// checks for timer interupt
	if (isTimer)
	{	//printf("timmer int\n");
		if (term == 0)
		{
			theScheduler->running->state = STATE_READY;
			pq_enqueue(theScheduler->ready, theScheduler->running);
		}
		else {
			term = 0;
		}

		dispatcher(theScheduler);
	}






}


/*
	This simply gets the next ready PCB from the Ready queue and moves it into the
	running state of the Scheduler.
*/
void dispatcher (Scheduler theScheduler) {

	if(!pq_is_empty(theScheduler->ready))
	{
		printf("dispatched\n");
		theScheduler->running = pq_dequeue(theScheduler->ready);
		theScheduler->running->state = STATE_RUNNING;
	}

}


/*
	This simply sets the running PCB's PC to the value in the SysStack;
*/
void pseudoIRET (Scheduler theScheduler) {
	theScheduler->running->context->pc = sysstack;
}


/*
	This will construct the Scheduler, along with its numerous ReadyQueues and
	important PCBs.
*/
Scheduler schedulerConstructor () {
	int i;
	Scheduler newScheduler = (Scheduler) malloc (sizeof(scheduler_s));
	newScheduler->ready = pq_create();
	newScheduler->created = q_create();
	newScheduler->zombies = q_create();
	newScheduler->blocked = q_create();
	newScheduler->running = PCB_create();
	newScheduler->isNew = 1;
	newScheduler->running = NULL;
	return newScheduler;
}


/*
	This will do the opposite of the constructor with the exception of
	the interrupted PCB which checks for equivalancy of it and the running
	PCB to see if they are pointing to the same freed process (so the program
	doesn't crash).
*/
void schedulerDeconstructor (Scheduler theScheduler) {

	pq_destroy(theScheduler->ready);


	q_destroy(theScheduler->created);
	q_destroy(theScheduler->zombies);
	q_destroy(theScheduler->blocked);
	PCB_destroy(theScheduler->running);

	free (theScheduler);
}


void main () {
	time_t t;

	srand((unsigned) time(&t));

	printf("\n");

	quantoms[0] = p0_quantoms;
	quantoms[1] = p1_quantoms;
	quantoms[2] = p2_quantoms;
	quantoms[3] = p3_quantoms;
	quantoms[4] = p4_quantoms;
	quantoms[5] = p5_quantoms;
	quantoms[6] = p6_quantoms;
	quantoms[7] = p7_quantoms;
	quantoms[8] = p8_quantoms;
	quantoms[9] = p9_quantoms;
	quantoms[10] = p10_quantoms;
	quantoms[11] = p11_quantoms;
	quantoms[12] = p12_quantoms;
	quantoms[13] = p13_quantoms;
	quantoms[14] = p14_quantoms;
	quantoms[15] = p15_quantoms;

	privileged_size = 0;
	sysstack = 0;
	switchCalls = 0;

	timer();

	printf("-----------------------End Program-----------------------");
}
