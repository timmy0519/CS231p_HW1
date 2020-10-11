#include "queue.h"

void insertq(Queue* q, int item)
{
    int size = q->size;
    if ((q->front == 0 && q->rear == size - 1) || (q->front == q->rear + 1))
    {
        printf("queue is full");
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
        q->front++;
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
    free((*q)->arr);
    // q->byteSize = -1;
    free(*q);
    *q = NULL;
}