/*static circular queue*/
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
Queue queue;

Queue* newQueue(int length){
    Queue* q = malloc(sizeof(Queue));
    q->arr = malloc(sizeof(int)*length);
    q->front = -1;
    q->rear = -1;
    return q;
}


int main()
{
    int size = 5;
    Queue* q = newQueue(5);
    insertq(q,5,7);
    display(q,5);
}

