/**
 * Highest Response Ratio
 * 
 * Comando de compilação: gcc -lm hrrn.c ou clang -lm hrrn.c
 * */
#include <stdio.h>
#include <stdlib.h>
#include "basicProcess.h"

void hrrn(process* p, int n);
double calcRR(process p, int totalTime);
void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime);

int main(int qArgs, char**arg){
	run(arg[1], atoi(arg[2]), &hrrn);

	return 0;
}

double calcRR(process p, int totalTime){
	double w = totalTime - p.submissionTime + p.blockTime;
	double s = p.execTime;
	double r = (w+s)/s;

	return r;
}

void execute(process* p, double* totalTime, double* totalWaiting, double* totalTTime){
	double timeSpend = p->execTime/2;
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

	*totalTime += timeSpend;
}

void hrrn(process* p, int n){
	double totalTime, totalWaiting, totalTTime;

	int deadProcess = 0;

	totalTime = 0, totalWaiting = 0, totalTTime = 0;
	
	while(deadProcess != n){
		double hrr = -999;
		int nextProcess;

		for(int i = 0 ; i < n ; i++){
			if(p[nextProcess].totalTime == p[nextProcess].execTime){
				continue;
			}
			
			double rr = calcRR(p[i], totalTime);
	
			if(hrr < rr){
				hrr = rr;
				nextProcess = i;
			}
		}

		execute(&p[nextProcess], &totalTime, &totalWaiting, &totalTTime);
		if(p[nextProcess].totalTime >= p[nextProcess].execTime){
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
