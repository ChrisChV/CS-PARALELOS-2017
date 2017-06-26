#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

typedef float Val;

int getBins(Val val, Val max, Val min, Val h){
	if(val < min || val > max) return -1;
	int count = 0;
	for(Val i = h; i <= max; i+=h){
		if(val <= i) return count;
		count++;
	}
	return -1;
}

void generateHistogram(Val * inputs, int size, Val ** histograms, int numHistograms, Val max, Val min, Val h){
	int my_rank = omp_get_thread_num();
	int numThreads = omp_get_num_threads();
	int n = size / numThreads;
	int i_init = my_rank * n;
	for(int i = i_init; i < i_init + n; i++){
		int index = getBins(inputs[i],max,min,h);
		#pragma opm critical
		(*histograms)[index]++;
	}
}

Val * getHistogram(Val * inputs, int size, int numHistograms, Val max, Val min, int numThreads, Val * h){
	Val * histograms = (Val *) malloc(sizeof(Val) * numHistograms);
	for(int i = 0; i < numHistograms; i++) histograms[i] = 0;
	*h = (max - min) / ((double) numHistograms);
	#pragma omp parallel num_threads(numThreads)
	generateHistogram(inputs,size,&histograms,numHistograms,max,min,*h);
	return histograms;
}

void printInput(Val * h, int numHistograms){
	for(int i = 0; i < numHistograms; i++){
		printf("%f\n", h[i]);
	}
}

void printHistogram(Val * hi, int numHistograms, Val min, Val max, Val h){
	int index = 0;
	for(Val i = min; i < max; i+=h, index++){
		printf("%f->%f\n", i,i+h);
		printf("%f\n", hi[index]);
	}
}

int main(int argv, char ** argc){
	if(argv != 4){
		printf("Faltan parametros <numThreads> <numHistograms> <histogramSize>\n");
		return 0;
	}
	srand(time(NULL));
	Val max = 10;
	Val min = 0;
	int numThreads = atoi(argc[1]);
	int numHistograms = atoi(argc[2]);
	int size = atoi(argc[3]);
	
	Val * inputs = (Val *) malloc(sizeof(Val) * size);
	for(int i = 0; i < size; i++){
		inputs[i] = rand() % ((int) (max) + 1);
	}
	Val h = 0;
	double start_time = omp_get_wtime();
	Val * res = getHistogram(inputs, size, numHistograms, max, min, numThreads, &h);
	double seconds = omp_get_wtime() - start_time;	
	//printInput(inputs,size);
	printf("\n");
	printHistogram(res,numHistograms,min,max,h);
	printf("Time%lf\n", seconds);
}



