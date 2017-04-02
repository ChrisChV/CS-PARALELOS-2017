#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LIMITE 10

int main(){
	int value = 0;
	int my_rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	if(my_rank != 0){
		while(value < LIMITE){
			MPI_Recv(&value,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			if(value == LIMITE) break;
			printf("Proceso 2->%d\n", value);
			value++; 
			MPI_Send(&value,1,MPI_INT,0,0,MPI_COMM_WORLD);
		}
	}
	else{
		while(value < LIMITE){
			MPI_Send(&value,1,MPI_INT,1,0,MPI_COMM_WORLD);
			MPI_Recv(&value,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			if(value == LIMITE) break;
			printf("Proceso 1->%d\n",value);
			value++;
		}
		printf("HELLO\n");
		MPI_Send(&value,1,MPI_INT,1,0,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}