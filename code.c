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
    int mSize;
    int pSize;
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
    c->pSize = p;
    c->mSize = m;
}
void delController(Controller **c){
    Controller* C = *c;

    if(C==NULL) return;

    for(int i=0;i<C->mSize;i++){
        delQueue(&(C->memQueue[i]));
        C->memQueue[i] = NULL;
    }
    free(C);
    *c = NULL;
}
void endCycle(Controller* c){
    c->totalCycle+=1;
    if(c->totalCycle==MAX_CYCLES) c->end = true;
}

int main()
{
    int pSize=0,mSize = 0;
    Controller* controller = NULL;
    bool endSimulation = false;
    while (!endSimulation)
    {
        printf("SETUP\n");
        delController(&controller);
           //setup stage------------------------
        
        if(mSize>=2048){
            mSize = 1;
            if(pSize>=64){
                endSimulation = true;
                break;
            }
            else    pSize<<=1;
        }else{
            mSize+=1;
            if(pSize==0)    pSize = 1;
        }

        // bool memory[mSize];
        // memset(memory,0,sizeof(memory));
        controller = newController(pSize,mSize);
        // Todo: put request on memQueue first
        // priority: starts from smaller index 0->p

        for (int i = 0; i < pSize; i++)
        {
            //put processor[i]'s request on queue
            int request = rand()%mSize;
            insertq(controller->memQueue[request],i);
        }
        //---------------
        printf("CYCLE start\n");
        // cycle start
        while(!controller->end){

            // memory access stage-----------
            for(int m=0;m<mSize;m++){
                Queue* curQ = controller->memQueue[m];
                if(!hasNext(curQ))   continue; 
                
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
                // //debug
                // printf("%d accessed memory[%d]\n",nP,m);
                // //
                popq(curQ);
                Queue* nQ = controller->memQueue[nP_request];
                insertq(nQ,nP);
            }
            //--------
            //endcycle stage---
            endCycle(controller);
        }
        printf("Finished at %d cycle: %d processors, %d mem, uniform distribution\n",
        controller->totalCycle, pSize, mSize);
    }
    printf("Finish whole simulation");
}

