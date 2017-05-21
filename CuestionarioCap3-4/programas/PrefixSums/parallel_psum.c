#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../utils.h"

int getPrefixSum(int * vector, int size){
	int res = 0;
	for(int i = 0; i < size; i++){
		res += vector[i];
	}
	return res;
}

int main(){
	int my_rank;
	int comm_sz;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	int my_random = rand() % RAND_RANGE_NUM;
	int res = 0;
	int * vector = (int *) malloc(sizeof(int) * (my_rank + 1));
	int * vector_res = (int *) malloc(sizeof(int) * comm_sz);
	vector[my_rank] = my_random;
	if(my_rank == 0) MPI_Send(&my_random,1,MPI_INT,my_rank + 1,0,MPI_COMM_WORLD);
	else if(my_rank == comm_sz -1) MPI_Recv(vector,my_rank,MPI_INT,my_rank - 1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	else{
		MPI_Recv(vector,my_rank,MPI_INT,my_rank - 1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(vector,my_rank + 1,MPI_INT,my_rank + 1,0,MPI_COMM_WORLD);
	}
	res = getPrefixSum(vector,my_rank + 1);
	MPI_Gather(&res,1,MPI_INT,vector_res,1,MPI_INT,0,MPI_COMM_WORLD);
	if(my_rank == comm_sz - 1) printVector(vector,comm_sz);
	if(my_rank == 0) printVector(vector_res,comm_sz);
	MPI_Finalize();
	return 0;
}