#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../utils.h"

int main(){
	int my_rank;
	int comm_sz;
	int n = 10;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Datatype data;
	int local_n = n / comm_sz;
	int * vector = getRandomVector(n);
	int * local_vector = malloc(sizeof(int) * local_n);
	int * res = malloc(sizeof(int) * n);
	MPI_Type_contiguous(local_n,MPI_INT,&data);
	MPI_Type_commit(&data);
	MPI_Scatter(vector,1,data,local_vector,1,data,0,MPI_COMM_WORLD);

	for(int i = 0; i < local_n; i++){
		local_vector[i] = local_vector[i] * 2;
	}
	MPI_Gather(local_vector,1,data,res,1,data,0,MPI_COMM_WORLD);

	if(my_rank == 0){
		printVector(vector,n);
		printVector(res,n);
	}


	MPI_Finalize();
	return 0;
}