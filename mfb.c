/**
 * Multilevel feedback
 * 
 * Comando de compilação: gcc -lm mfb.c ou clang -lm mfb.c
 * */
#include <stdio.h>
#include <stdlib.h>
#include "basicProcess.h"

typedef struct queue{
	int pIndex;
	struct queue* next;
} queue;

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime);
queue* startQueue();
void push(queue* q, int pIndex);
int pop(queue* q);
void mfb(process* p, int n);

int main(int qArgs, char**arg){
	run(arg[1], atoi(arg[2]), &mfb);

	return 0;
}

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime){
	p->totalTime += p->execTime/2;
	
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

void mfb(process* p, int n){
	int lenQueue1, lenQueue2;
	queue *q1, *q2;

	q1 = startQueue();
	q2 = startQueue();

	double totalTime, totalWaiting, totalTTime;

	totalTime = 0, totalWaiting = 0, totalTTime = 0;

	int deadProcess = 0;

	for(int i = 0 ; i < n ; i++){
		push(q1, i);
	}
	lenQueue1 = n;

	while(deadProcess != n){
		if(lenQueue1 > 0){
			int index = pop(q1);
			execute(&p[index], &totalTime, &totalWaiting, &totalTTime);
			push(q2, index);
			lenQueue1--;
			lenQueue2++;
		}
		if(lenQueue2 > 0){
			int index = pop(q2);
			execute(&p[index], &totalTime, &totalWaiting, &totalTTime);
			lenQueue2--;
			deadProcess++;
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
