#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.h"
#include "task.h"
#include "cpu.h"

int id = 0;
int totaltask = 0;
int count = 0;

struct node *head = NULL;
struct node *current;
double waitingTime = 0;
int waiting = 0;
int wait;
int turnaround = 0;
double turnaroundTime = 0;
double burst0 = 0;
double totalResponseTime = 0;
    
  

int execTime = 0;
// int bursts[1024];
// int wait[1024];
// int remainingBurst[1024];
// int response[1024];




void calculatingTime();
void calculatingAverage();
void roundRobin(struct node *current);



void add(char *name, int priority, int cpuburst) {
    Task *new_task = (Task*)malloc(sizeof(Task));
    new_task->name = name;
    new_task->priority = priority;
    new_task->burst = cpuburst;
    id++;
    new_task->tid = id;



    insert(&head, new_task);

    totaltask++;
    

}

struct node *prev = NULL;
struct node *next = NULL;



void schedule() {
    current = head;

    while(current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;

    roundRobin(head);
    calculatingTime();
    calculatingAverage();
}

int count1 = 0;


void roundRobin(struct node *current) {


    while (current != NULL) {
        if (current->task->burst <= QUANTUM) {
            run(current->task, current->task->burst);
            count1++;
            delete(&head, current->task);
            current = current->next;
            if(current == NULL) {
                current = head;
            }
        } else {
            run(current->task, QUANTUM);
            count1++;
            current->task->burst -= QUANTUM;
            current = current->next;
            if (current == NULL) {
                current = head;
            }
        }

        
    }
    
}


void calculatingTime() {

    count = totaltask;
    current = head;
    

    while(count > 0) {      
         if(current != NULL) {
            burst0 = current->task->burst;
            current = current->next;
         }  

    for(int i = 0; i < totaltask; i++) {

        if(burst0 > 0) {
            if(burst0 > QUANTUM) {
                execTime += QUANTUM;
                burst0 = burst0 - QUANTUM;
            }
            else {
                    execTime += burst0;
                    waiting = execTime - current->task->burst - waiting;
                    burst0 = 0;
                    count--;
                }
            }
        }

        wait += waiting;
        turnaround += waiting + current->task->burst;
    }


}


void calculatingAverage() {


   for(int i = 0; i < totaltask; i++) {
        waitingTime += wait;
        turnaroundTime += turnaround;
        // totalResponseTime += response[i];
        
        // if(bursts[i] > QUANTUM) {
        //     totalResponseTime += QUANTUM;
        //     remainingBurst[i] = remainingBurst[i] - QUANTUM;
        // }
        // else {
        //     totalResponseTime += remainingBurst[i];
        //     remainingBurst[i] = 0;
        // }
    }




    waitingTime = waitingTime / (double)totaltask;
    turnaroundTime = turnaroundTime / (double)totaltask;
    totalResponseTime = totalResponseTime / (double)totaltask;

    printf("\n");

    printf("Average waiting time = %.2f\n", waitingTime);
    printf("Average turnaround time = %.2f\n", turnaroundTime);
    printf("Average response time = %.2f\n", totalResponseTime);
}

