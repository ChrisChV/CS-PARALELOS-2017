#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

pthread_barrier_t barrier;

void * fun(void * arg){
	int rank = *((int*)arg);
	pthread_barrier_wait(&barrier);
	printf("MY->%d\n", rank);
	return NULL;
}

int main(int argv, char ** argc){
	if(argv != 2){
		printf("Faltan argumentos <numThreads>\n");
		return 0;
	}
	int numThreads = atoi(argc[1]);
	pthread_t threads[numThreads];
	int threads_args[numThreads];
	pthread_barrier_init(&barrier,NULL,numThreads);
	for(int i = 0; i < numThreads; i++){
		threads_args[i] = i;
		pthread_create(&threads[i],NULL,fun,(void*) &threads_args[i]);
	}
	for(int i = 0; i< numThreads; i++){
		pthread_join(threads[i],NULL);
	}
}