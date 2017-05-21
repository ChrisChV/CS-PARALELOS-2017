#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../utils.h"

int suma(int * vector, int size){
	int res = 0;
	for(int i = 0; i < size; i++){
		res += vector[i];
	}
	return res;
}

int main(){
	int my_rank;
	int comm_sz;
	int n = 10;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	int * vector = getRandomVector(n);
	int * dis = (int*) malloc(sizeof(int) * comm_sz);
	int * scounts = (int*) malloc(sizeof(int) * comm_sz);
	int local_n = 0;
	int total_res;
	if(my_rank != comm_sz - 1) local_n = n / comm_sz;
	else local_n = (n / comm_sz) + (n % comm_sz);
	int * local_vector = (int *) malloc(sizeof(int) * local_n);
	for(int i = 0; i < comm_sz; i++){
		dis[i] = i * local_n;
		if(i != comm_sz - 1) scounts[i] = local_n;
		else scounts[i] = local_n + (n % comm_sz);
	}
	MPI_Scatterv(vector,scounts,dis,MPI_INT,local_vector,local_n,MPI_INT,0,MPI_COMM_WORLD);
	int res = suma(local_vector,local_n);
	MPI_Reduce(&res,&total_res,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	if(my_rank == 0) {
		printVector(vector,n);
		printf("%d\n",total_res);
	}


	MPI_Finalize();
	return 0;
}