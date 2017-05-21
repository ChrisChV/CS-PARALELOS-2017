#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include <mpi.h>

int main(){
	int my_rank;
	int comm_sz;
	int n = 10;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	int * vector = getRandomVector(n);
	int * vector_res = (int*) malloc(sizeof(int) * n);
	int res;
	MPI_Scan(vector,vector_res,n,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
	if(my_rank == 0){
		printVector(vector,n);
	}
	if(my_rank == 1) printVector(vector_res,n);
	MPI_Finalize();
	return 0;
}