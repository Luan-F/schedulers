/**
 * Round Robin
 * 
 * Comando de compilação: gcc -lm rr.c ou clang -lm rr.c
 * */
#include <stdio.h>
#include <stdlib.h>
#include "basicProcess.h"

#define Q 2

typedef struct queue{
	int pIndex;
	struct queue* next;
} queue;

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime);
queue* startQueue();
void push(queue* q, int pIndex);
int pop(queue* q);
void rr(process* p, int n);
int min(int x, int y);

int main(int qArgs, char**arg){
	run(arg[1], atoi(arg[2]), &rr);

	return 0;
}

int min(int x, int y){
	return x > y ? x : y;
}

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime){
	p->totalTime += min(Q, p->execTime);
	
	if(p->totalTime == p->execTime){
		p->turnaroundTime = *totalTime - p->submissionTime;
		*totalTTime += p->turnaroundTime;
		p->waitingTime = p->turnaroundTime - p->execTime;
		*totalWaiting += p->waitingTime;
	}
	else{
		p->responseTime = *totalTime - p->submissionTime;
	}

	*totalTime += p->totalTime/2;
}

queue* startQueue(){
	queue* q = malloc(sizeof(queue));
	q->pIndex = -1;
	q->next = NULL;

	return q;
}

void push(queue* q, int pIndex){
	queue* aux = q;

	queue* n = malloc(sizeof(queue));
	n->pIndex = pIndex;
	n->next = NULL;

	while(aux->next != NULL){
		aux = aux->next;
	}

	aux->next = n;
}

int pop(queue* q){
	queue* aux = q->next;
	int res = aux->pIndex;

	q->next = aux->next;
	free(aux);

	return res;
}

void rr(process* p, int n){
	int lenQueue = 0;
	queue* q = startQueue();
	
	double totalTime, totalWaiting, totalTTime;

	int deadProcess = 0;

	totalTime = 0, totalWaiting = 0, totalTTime = 0;

	int i;
	for(i = 0 ; i < ALPHA ; i++){
		push(q, i);
	}
	lenQueue = ALPHA;

	while(deadProcess != n){
		if(lenQueue < ALPHA){
			push(q, i);
			i++;
			lenQueue++;
		}
		if(lenQueue > 0){
			int index = pop(q);
			execute(&p[index], &totalTime, &totalWaiting, &totalTTime);

			if(p[index].totalTime >= p[index].execTime){
				deadProcess++;
				lenQueue--;
			}
			else{
				push(q, index);
			}
		}

	}

	printf("==========================================\n");
	printf("Total Time spend: %lf\n", totalTime);
	printf("AVG Time spend: %lf\n", totalTime/n);
	
	printf("==========================================\n");
	printf("Total Time waiting: %lf\n", totalWaiting);
	printf("AVG Time waiting: %lf\n", totalWaiting/n);
	
	printf("==========================================\n");
	printf("Total Time turnaround: %lf\n", totalTTime);
	printf("AVG Time turnaround: %lf\n", totalTTime/n);
}
