/**
 * Lottery Scheduling
 * 
 * Comando de compilação: gcc -lm lottery.c ou clang -lm lottery.c
 * */
#include <stdio.h>
#include <time.h>
#include "basicProcess.h"

typedef struct queue{
	int pIndex;
	struct queue *next;
} queue;

queue* startQueue();
void push(queue* q, int pIndex);
int pop(queue* q);
void lottery(process* p, int n);
void* work(void* id);
void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime);

int main(int qArgs, char**arg){
	run(arg[1], atoi(arg[2]), &lottery);

	return 0;
}

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime){
	double timeSpend = p->execTime/2;
	p->totalTime += timeSpend;
	
	if(p->totalTime == p->execTime){
		p->turnaroundTime = *totalTime - p->submissionTime + p->blockTime;
		*totalTTime += p->turnaroundTime;
		p->waitingTime = p->turnaroundTime - p->execTime;
		*totalWaiting += p->waitingTime;
	}
	else{
		p->responseTime = *totalTime - p->submissionTime;
	}

	*totalTime += timeSpend;
}

queue* startQueue(){
	queue* q = malloc(sizeof(queue));
	q->pIndex = -1;
	q->next = NULL;

	return q;
}

void push(queue* q, int pIndex){
	queue* aux = q;

	while(aux->next != NULL){
		aux = aux->next;
	}

	aux->next = malloc(sizeof(queue*));
	aux->next->pIndex = pIndex;
	aux->next->next = NULL;
}

int pop(queue* q){
	queue* r = q->next;
	
	int pIndex = r->pIndex;

	q->next = r->next;

	free(r);

	return pIndex;
}

void lottery(process* p, int n){
	int lenReady = 0;
	double totalTime, totalWaiting, totalTTime;

	queue* readyQ = startQueue();

	totalTime = 0, totalWaiting = 0, totalTTime = 0;

	srand(time(NULL));

	int qTickets = 15*n;
	int tickets[qTickets];

	int liveProcess[n];

	int i, k, deadProcess = 0;

	for(i = 0, k = 0 ; i < n ; i++){
		liveProcess[i] = 1;
		tickets[k++] = i;
		tickets[k++] = i;
		tickets[k++] = i;
	}

	for( ; k < qTickets ; k++){
		tickets[k] = rand() % n;
	}

	while(deadProcess != n){

		if(lenReady < ALPHA){
			int r = rand() % qTickets;
			int cTicket = tickets[r];

			if(cTicket != -1 && liveProcess[cTicket]){
				push(readyQ, cTicket);
				lenReady++;
				tickets[r] = -1;
			}
		
		}

		if(lenReady > 0){
			int index = pop(readyQ);
			lenReady--;

			if(liveProcess[index]){
				execute(&p[index], &totalTime, &totalWaiting, &totalTTime);
				if(p[index].totalTime >= p[index].execTime){
					liveProcess[index] = 0;
					deadProcess++;
				}
				else{
					push(readyQ, index);
					lenReady++;
				}
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
