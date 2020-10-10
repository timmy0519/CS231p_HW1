#ifndef QUEUE_H
#define QUEUE_H

struct queue
{
    int *arr;
    int length;
    int front;
    int rear;
} ;
typedef struct queue Queue;

void insertq(Queue*, int, int);

void display(Queue*, int );

void deleteq(Queue*, int);

Queue* newQueue(int);
#endif
