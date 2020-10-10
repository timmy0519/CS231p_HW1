// #include "code.c"
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int main(){
    int n, ch,sz;
    printf("\n Input Size:\n");
    scanf(" %d", &sz);
    Queue* queue = newQueue(sz);
    do
    {

        printf("\n\n Circular Queue:\n1. Insert \n2. Delete\n3. Display\n0. Exit");
        printf("\nEnter Choice 0-3? : ");
        scanf(" %d", &ch);
        switch (ch)
        {
            case 1:
                printf("\nEnter number: ");
                scanf("%d", &n);
                insertq(*queue, sz,n);
                break;
            case 2:
                popq(*queue, sz);
                break;
            case 3:
                display(*queue, sz);
                break;
        }
    }while (ch != 0);
}