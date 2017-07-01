#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RSA.h"

//Time encriptacion
//Time desencriptacion
//Time keyGeneration


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
		count[i] = local_n;
		displ[i] = i * n;
	}
	MPI_Allgatherv(local_size,local_n,MPI_INT,sizes,count,displ,MPI_INT,MPI_COMM_WORLD);

	int i_init = my_rank * n;
	int i_end = i_init + local_n;
	int bcast_rank = 0;
	for(int i = 0; i < tamBase; i++){

		(*cypher)[i] = (char *) malloc(sizes[i]);

		bcast_rank = i / n;
		if(bcast_rank == my_rank){
			(*cypher)[i] = res[i- i_init];
			(*cypher)[i][sizes[i] - 1] = '\0';
		}
		
		MPI_Bcast((*cypher)[i],sizes[i],MPI_CHAR,bcast_rank,MPI_COMM_WORLD);


	}
}


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
	int tamBase = 0;
	int numThreads = atoi(argv[1]);
	int tamNumber = atoi(argv[2]);
	char * file = argv[3];
	char * cc = getFile(file,&tamBase);
	char ** res = NULL;
	Keys keys = keyGeneration(tamNumber);

	cifrar(cc,tamBase,keys,numThreads,&res,my_rank,mpi_size);


	int n = tamBase / mpi_size;
	int local_n = n;
	if(my_rank == mpi_size - 1){
		local_n += tamBase % mpi_size;
	}
	
	int * count = (int *) malloc(sizeof(int) * mpi_size);
	int * displ  = (int *) malloc(sizeof(int) * mpi_size);
	for(int i = 0; i < mpi_size; i++){
		count[i] = sizeof(char *) * local_n;
		displ[i] = i * sizeof(char *) * n;
	}

	char ** cypher = (char **) malloc(sizeof(void *) * tamBase);
	for(int i = 0; i < tamBase; i++){
		cypher[i] = NULL;
	}
	for(int i = 0; i < local_n; i++){
		printf("%s\n", res[i]);
	}
	printf("\n");
	allgather(&cypher,res,tamBase,n,local_n,mpi_size,my_rank);
	if(my_rank == 0){
		for(int i = 0; i < tamBase; i++){
			printf("%s\n", cypher[i]);
		}
	}






	/*	
	if(my_rank == 0){
		for(int i = 0; i < local_n; i++){
			printf("%s\n", res[i]);
		}
		printf("\n");
	}
	*/

	/*
	char ** temp = (char **) malloc(sizeof(void *) * 1);

	
	temp[0] = (char *) malloc(strlen(res[0]) + 1);
	for(int i = 0; i < strlen(res[0]); i++){
		temp[0][i] = res[0][i];
	}
	temp[0][strlen(res[0])] = '\0';
	
	temp[0] = (char *) malloc(2);
	char c = 'a';
	char nu = '\0';
	temp[0][0] = c;
	temp[0][1] = nu;
	

	//printf("%s\n", temp[0]);
	char ** temp2 = (char **) malloc(sizeof(void *) * 1);

	
	if(my_rank == 1){
		MPI_Send(temp,sizeof(char *),MPI_CHAR,0,0,MPI_COMM_WORLD);
	}
	else{
		MPI_Recv(temp2,sizeof(char *), MPI_CHAR,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	//TODO Esta pendejada no funciona. Copia por separado.
	//MPI_Allgather(res,sizeof(char *) * local_n, MPI_CHAR, cypher, sizeof(char *) * local_n, MPI_CHAR, MPI_COMM_WORLD);

//	if(my_rank == 1) printf("%s\n", cypher[5]);

	//MPI_Barrier(MPI_COMM_WORLD);

	if(my_rank == 0){
		//if(cypher[5] == NULL) printf("dsadasdsa\n");
		//cypher[5][0] = 's';
		//for(int i = 0; i < tamBase; i++){
		//	printf("%s\n", cypher[i]);
		//}
		//for(int i = 0; i < local_n; i++){
		//	printf("%s\n", temp[i]);
		//}
		printf("%s\n", temp2[0]);
	}
	*/
	
}

/*
int main(int argc, char ** argv){
	if(argc != 4){
		printf("Faltan argumentos <numThreads> <tamNumber> <file>\n");
		return 0;
	}
	int numThreads = atoi(argv[1]);
	int tamNumber = atoi(argv[2]);
	char * file = argv[3];
	char * cc = getFile(file);
	clock_t keyGini = clock();
	Keys keys = keyGeneration(tamNumber);
	clock_t keyGend = clock();
	int tamCypher = 0;
	//clock_t cifrarIni = clock();
	char ** cypher = cifrar(cc,keys,numThreads,&tamCypher);
	//clock_t cifrarEnd = clock();
	//printf("ENCRIPTADO\n");
	//clock_t descifrarIni = clock();
	char * res = descifrar(cypher, keys, numThreads, tamCypher);
	//clock_t descifrarEnd = clock();
	//printf("%s\n",res);
	double key = (double) (keyGend - keyGini) / CLOCKS_PER_SEC;
	//double ci = (double) (cifrarEnd - cifrarIni) / CLOCKS_PER_SEC;
	//ouble deci = (double) (descifrarEnd - descifrarIni) / CLOCKS_PER_SEC;
	printf("%lf\n", key);
	//printf("C->%lf\n", ci);
	//printf("D->%lf\n", deci);
}
*/

/*
int main(){
	mpz_t test;
	mpz_t divi;
	mpz_t mod;
	mpz_init(test);
	mpz_init(divi);
	mpz_init(mod);
	mpz_set_ui(divi,1000000000);
	mpz_set_str(test,"28371283712833435",10);
	mpz_set_str(mod,"21212132",10);
	int size = mpz_sizeinbase(test,2);
	char * number = (char *) malloc(size);
	mpz_get_str(number,2,test);
	printf("%d\n", size);
	printf("%s\n", number);
	int k = 1;
	int h = 0;
	int resto = 0;
	char ** res = divideNumber(number,size,k,&h,&resto);
	for(int i = 0; i < k; i++){
		printf("%s\n", res[i]);
	}
	mpz_set_str(mod,number,2);
	mpz_out_str(stdout,2,mod);
	printf("\n");

}
*/




