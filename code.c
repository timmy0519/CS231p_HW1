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
  int access_counter;
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

void zeroOutProcessor(Processor * proc);
void incrAccessTime(Processor * p);
int getAccessTime(Processor * p);
void incrAccessCounter(Processor * p);
int getAccessCounter(Processor * p);

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

void zeroOutProcessor(Processor * proc)
{
  proc->access_time = 0;
  proc->access_counter = 0;
  proc->mem_mod_requested = 0;
  proc->time_cumulative_avg = 0.0;
}

void incrAccessTime(Processor * p)
{
  (p->access_time)++;
}

void incrAccessCounter(Processor * p)
{
  (p->access_counter)++;
}

int getAccessTime(Processor * p)
{
  return p->access_time;
}

int getAccessCounter(Processor * p)
{
  return p->access_counter;
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
  if(new_mem_module_req_index < 0)
  {
    new_mem_module_req_index = mSize + new_mem_module_req_index;
  }
  return new_mem_module_req_index % mSize;
}

double calc_time_cumulative_avg(Processor * proc, int curr_cycle)
{
  double proc_AC = (double) getAccessCounter(proc);
  if(proc_AC == 0)
    return 0;
  return curr_cycle / proc_AC;
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


int main(int argc, char * argv[])
{
    // expected number of arguments
    if(argc != N_ARGS + 1)
      return -1;

    int pSize=1,mSize = 1;
    Controller* controller = NULL;
    bool endSimulation = false;
    FILE* fp;
    char fileName[80];
    sprintf(fileName,"p_%d_curve.txt",pSize);
    fp = fopen(fileName,"w");

    // init variables with command line arguements
    pSize = atoi(argv[1]);
    char dist_type = argv[2][0];

    // main loop
    while (!endSimulation)
    {
        printf("SETUP\n");
        delController(&controller);
        //setup stage------------------------

        // init array of memory modules "taken" status
        bool memory[mSize];
        memset(memory,0,sizeof(memory)); // zero out array
        // init controller
        controller = newController(pSize,mSize);
        // initialize array of processor structs
        Processor processors[pSize];
        for(int i = 0; i < pSize; i++)
        {
          zeroOutProcessor(processors + i);
          // if using normal distribution workload, then set mu_pi
          if(dist_type == 'n')
          {
            processor[i].mem_mod_requested = generate_request(processor + i, mSize, 'u');
          }
        }

        for (int i = 0; i < pSize; i++)
        {
            //put processor[i]'s request on queue
            int request = generate_request(processor + i, mSize, dist_type);
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
                int proc_m_request_indx = curQ->arr[m]; // TODO: left off here
                popq(curQ);
            }
            //-------
            

            //generating request stage
            for(int m=0;m<mSize;m++){
                Queue* curQ = controller->memQueue[m];
                if(!hasNext(curQ)) continue;

                int nP = curQ->arr[curQ->front]; //next processor's index
                int nP_request = generate_request(processor + nP, mSize, dist_type); //generate new request 
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

        printf("Finished at %d cycle: %d processors, %d mem, %c distribution\n",
        controller->totalCycle, pSize, mSize, dist_type);

        fprintf(fp,"%d, %d\n",mSize,controller->totalCycle);

        // check if we finished simulating m = [0, 2048]
        if(mSize>=2048){

            //one file per different p parameter
            fclose(fp);
            //
            mSize = 1;
            // check if we finished simulating p = [2, 4, 8, 16, 32, 64]
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

