#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include <mpi.h>

int main(){
	int my_rank;
	int comm_sz;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	int my_val = my_rank + 1;
	int sum = 0;
	int temp_val = 0;
	temp_val = my_val;
	sum = temp_val;
	int des = (my_rank + 1) % comm_sz;
	int source = my_rank - 1;
	if(source == -1) source = comm_sz -1;
	for(int i = 1; i < comm_sz; i++){
		MPI_Sendrecv_replace(&temp_val,1,MPI_INT,des,i,source,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		sum += temp_val;
	}
	printf("%d->%d\n",sum,my_rank);
	MPI_Finalize();
	return 0;
}