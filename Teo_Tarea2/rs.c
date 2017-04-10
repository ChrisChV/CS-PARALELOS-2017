#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


int main(){
	srand(time(NULL));
	int id;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&id);
	if(id != 0){
		int random = rand() % 1000;
		int i = 0;
		printf("Mensaje Enviado\n");
		MPI_Rsend(&random,1,MPI_INT,0,0,MPI_COMM_WORLD);
		i += 10;
		i -= 5;
		printf("Proceso 1 terminado\n");
	}
	else{
		int res;
		MPI_Recv(&res,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		printf("Mensaje Recibido\n");
		res += 10;
		res -= 5;
		printf("Proceso 0 terminado->%d\n",res);
	}
	MPI_Finalize();
	return 0;
}