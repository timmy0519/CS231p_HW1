#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

void insertq(Queue*, int);

void display(Queue*);

void popq(Queue*);

Queue* newQueue(int);
bool hasNext(Queue *q);
// free memory
void delQueue(Queue**);
#endif
