#include math.h
#include "queue.h"
#define MAXQUEUESIZE 8

/* Global variable declarations */
typedef int ItemType;
typedef struct {
    ItemType item[MAXQUEUESIZE];
    int count;
    int front;
    int rear;
} queue;

/* Prototypes declarations */
void storeBinaryInQueue(queue *q, int decimal);
void doCircularshiftInQueue(queue *q, int cycle);
int powerOf(int num, int pow);
void dequeuPrintDecimel(queue *q);

void main(int argc, char const *argv[])
{
    printf("%s\n", argv[1]);
}

void storeBinaryInQueue(queue *q, int decimal) {
    while (decimal>0) {
        enqueue(q, decimal%2);
        decimal = decimal / 2;
    }
}

void doCircularshiftInQueue(queue *q, int cycle) {
    int temp;

    if (cycle>0 && cycle<=q->count) {
        while(cycle>0) {
            q->item[q->front]=q->item[q->rear];
            cycle--;
        }
    } else {
        printf("Invalid cycle");
    }
}

int powerOf(int num, int pow) {
    int i, j;

    for(i=0;i<=pow;i++) {
        num+=num;
    }

    for(j=0;j<=pow;j++) {
        num+=num;
    }

    return num;
}

void dequeuPrintDecimal(queue *q) {
    int dec;

    dec = 0;

    while(q->count>=0) {
        if (dequeue(q)>0) {
            dec += powerOf(2,q->count+1);
        }
    }
}