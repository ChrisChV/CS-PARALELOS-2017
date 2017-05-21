#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

int numThreads;
char ** mensajes;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void * PC(void * arg){
	int rank = *((int*) arg);
	while(1){
		pthread_mutex_unlock(&mutex);
		if(rank % 2 != 0){
			if(mensajes[(rank - 1) / 2] != NULL){
				printf("M->%d->%s\n", rank,mensajes[(rank - 1) / 2]);
				mensajes[(rank - 1) / 2] = NULL;
				pthread_mutex_unlock(&mutex);
				break;
			}
		}
		else{
			char * temp = "Mi Mensaje";
			mensajes[rank/2] = temp;
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}


int main(int argc, char ** argv){
	if(argc != 2){
		printf("Faltan argumentos <numThreads>\n");
		return 0;
	}
	numThreads = atoi(argv[1]);
	if(numThreads % 2 != 0) return 0;
	pthread_t threads[numThreads];
	int threads_args[numThreads];
	mensajes = (char **) malloc(sizeof(char *) * numThreads / 2);
	for(int i = 0; i < numThreads / 2; i++){
		mensajes[i] = NULL;
	}
	for(int i = 0; i < numThreads; i++){
		threads_args[i] = i;
		pthread_create(&threads[i],NULL,PC,(void*) &threads_args[i]);
	}
	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}



}