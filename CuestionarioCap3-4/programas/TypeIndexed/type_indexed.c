#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../utils.h"

int main(){
	int my_rank;
	int comm_sz;
	int n = 4;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Datatype data;
	int * matrix = getRandomVector(n * n);
	int * res_matrix = getZeroVector(n * n);
	int block_sizes[n];
	int displ[n];
	for(int i = 0; i < n; i++){
		block_sizes[i] = n - i;
	}
	for(int i = 0; i < n; i++){
		displ[i] = (i) * (n + 1);
	}
	MPI_Type_indexed(n,block_sizes,displ,MPI_INT,&data);
	MPI_Type_commit(&data);
	if(my_rank == 0){
		printVectorMatrix(matrix,n,n);
		MPI_Send(matrix,1,data,1,0,MPI_COMM_WORLD);
	} 
	else{
		MPI_Recv(res_matrix,1,data,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);	
		printVectorMatrix(res_matrix,n,n);
	} 
	
	MPI_Finalize();
	return 0;
}