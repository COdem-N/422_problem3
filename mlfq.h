/*
10/17/2017
Authors: Carter Odem , Amanda Aldrich

In this project our goal is to design, implement, and test a scheduling
simulation of the full MLFQ algorithm
*/

#ifndef mlfq_h
#define mlfq_h

#include "scheduler.h"

//includes
#include "fifo_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct mlfq {
    unsigned int quantums; // amount of time taken by the cpu
    ReadyQueue queue; // the fifo queue of pcb's
    unsigned int priority; // 0 is highest – 15 is lowest.
} mlfq_s;

typedef mlfq_s * MLFQ_p;





#endif
