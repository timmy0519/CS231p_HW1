/*static circular queue*/
#define MAX_CYCLES 10
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <stdbool.h> 
#include <string.h>
struct controller
{
    /* data */
    Queue** memQueue;
    int totalCycle;
    bool end;
};
typedef struct controller Controller;
Controller* newController(int p,int m){
    // array of priorityQueue for each mem
    Controller* c = calloc(1,sizeof(Controller));
    c->memQueue = (Queue**)calloc(m,sizeof(Queue*));
    for(int i=0;i<m;i++){
        c->memQueue[i] = newQueue(p);
    }
    c->totalCycle = 0;
    c->end = false;
}
void endCycle(Controller* c){
    c->totalCycle+=1;
    if(c->totalCycle==MAX_CYCLES) c->end = true;
}

int main()
{
    
    
    //setup stage------------------------
    int pSize=10,mSize = 5;
    int  totalCycle = 0; 
    bool memory[mSize];
    memset(memory,0,sizeof(memory));
    Controller* controller = newController(pSize,mSize);

    // Todo: put request on memQueue first
    // priority: starts from smaller index 0->p

    for (int i = 0; i < pSize; i++)
    {
        //put processor[i]'s request on queue
        int request = rand()%mSize;
        insertq(controller->memQueue[request],i);
    }
    //---------------
        
    // cycle start
    while(!controller->end){

        // memory access stage-----------
        
        for(int m=0;m<mSize;m++){
            Queue* curQ = controller->memQueue[m];
            if(memory[m] || !hasNext(curQ))   continue; 
            
            //Todo:
            //process first request and update the metadata(average access time) inside processor
            //don't generate next request -> avoid same processor accesses more than one mem
            
        }
        //-------


        //generating request stage
        for(int m=0;m<mSize;m++){
            Queue* curQ = controller->memQueue[m];
            if(!hasNext(curQ)) continue;

            int nP = curQ->arr[curQ->front]; //next processor's index
            int nP_request = rand()%mSize;; //generate new request 
            //debug
            printf("%d accessed memory[%d]\n",nP,m);
            //
            popq(curQ);
            Queue* nQ = controller->memQueue[nP_request];
            insertq(nQ,nP);
        }
        //--------

        //endcycle stage---
        endCycle(controller);
    }
}

