#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_NUM 10;

int * getRandomMatrix(int size){
	srand(time(NULL));
	int * res = (int *) malloc(sizeof(int) * size * size);
	for(int i = 0; i < size * size; i++){
		res[i] = rand() % MAX_NUM + 1;
	}
	return res;
}

int * getRandomVector(int size){
	srand(time(NULL));
	int * res = (int *) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		res[i] = rand() % MAX_NUM + 1;
	}
	return res;
}

void matrixVectorMultiplication(int * local_matrix, int * vector, int local_n, 
									int matrixSize, int * res, double * local_elapsed){
//	int * res = (int *) malloc(sizeof(int) * local_n);	
	//double local_start = MPI_Wtime();
	clock_t local_start = clock();
	for(int i = 0; i < local_n; i++){
		res[i] = 0;
		for(int j = 0; j < matrixSize; j++){
			res[i] += local_matrix[i*matrixSize+j] * vector[j];
		}
	}
	clock_t local_finish = clock();
	//double local_finish = MPI_Wtime();
	//*local_elapsed = local_finish - local_start;
	*local_elapsed = (double)(local_finish - local_start) / CLOCKS_PER_SEC;
//	return res;
}

int main(int argc, char ** argv){
	if(argc != 3){
		printf("Malos Argumentos: <matrixSize> <iteraciones>\n");
		return 0;
	}
	int matrixSize = atoi(argv[1]);
	int iteraciones = atoi(argv[2]);
	int * matrix = NULL;
	int * vector = getRandomVector(matrixSize);
	int * local_vector = NULL;
	int * local_matrix = NULL;
	int my_rank;
	int comm_sz;
	int local_n = 0;
	double local_elapsed = 0;
	double elapsed = 0;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	local_n = matrixSize / comm_sz;
	local_matrix = (int *) malloc(sizeof(int) * local_n * matrixSize);
	if(my_rank == 0){
		matrix = getRandomMatrix(matrixSize);
		/*
		printf("Matriz->");
		for(int i = 0; i < matrixSize * matrixSize; i++){
			printf("%d ", matrix[i]);
		}
		printf("\n");
		printf("Vector Original->");
		for(int i = 0; i < matrixSize; i++){
			printf("%d ", vector[i]);
		}
		printf("\n");
		*/
	}
	
	MPI_Scatter(matrix,local_n * matrixSize,MPI_INT,local_matrix,local_n * matrixSize,MPI_INT,0,MPI_COMM_WORLD);
	for(int i = 0; i < iteraciones; i++){
		local_vector = (int *) malloc(sizeof(int) * local_n);

		MPI_Barrier(MPI_COMM_WORLD);
		matrixVectorMultiplication(local_matrix, vector, local_n, matrixSize, local_vector, &local_elapsed);
		
		MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

		MPI_Allgather(local_vector, local_n, MPI_INT, vector, local_n, MPI_INT, MPI_COMM_WORLD);
		if(my_rank == 0){
			/*	
			printf("Resultado itracion %d->",i);
			for(int i = 0; i < matrixSize; i++){
				printf("%d ", vector[i]);
			}
			printf("\n");
			*/
			elapsed = elapsed * 1000;
			printf("%lf",elapsed);
		}
		free(local_vector);
	}
	if(my_rank == 0) free(matrix);
	free(vector);
	free(local_matrix);
	MPI_Finalize();
	return 0;
}