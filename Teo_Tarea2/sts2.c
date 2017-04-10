#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX 100000

int main(){
	srand(time(NULL));
	int id;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	if(id != 0){
		int * random = (int *) malloc(sizeof(int) * MAX);
		for(int j = 0; j < MAX; j++){
			random[j] = rand() % 1000;
		}
		int i = 0;
		printf("Mensaje Enviado\n");
		MPI_Send(random,MAX,MPI_INT,0,0,MPI_COMM_WORLD);
		i += 10;
		i -= 5;
		printf("Proceso 1 terminado\n");
	}
	else{
		int * res = (int *) malloc(sizeof(int) * MAX);
		MPI_Recv(res,MAX,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Mensaje Recibido\n");
		res[0] += 10;
		res[0] -= 5;
		printf("Proceso 0 terminado->%d\n",res[0]);
	}
	MPI_Finalize();
	return 0;
}