/*static circular queue*/
#define MAX_CYCLES 10
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>
#include <time.h>
#include <math.h>
#define M_PI 3.14159265358979323846
//***********FUNC_DECLARACTION****************
struct queue
{
    int *arr;
    int length;
    int front;
    int rear;
    int byteSize;
    int size;
} ;
typedef struct queue Queue;
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
struct processor
{
  int access_time;
  int mem_mod_requested;
  double time_cumulative_avg;
};
typedef struct processor Processor;

Controller* newController(int,int);
void insertq(Queue*, int);
void display(Queue*);
void popq(Queue*);
Queue* newQueue(int);
bool hasNext(Queue *q);
void delQueue(Queue**);

void incrAccessTime(Processor * p);
int getAccessTime(Processor * p);

double sample_norm();
double sample_unif();
/**
 * args: -proc  : processor that generates the request
 *       -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(Processor * proc, int mSize, char d);

double calc_time_cumulative_avg(Processor * proc, int curr_cycle);
//************CONTROLLER**************
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
//************************************
//*********PROCESSOR****************

void incrAccessTime(Processor * p)
{
  (p->access_time)++;
}

int getAccessTime(Processor * p)
{
  return p->access_time;
}

/**
 * args: -proc  : processor that generates the request
 *       -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(Processor * proc, int mSize, char d)
{
  // return variable
  int new_mem_module_req_index = 0;

  // decide type of distribution to sample from
  if(d == 'n') // normal sampling
  {
    // generate a normally distributed number within norm(mu=proc->mmr, sd=mSize)
    new_mem_module_req_index = sample_norm()
                               * (mSize / 6)                // set std-dev here
                               + (proc->mem_mod_requested); // set mean here
  }
  else // uniform sampling
  {
    // generate a uniformly distributed number within [0, mSize]
    new_mem_module_req_index = sample_unif() * mSize;
  }

  // return new index within number of memory modules
  return new_mem_module_req_index % mSize;
}

double calc_time_cumulative_avg(Processor * proc, int curr_cycle)
{
  double proc_AT = (double) getAccessTime(proc);
  if(proc_AT == 0)
    return 0;
  return curr_cycle / proc_AT;
}

//*********************************
//***********QUEUE********************
void insertq(Queue* q, int item)
{
    int size = q->size;
    if ((q->front == 0 && q->rear == size - 1) || (q->front == q->rear + 1))
    {
        printf("queue is full to insert %d", item);
        display(q);
        return;
    }
    else if (q->rear ==  - 1)
    {
        q->rear++;
        q->front++;
    }
    else if (q->rear == size - 1 && q->front > 0)
    {
        q->rear = 0;
    }
    else
    {
        q->rear++;
    }
    q->arr[q->rear] = item;
}

void display(Queue* q)
{
    int size = q->size;
    int i;
    printf("\n");
    if (q->front > q->rear)
    {
        for (i = q->front; i < size; i++)
        {
            printf("%d ", q->arr[i]);
        }
        for (i = 0; i <= q->rear; i++)
            printf("%d ", q->arr[i]);
    }else if(q->front == -1){
        printf("The queue is empty");
    }
    else
    {
        for (i = q->front; i <= q->rear; i++)
            printf("%d ", q->arr[i]);
    }
}
bool hasNext(Queue *q){
    if(q->front!=-1) return true;
    return false;
}
void popq(Queue* q)
{
    int size = q->size;
    if (q->front ==  - 1)
    {
        printf("Queue is empty ");
    }
    else if (q->front == q->rear)
    {
        // printf("\n %d deleted", q->arr[q->front]);
        q->front =  - 1;
        q->rear =  - 1;
    }
    else
    {
        // printf("\n %d deleted", q->arr[q->front]);
        q->front = (q->front+1)%q->size;
    }
}

Queue* newQueue(int length){
    Queue* q = calloc(1,sizeof(Queue));
    q->arr = calloc(length,sizeof(int));
    q->front = -1;
    q->rear = -1;
    q->byteSize = sizeof(int)*length;
    q->size = length;
    return q;
}
void delQueue(Queue **q){
    if(*q==NULL) return;
    
    free((*q)->arr);
    // q->byteSize = -1;
    free(*q);
    *q = NULL;
}
//**********************************************
//*******SAMPLE*******************
double sample_unif() {
  // return a uniformly distributed random value
  return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

/**
 * returns a randomly sampled number from a normal distribution
 */
double sample_norm() {
  // return a normally distributed random value
  double v1=sample_unif();
  double v2=sample_unif();
  // Box-Muller inverse transform
  return cos(2 * M_PI * v2) * sqrt(-2. * log(v1));
}
//******************


int main()
{
    int pSize=1,mSize = 1;
    Controller* controller = NULL;
    bool endSimulation = false;
    FILE* fp;
    char fileName[80];
    sprintf(fileName,"p_%d_curve.txt",pSize);
    fp = fopen(fileName,"w");
    while (!endSimulation)
    {
        printf("SETUP\n");
        delController(&controller);
           //setup stage------------------------


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

        fprintf(fp,"%d, %d\n",mSize,controller->totalCycle);
        if(mSize>=2048){

            //one file per different p parameter
            fclose(fp);
            //
            mSize = 1;
            if(pSize>=64){
                endSimulation = true;
                break;
            }
            else    pSize<<=1;
            //open new file
            sprintf(fileName,"p_%d_curve.txt",pSize);
            fp = fopen(fileName,"w");
        }else{
            mSize+=1;

        }
    }
    printf("Finish whole simulation");
}

