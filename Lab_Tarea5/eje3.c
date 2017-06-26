#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

void ParallelMemcpy(int * a, int * temp, int n){
	int my_rank = omp_get_thread_num();
	int numThreads = omp_get_num_threads();
	int local_n = n / numThreads;
	int i = my_rank * local_n;
	memcpy(&a[i],&temp[i], local_n * sizeof(int));
}

void Count_sort (int * a, int n , int numThreads) {
	int i, j, count;
	int * temp = malloc ( n *sizeof(int));
	#pragma omp parallel for num_threads(numThreads) private(count,j)
	for (i = 0; i < n ; i ++) {
		count = 0;
		for (j = 0; j < n; j ++){
			if (a[j] < a [i]) count ++;
			else if (a[j] == a[i] && j < i) count ++;
		}
		temp[count] = a[i];
	}
	//memcpy(a,temp, n *sizeof(int));
	#pragma omp parallel num_threads(numThreads)
	ParallelMemcpy(a,temp,n);
	free(temp);
}

void print(int * a, int n){
	for(int i = 0; i < n; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}

int main(int argv, char ** argc){
	if(argv != 3){
		printf("Faltan argumentos <numThreads> <tamVector>\n");
		return 0;
	}
	srand(time(NULL));
	int numThreads = atoi(argc[1]);
	int n = atoi(argc[2]);
	int * a = (int *) malloc(sizeof(int) * n);
	for(int i = 0; i < n; i++){
		a[i] = rand() % 10;
	}
	//print(a,n);
	double start_time = omp_get_wtime();
	Count_sort(a,n,numThreads);
	double seconds = omp_get_wtime() - start_time;	
	//print(a,n);
	printf("Time->%lf\n", seconds);

}


// a) Las variables count y j tienen que ser privadas, todas las demas pueden quedarse como shared.
// b) No hay dependencias, cada elemento puede ser calculado independientemente de los demas.
// c) Yes
// d) Yes
// e) 
