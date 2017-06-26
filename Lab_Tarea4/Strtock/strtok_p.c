#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>

#define MAX 255

int numThread;
sem_t * semaforos;
FILE * pFile;


void * tokenize(void * rank){
	long my_rank = *((long*)rank);
	int count = 0;
	int next = (my_rank + 1) % numThread;
	char * fg_rv;
	char my_line[MAX];
	char * my_string;
	char * saveptr;
	sem_wait(&semaforos[my_rank]);
	fg_rv = fgets(my_line,MAX,pFile);
	sem_post(&semaforos[next]);
	while(fg_rv != NULL){
		printf("Thread %ld > my line = %s", my_rank, my_line);
		count = 0;
		my_string = strtok_r(my_line," \t\n",&saveptr);
		while(my_string != NULL){
			count++;
			printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
			my_string = strtok_r(NULL, " \t\n",&saveptr);
		}
		sem_wait(&semaforos[my_rank]);
		fg_rv = fgets(my_line, MAX, pFile);
		sem_post(&semaforos[next]);
	}

	
	return NULL;
}

int main(int argc, char ** argv){
	if(argc != 2){
		printf("Faltan Argumentos <numThread>\n");
		return 0;
	}
	numThread = atoi(argv[1]);
	semaforos = malloc(sizeof(sem_t) * numThread);
	pFile = fopen("strTest","r");
	for(int i = 0; i < numThread; i++){
		if(i == 0){
			sem_init(&semaforos[i],0,1);
		}
		else{
			sem_init(&semaforos[i],0,0);
		}
	}
	pthread_t threads[numThread];
	long threads_args[numThread];
	for(long i = 0; i < numThread; i++){
		threads_args[i] = i;
		pthread_create(&threads[i],NULL,tokenize,(void*) &threads_args[i]);
	}
	for(int i = 0; i < numThread; i++){
		pthread_join(threads[i],NULL);
	}

}