#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RSA.h"


//Time encriptacion
//Time desencriptacion
//Time keyGeneration

/**
	TODO 
	-Hacer que las keys se generen en el nodo 0 y se repartan para todos.
	-Mejorar las funciones de paralelizacion.
	-PRUEBAS
**/

void printCypher(char ** cypher, int tamC){
	int tamL = 0;
	for(int i = 0; i < tamC; i++){
		printf("%s\n", cypher[i]);
	}
	printf("\n");
}

char * getFile(char * file, int * count){
	FILE * archivo = fopen(file,"r");
	char temp = fgetc(archivo);
	char * cc;
	*count = 1;
	while(temp != EOF){
		if(*count == 1) cc = (char *) malloc(*count);
		else cc = (char *) realloc(cc,*count);
		cc[*count - 1] = temp;
		(*count)++;
		temp = fgetc(archivo);
	}
	fclose(archivo);
	(*count)--;
	return cc;
}

void allgather(char *** cypher, char ** res, int tamBase, int n, int local_n, int mpi_size, int my_rank){
	int * sizes = (int *) malloc(sizeof(int) * tamBase);
	int * local_size = (int *) malloc(sizeof(int) * local_n);
	for(int i = 0; i < local_n; i++){
		local_size[i] = strlen(res[i]) + 1;
	}
	int * count = (int *) malloc(sizeof(int) * mpi_size);
	int * displ  = (int *) malloc(sizeof(int) * mpi_size);
	for(int i = 0; i < mpi_size; i++){
		if(i == mpi_size - 1) count[i] =  n + (tamBase % mpi_size);
		else count[i] = n;
		displ[i] = i * n;

	}
	MPI_Allgatherv(local_size,local_n,MPI_INT,sizes,count,displ,MPI_INT,MPI_COMM_WORLD);

	int i_init = my_rank * n;
	int i_end = i_init + local_n;
	int bcast_rank = 0;
	for(int i = 0; i < tamBase; i++){

		(*cypher)[i] = (char *) malloc(sizes[i]);

		bcast_rank = i / n;
		if(bcast_rank == mpi_size) bcast_rank--;
		if(bcast_rank == my_rank){
			(*cypher)[i] = res[i- i_init];
			(*cypher)[i][sizes[i] - 1] = '\0';
		}
		
		MPI_Bcast((*cypher)[i],sizes[i],MPI_CHAR,bcast_rank,MPI_COMM_WORLD);


	}
}


/**
	
**/

int main(int argc, char **argv){
	if(argc != 4){
		printf("Faltan argumentos <numThreads> <tamNumber> <file>\n");
		return 0;
	}
	int my_rank;
	int mpi_size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
	
	int numThreads = atoi(argv[1]);
	int tamNumber = atoi(argv[2]);
	double timeSpend = 0;
	char * file = argv[3];
	
	char ** res_c = NULL;
	char * res_d = NULL;

	Keys keys;
	int tamBase = 0;
	double timeMax = 0;
	double tk = 0;
	char * cc = NULL;
	int * tamKeys = (int *) malloc(sizeof(int) * 3);

	//Se generan las keys, el mensaje y el tamBase en el nodo 0 y luego se distribuye a los demas.	
	if(my_rank == 0){
		clock_t ini = clock();
		keys = keyGeneration(tamNumber);
		clock_t end = clock();
		tk = (double)(end - ini) / CLOCKS_PER_SEC;
		tamKeys[0] = (int) strlen(keys.publicKey);
		tamKeys[1] = (int) strlen(keys.privateKey);
		tamKeys[2] = (int) strlen(keys.modulus);
		cc = getFile(file,&tamBase);
	}
	MPI_Bcast(tamKeys,3,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&tamBase,1,MPI_INT,0,MPI_COMM_WORLD);
	if(my_rank != 0){
		keys.publicKey = (char *) malloc(tamKeys[0]);
		keys.privateKey = (char *) malloc(tamKeys[1]);
		keys.modulus = (char *) malloc(tamKeys[2]);
		cc = (char *) malloc(tamBase);
	}
	MPI_Bcast(keys.publicKey,tamKeys[0],MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(keys.privateKey,tamKeys[1],MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(keys.modulus,tamKeys[2],MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Bcast(cc,tamBase,MPI_CHAR,0,MPI_COMM_WORLD);

	/*
	if(my_rank == 0){
		printf("TamBase->%d\n", tamBase);
		printf("Modulus->%s\n", keys.modulus);
		printf("public->%s\n", keys.publicKey);
		printf("private->%s\n", keys.privateKey);	
	}
	*/

//	clock_t begin = clock();

	timeSpend = cifrar(cc,tamBase,keys,numThreads,&res_c,my_rank,mpi_size);

//	clock_t end = clock();

//	double timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	

	MPI_Reduce(&timeSpend, &timeMax, 1, MPI_DOUBLE, MPI_MAX,0,MPI_COMM_WORLD);
	if(my_rank == 0) printf("%lf ",timeMax);

	int n = tamBase / mpi_size;
	int local_n = n;
	if(my_rank == mpi_size - 1){
		local_n += tamBase % mpi_size;
	}


	char ** cypher = (char **) malloc(sizeof(void *) * tamBase);
	for(int i = 0; i < tamBase; i++){
		cypher[i] = NULL;
	}
	allgather(&cypher,res_c,tamBase,n,local_n,mpi_size,my_rank);

/*
	if(my_rank == 0){
		printf("Cypher->\n");
		for(int i = 0; i < tamBase; i++){
			printf("%s\n", cypher[i]);
		}
		printf("\n");
	}
*/

//	begin = clock();

	timeSpend = descifrar(cypher,tamBase,keys,numThreads,&res_d, my_rank, mpi_size);

//	end = clock();

//	timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	MPI_Reduce(&timeSpend, &timeMax, 1, MPI_DOUBLE, MPI_MAX,0,MPI_COMM_WORLD);
	if(my_rank 	== 0) printf("%lf ",timeMax);

	char * descypher = (char *) malloc(tamBase);

	int * count = (int *) malloc(sizeof(int) * mpi_size);
	int * displ  = (int *) malloc(sizeof(int) * mpi_size);
	for(int i = 0; i < mpi_size; i++){
		if(i == mpi_size - 1) count[i] = n + (tamBase % mpi_size);
		else count[i] = n;
		displ[i] = i * n;
	}

	MPI_Allgatherv(res_d,local_n,MPI_CHAR,descypher,count,displ,MPI_CHAR,MPI_COMM_WORLD);

	descypher = (char *) realloc(descypher,tamBase + 1);
	descypher[tamBase] = '\0';


	if(my_rank == 0){
		//printf("%s\n", descypher);
		printf("%lf\n",tk);
	}

	MPI_Finalize();
}




