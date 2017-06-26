#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int main(int argc, char ** argv){
	if(argc != 3){
		printf("Faltan argumentos <num_threads> <num_of_tosses>\n");
		return 0;
	}
	int numThreads = atoi(argv[1]);
	int number_of_tosses = atoi(argv[2]);
	int number_in_circle = 0;
	double start_time = omp_get_wtime();
	#pragma omp parallel for num_threads(numThreads)
	for (int toss = 0; toss < number_of_tosses ; toss ++) {
		double x = (double)rand()/RAND_MAX*2.0-1.0;;
		double y = (double)rand()/RAND_MAX*2.0-1.0;;
		double distance_squared = x * x + y * y ;
		if (distance_squared <= 1){
			#pragma omp critical
			number_in_circle++;	
		} 
	}
	double seconds = omp_get_wtime() - start_time;	
	double pi_estimate = 4* number_in_circle /((double) number_of_tosses);
	printf("%lf\n", pi_estimate);
	printf("Time->%lf\n", seconds);
}