/*
 * Project 1
 *
 * Authors: Keegan Wantz, Carter Odem
 * TCSS 422.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "pcb.h"
#include "fifo_queue.h"
#include "priority_queue.h"

PCB_p make_pcb();
void PCB_test();
void FIFO_test();
void PQ_test();

int main(void) {
    printf("Beginning Tests:\n");
    printf("PCB Test:\n");
    PCB_test();
    printf("FIFO Test:\n");
    FIFO_test();
    printf("PQ Test:\n");
    PQ_test();
    printf("ALL_TESTS_COMPLETE\n");
}



void PCB_test() {
    char * pcb_str = NULL;
    time_t t;

    srand(time(NULL));

    printf("Creating PCB:\n");
    PCB_p my_pcb = PCB_create();

    pcb_str = PCB_to_string(my_pcb);

    printf("\n\nInitial state:\n");
    if (pcb_str != NULL) {
        printf("%s\n",pcb_str);
        free(pcb_str);
    }

    printf("\nAssigning state:\n");
    PCB_assign_PID(my_pcb);
    PCB_assign_state(my_pcb, STATE_READY); //0 for new state
    PCB_assign_priority(my_pcb, rand() % 16);
    PCB_assign_parent(my_pcb, my_pcb->pid-1);

    printf("\n\nAssigning complete, printing again:\n");
    pcb_str = PCB_to_string(my_pcb);

    if (pcb_str != NULL) {
        printf("%s\n", pcb_str);
        free(pcb_str);
    }

    printf("\n\nDestroying PCB.\n");
    PCB_destroy(my_pcb);
    printf("Done.\n");

    printf("END_OF_PCB_TESTS\n");
}

void FIFO_test() {
    int rand_pcb,i;
    PCB_p c_pcb = NULL;
    char * q_str = NULL;

    srand(time(NULL));

    rand_pcb = (rand() % 21) + 10;

    printf("Creating and enqueueing:\n\n");
    FIFOq_p my_fifo = q_create();

    for (i = 0; i < rand_pcb; i++) {
        q_enqueue(my_fifo, make_pcb());
        q_str = q_to_string(my_fifo, 1);
        printf("%s\n", q_str);
        free(q_str);
    }

    printf("\n\n\nDequeueing:\n\n");
    while (!q_is_empty(my_fifo)) {
        q_str = q_to_string(my_fifo, 0);
        printf("%s\n", q_str);
        free(q_str);

        c_pcb = q_dequeue(my_fifo);

        q_str = q_to_string(my_fifo, 0);
        printf("%s\n", q_str);
        free(q_str);

        q_str = PCB_to_string(c_pcb);
        printf("%s\n", q_str);
        free(q_str);

        PCB_destroy(c_pcb);
    }

    printf("\n\n\nDestroying the queue.\n\n");
    q_destroy(my_fifo);
    printf("Complete.\n");
    printf("END_OF_FIFO_TESTS\n");
}

void PQ_test() {
    int i, j, to_remove, d;
    char * out_str;
    PCB_p pcb_ptr = NULL;

    srand(time(NULL));

    printf("Creating priority queue.\n");
    PQ_p my_pq = pq_create();

    printf("Beginning to add PCBs:\n\n");
    for (i = 0; i < 10; i++) {
        printf("Step %d:\n", i+1);
        for(j = 0; j < 10; j++) {
            pcb_ptr = make_pcb();
            if (pcb_ptr != NULL) {
                pq_enqueue(my_pq, pcb_ptr);
            }
        }

        out_str = pq_to_string(my_pq);

        if (out_str != NULL) {
            printf("%s\n", out_str);
            free(out_str);
        }

        to_remove = (rand()%2) + 4;
        printf("Dequeuing %d PCBs:\n", to_remove);
        for (d = 0; d < to_remove; d++) {
            pcb_ptr = pq_dequeue(my_pq);
            if (pcb_ptr != NULL) {
                out_str = PCB_to_string(pcb_ptr);
                if (out_str != NULL) {
                    printf("%s\n", out_str);
                    free(out_str);
                }
                PCB_destroy(pcb_ptr);
            }
        }
        printf("Dequeues complete.\n\n");
    }
    printf("Final state:\n");

    out_str = pq_to_string(my_pq);

    if (out_str != NULL) {
        printf("%s\n", out_str);
        free(out_str);
    }

    printf("\nDestroying PQ.\n");
    pq_destroy(my_pq);
    printf("Done.\n");
    printf("END_OF_PQ_TESTS\n");
}









// Creates a pcb and populates it with data
PCB_p make_pcb() {
    time_t t;
    PCB_p my_pcb = PCB_create();

    PCB_assign_PID(my_pcb);
    PCB_assign_state(my_pcb, STATE_READY); //0 for new state
    PCB_assign_priority(my_pcb, rand() % 16);
    PCB_assign_parent(my_pcb, my_pcb->pid-1);

    return my_pcb;
}
