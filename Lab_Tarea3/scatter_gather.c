#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX_NUM 10
#define TAM_VECTOR 50


int * getRandomVector(int size){
	srand(time(NULL));
	int * res = (int *) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		res[i] = rand() % MAX_NUM;
	}
	return res;
}

int main(){
	int my_rank;
	int comm_sz;
	int local_n = 0;
	int * vector = NULL;
	int * local_vector = NULL;
	int * res_vector = NULL;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	local_n = TAM_VECTOR / comm_sz;
	local_vector = (int * ) malloc(sizeof(int) * TAM_VECTOR);
	if(my_rank == 0){
		vector = getRandomVector(TAM_VECTOR);
		printf("Vector Original->");
		for(int i = 0; i < TAM_VECTOR; i++){
			printf("%d ",vector[i]);
		}
		printf("\n");
		MPI_Scatter(vector,local_n,MPI_INT,local_vector,local_n,MPI_INT,0,MPI_COMM_WORLD);
	}
	else{
		MPI_Scatter(vector,local_n,MPI_INT,local_vector,local_n,MPI_INT,0,MPI_COMM_WORLD);
	}

	for(int i = 0; i < local_n; i++){
		local_vector[i] = local_vector[i] * 2;
	}

	if(my_rank == 0){
		res_vector = (int *) malloc(sizeof(int) * TAM_VECTOR);
		MPI_Gather(local_vector,local_n,MPI_INT,res_vector,local_n,MPI_INT,0,MPI_COMM_WORLD);
		printf("Vector Alterado->");
		for(int i = 0; i < TAM_VECTOR; i++){
			printf("%d ", res_vector[i]);
		}
		printf("\n");
	}
	else{
		MPI_Gather(local_vector,local_n,MPI_INT,res_vector,local_n,MPI_INT,0,MPI_COMM_WORLD);	
	}
	MPI_Finalize();
	return 0;
}