/*static circular queue*/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <stdbool.h> 
struct controller
{
    /* data */
    Queue* memQueue;
    int totalCycle;
    bool end;
};
typedef struct controller Controller;
Controller* newController(int sz){
    // array of priorityQueue for each mem
    Controller* c = calloc(1,sizeof(Controller));
    c->memQueue = calloc(sz,sizeof(Queue*));
    for(int i=0;i<sz;i++){
        c->memQueue[i] = *newQueue(sz);
    }
    c->totalCycle = 0;
    c->end = false;

}
int main()
{
    Controller* controller = newController(5);
    display(controller->memQueue[0],5);
}

