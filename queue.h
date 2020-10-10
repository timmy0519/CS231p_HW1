#ifndef QUEUE_H
#define QUEUE_H

struct queue
{
    int *arr;
    int length;
    int front;
    int rear;
    int byteSize;
} ;
typedef struct queue Queue;

void insertq(Queue*, int, int);

void display(Queue*, int );

void popq(Queue*, int);

Queue* newQueue(int);
// free memory
void delQueue(Queue**);
#endif
