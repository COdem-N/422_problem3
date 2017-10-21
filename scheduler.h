/*
	10/12/2017
	Authors: Connor Lundberg, Carter Odem

	In this project we will be making a simple Round Robin scheduling algorithm
	that will take a single ReadyQueue of PCBs and run them through our scheduler.
	It will simulate the "running" of the process by randomly changing the PC value
	of the process as well as incorporating various interrupts to show the effects
	it has on the scheduling simulator.

	This file holds the definitions of structs and declarations functions for the
	scheduler.c file.
*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

//includes
#include "pcb.h"
#include "fifo_queue.h"
#include "priority_queue.h"


//defines
#define MAX_PCB_TOTAL 30
#define MAX_PCB_IN_ROUND 5
#define MAX_PC_JUMP 4000
#define MIN_PC_JUMP 3000
#define PC_JUMP_LIMIT 999
#define IS_TIMER 1
#define SWITCH_CALLS 4
#define DEATH_MARK 20

#define p0_quantoms 16
#define p1_quantoms 15
#define p2_quantoms 14
#define p3_quantoms 13
#define p4_quantoms 12
#define p5_quantoms 11
#define p6_quantoms 10
#define p7_quantoms 9
#define p8_quantoms 8
#define p9_quantoms 7
#define p10_quantoms 6
#define p11_quantoms 5
#define p12_quantoms 4
#define p13_quantoms 3
#define p14_quantoms 2
#define p15_quantoms 1

//structs
/*typedef struct created_list_node {
	PCB pcb;
}

typedef struct created_list {

}*/

typedef struct scheduler {
	ReadyQueue created;
	ReadyQueue zombies;
	ReadyQueue blocked;
	PriorityQueue ready;
	PCB running;
	int isNew;
} scheduler_s;

typedef scheduler_s * Scheduler;


//declarations
void timer ();

int makePCBList (Scheduler);

unsigned int runProcess (Scheduler theScheduler, unsigned int);

void pseudoISR (Scheduler);

void scheduling (int, Scheduler);

void dispatcher (Scheduler);

void pseudoIRET (Scheduler);

Scheduler schedulerConstructor ();

void schedulerDeconstructor (Scheduler);

void print_privileged();

int terminate(PCB thepcb, Scheduler theScheduler);
#endif
