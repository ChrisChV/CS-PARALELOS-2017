#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>
#include "tt.h"

void printMatrix(int ** matrix, int rows, int columns){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void llenarMatriz(int *** matrix, int rows, int columns, int init){
	*matrix = (int **) malloc(sizeof(void*) * rows);
	for(int i = 0; i < rows; i++){
		(*matrix)[i] = (int *) malloc(sizeof(int) * columns);
		for(int j = 0; j < columns; j++){
			(*matrix)[i][j] = init;
		}
	}
}

void * fun(void * arg){
	printMatrix(matrix,5,5);
}

int main(){
	int my_rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	if(my_rank == 0){
		llenarMatriz(&matrix,5,5,0);
		matrix[1][1] = 2;
		pthread_t threads[2];
		pthread_create(&threads[0],NULL,fun,NULL);
		pthread_create(&threads[1],NULL,fun,NULL);
		pthread_join(threads[0],NULL);
		pthread_join(threads[1],NULL);

	}
	else{
		llenarMatriz(&matrix,5,5,1);
		matrix[0][0] = 0;
		pthread_t threads[2];
		pthread_create(&threads[0],NULL,fun,NULL);
		pthread_create(&threads[1],NULL,fun,NULL);
		pthread_join(threads[0],NULL);
		pthread_join(threads[1],NULL);
	}
	
	
}