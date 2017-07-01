#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <pthread.h>
#include "tt.h"

void printMatrix(int ** matrix, int rows, int columns){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void llenarMatriz(int *** matrix, int rows, int columns, int init){
	*matrix = (int **) malloc(sizeof(void*) * rows);
	for(int i = 0; i < rows; i++){
		(*matrix)[i] = (int *) malloc(sizeof(int) * columns);
		for(int j = 0; j < columns; j++){
			(*matrix)[i][j] = init;
		}
	}
}

void * fun(void * arg){
	printMatrix(matrix,5,5);
}

/*
int main(){
	int my_rank;
	int mpi_size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
	char ** a = (char **) malloc(sizeof(void *) * 2);
	a[0] = (char *) malloc(2);
	a[1] = (char *) malloc(2);
	a[0] = "aasa";
	a[1] = "bdasdasd";
	char ** b = (char **) malloc(sizeof(void *) * 4);
	int * count = (int *) malloc(sizeof(int) * 2);
	int * displ = (int *) malloc(sizeof(int) * 2);
	count[0] = sizeof(char *) * 2;
	count[1] = sizeof(char *) * 2;
	displ[0] = 0;
	displ[1] = sizeof(char *) * 2 ;

	//b[0] = (char *) malloc(2);
	//b[1] = (char *) malloc(2);
	//MPI_Allgather(&(a[0]),1,MPI_CHAR,&(b[0]),1,MPI_CHAR,MPI_COMM_WORLD);
	//MPI_Allgather(&(a[1]),1,MPI_CHAR,&(b[1]),1,MPI_CHAR,MPI_COMM_WORLD);

	
	MPI_Allgatherv(a,sizeof(char *) * 2,MPI_CHAR,b,count,displ,MPI_CHAR,MPI_COMM_WORLD);		
	
	if(my_rank == 1){
		printf("%s\n", b[0]);
		printf("%s\n", b[1]);
		printf("%s\n", b[2]);
		printf("%s\n", b[3]);	
	}
	
}
*/

int main(){
	int my_rank;
	int mpi_size;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&mpi_size);
	char ** a = (char **) malloc(sizeof(void *) * 2);
	a[0] = (char *) malloc(6);
	a[1] = (char *) malloc(6);
	for(int i = 0; i < 5; i++){
		a[0][i] = 'a';
		a[1][i] = 'b';
	}
	a[0][5] = '\0';
	a[0][5] = '\0';
	char ** b = (char **) malloc(sizeof(void *) * 4);
	//for(int i = 0; i < 4; i++){
	//	b[i] = (char *) malloc(10);
	//}
	int * count = (int *) malloc(sizeof(int) * 2);
	int * displ = (int *) malloc(sizeof(int) * 2);
	count[0] = sizeof(char *) * 2;
	count[1] = sizeof(char *) * 2;
	displ[0] = 0;
	displ[1] = sizeof(char *) * 2 ;
	//b[0] = (char *) malloc(2);
	//b[1] = (char *) malloc(2);
	//MPI_Allgather(&(a[0]),1,MPI_CHAR,&(b[0]),1,MPI_CHAR,MPI_COMM_WORLD);
	//MPI_Allgather(&(a[1]),1,MPI_CHAR,&(b[1]),1,MPI_CHAR,MPI_COMM_WORLD);

	//MPI_Allgather(&a[0],strlen(a[0]),MPI_CHAR,&b[0],strlen(a[0]),)
	
	MPI_Allgatherv(a,sizeof(char *) * 2,MPI_CHAR,b,count,displ,MPI_CHAR,MPI_COMM_WORLD);		
	//if(my_rank == 0){
	//	b[0] = a[0];	
	//}
	//MPI_Bcast(b[0],5,MPI_CHAR,0,MPI_COMM_WORLD);
	//if(my_rank == 1) printf("%s\n", b[0]);



	if(my_rank == 1){
		printf("%s\n", b[0]);
		printf("%s\n", b[1]);
		//printf("%s\n", b[2]);
		//printf("%s\n", b[3]);	
	}
}

/*
int main(){
	int my_rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	char * a = (char *) malloc(2);
	a[0] = 'a';
	a[1] = 'b';
	char * b = (char *) malloc(2);
	int * count = (int *) malloc(sizeof(int) * 2);
	int * displ = (int *) malloc(sizeof(int) * 2);
	count[0] = 2;
	count[1] = 2;
	displ[0] = 0;
	displ[1] = 0;

	//b[0] = (char *) malloc(2);
	//b[1] = (char *) malloc(2);
	//MPI_Allgather(&(a[0]),1,MPI_CHAR,&(b[0]),1,MPI_CHAR,MPI_COMM_WORLD);
	//MPI_Allgather(&(a[1]),1,MPI_CHAR,&(b[1]),1,MPI_CHAR,MPI_COMM_WORLD);


	
	MPI_Allgather(a,2,MPI_CHAR,b,2,MPI_CHAR,MPI_COMM_WORLD);		
	
	if(my_rank == 0){
		printf("%c\n", b[0]);
		printf("%c\n", b[1]);
		printf("%c\n", b[2]);
		printf("%c\n", b[3]);
	}
}
*/