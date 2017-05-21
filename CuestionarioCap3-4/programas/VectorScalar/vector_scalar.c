#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define BUFF_SIZE 10

int multiDot(int * vec1, int * vec2, int size){
	int res = 0;
	for(int i = 0; i < size; i++){
		res += vec1[i] * vec2[i];
	}
	return res;
}

int * multiScalar(int * vec, int scalar, int size){
	int * res = (int*) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		res[i] = vec[i] * scalar;
	}
	return res;
}

void printVec(int * vec, int size){
	for(int i = 0; i < size; i++){
		printf("%d ", vec[i]);
	}
	printf("\n");
}

int main(){
	int my_rank;
	int comm_sz;
	int n = 4;
	int * vec1 = NULL;
	int * vec2 = NULL;
	int * local_vec1;
	int * local_vec2;
	int * res_scalar1;
	int * res_scalar2;
	int res_dot;
	int scalar;
	int local_n;
	int total_res_dot;
	int * total_res_scalar1;
	int * total_res_scalar2;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	if(my_rank == 0){
		char buff[BUFF_SIZE];
		printf("Tamaño de n->%d\n", n);
		vec1 = (int*) malloc(sizeof(int) * n);
		vec2 = (int*) malloc(sizeof(int) * n);
		int temp = 0;
		printf("Ingrese el vector 1->");
		for(int i = 0; i < n; i++){
			fgets(buff,BUFF_SIZE,stdin);
			temp = atoi(buff);
			vec1[i] = temp;
		}
		printf("Ingrese el vector 2->");
		for(int i = 0; i < n; i++){
			fgets(buff,BUFF_SIZE,stdin);
			temp = atoi(buff);
			vec2[i] = temp;
		}
		printf("Ingrese el scalar->");
		fgets(buff,BUFF_SIZE,stdin);
		scalar = atoi(buff);
	}
	local_n = n / comm_sz;
	local_vec1 = (int*) malloc(sizeof(int) * local_n);
	local_vec2 = (int*) malloc(sizeof(int) * local_n);
	MPI_Scatter(vec1,local_n,MPI_INT,local_vec1,local_n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(vec2,local_n,MPI_INT,local_vec2,local_n,MPI_INT,0,MPI_COMM_WORLD);
	if(my_rank == 0) {
		for(int i = 1; i < comm_sz; i++){
			MPI_Send(&scalar,1,MPI_INT,i,0,MPI_COMM_WORLD);
		}
	}
	else MPI_Recv(&scalar,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	res_scalar1 = multiScalar(local_vec1,scalar,local_n);
	res_scalar2 = multiScalar(local_vec2,scalar,local_n);
	res_dot = multiDot(local_vec1,local_vec2,local_n);
	MPI_Reduce(&res_dot,&total_res_dot,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	total_res_scalar1 = (int*) malloc(sizeof(int) * n);
	total_res_scalar2 = (int*) malloc(sizeof(int) * n);
	MPI_Gather(res_scalar1,local_n,MPI_INT,total_res_scalar1,local_n,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Gather(res_scalar2,local_n,MPI_INT,total_res_scalar2,local_n,MPI_INT,0,MPI_COMM_WORLD);
	if(my_rank == 0){
		printf("Resultado del producto punto->%d\n",total_res_dot);
		printf("Vector1->");
		printVec(total_res_scalar1,n);
		printf("Vector2->");
		printVec(total_res_scalar2,n);
	}
	MPI_Finalize();
	return 0;

}