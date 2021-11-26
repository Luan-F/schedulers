#include <stdlib.h>
#include <math.h>

#define ALPHA 100

typedef struct process {
	int id, priority;
	double submissionTime, execTime, blockTime;
	double totalTime;
	double responseTime, turnaroundTime, waitingTime;
	int ready;
} process;

process createProcess(int id, int priority, int submissionTime, int execTime, int blockTime);
void printProcess(process p);
void run(char* filename, int n, void (*scheduler)(process*, int));
void intercala(process* p, int l, int m, int r);
void merge(process* p, int l, int r);

process createProcess(int id, int priority, int submissionTime, int execTime, int blockTime){
	process p = {
		id, priority,
		submissionTime, execTime, blockTime,
		0.0,
		0.0, 0.0, 0.0,
		0
	};

	return p;
}

void printProcess(process p){
	printf("[%d]\t%d\t%f\t%f\t%f\n", p.id, p.priority, p.submissionTime, p.execTime, p.blockTime);
}

void run(char* filename, int n, void (*scheduler)(process*, int)){
	FILE *fp;
	fp = fopen(filename, "r");

	process* p = malloc(sizeof(process)*n);

	char line[256];
	for(int i = 0 ; i < n ; i++){
		int id, priority, sT, eT, bT;
	
		fgets(line, 256, fp);
		int count = sscanf(line, "%d,%d,%d,%d,%d", &id, &sT, &priority, &eT, &bT);
		p[i] = createProcess(id, priority, sT, eT, bT);
//		printProcess(p[i]);
	}

	fclose(fp);

	merge(p, 0, n-1);

	(*scheduler)(p, n);

	free(p);
}

void intercala(process* p, int l, int m, int r){
	int size_a = m-l+1;
	process* a = malloc(sizeof(process)*size_a);

	int size_b = r-m;
	process* b = malloc(sizeof(process)*size_b);

	int i, j, k;
	for(i = l ; i <= m ; i++){
		a[i-l] = p[i];
	}
	
	for(j = m+1 ; j <= r ; j++){
		b[j-m-1] = p[j];
	}

	i = 0, j = 0, k = l;

	while(i < size_a && j < size_b){
		if(a[i].submissionTime <= b[j].submissionTime){
			p[k] = a[i];
			i++;
		}
		else{
			p[k] = b[j];
			j++;
		}
		k++;
	}

	while(i < size_a){
		p[k] = a[i];
		k++;
		i++;
	}
	
	while(j < size_b){
		p[k] = b[j];
		k++;
		j++;
	}

	free(a);
	free(b);
}

void merge(process* p, int l, int r){
	if(l < r){
		int m = floor((l+r)/2);
		merge(p, l, m);
		merge(p, m+1, r);
		intercala(p, l, m, r);
	}
}
