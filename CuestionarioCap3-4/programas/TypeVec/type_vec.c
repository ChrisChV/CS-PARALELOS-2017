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
	int local_n  = n / comm_sz;
	int * vector = getRandomVector(n);
	int * local_vector = (int*) malloc(sizeof(int) * local_n);
	int * res_vector = (int*) malloc(sizeof(int) * n);
	MPI_Type_vector(1,local_n,n + 1,MPI_INT,&data);
	MPI_Type_commit(&data);
	if(my_rank == 0){
		printVector(vector,n);
		for(int i = 0; i < local_n;i++){
			local_vector[i] = vector[i];
		}
		for(int i = 1; i < comm_sz; i++){
			MPI_Send(&vector[i * local_n],1,data,i,0,MPI_COMM_WORLD);
		}
	}
	else{
		MPI_Recv(local_vector,1,data,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	for(int i = 0; i < local_n; i++){
		local_vector[i] = local_vector[i] * 2;
	}

	if(my_rank != 0){
		MPI_Send(local_vector,1,data,0,0,MPI_COMM_WORLD);
	}
	else{
		for(int i = 0; i < local_n; i++){
			res_vector[i] = local_vector[i];
		}
		for(int i = 1; i < comm_sz; i++){
			MPI_Recv(&res_vector[i * local_n],1,data,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		printVector(res_vector,n);
	}
	MPI_Finalize();
	return 0;
}