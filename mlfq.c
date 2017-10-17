/*
10/17/2017
Authors: Carter Odem , Amanda Aldrich

In this project our goal is to design, implement, and test a scheduling
simulation of the full MLFQ algorithm. this will be  the full MLFQ
scheduling mechanism and simulate the processing of
many processes
*/
#include "mlfq.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned int sysstack;
int switchCalls;




void main() {
  setvbuf(stdout, NULL, _IONBF, 0);
  time_t t;
  srand((unsigned) time(&t));
  sysstack = 0;
  switchCalls = 0;
  timer();
}
