#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RANDOM_RANGE 100;

typedef int M_num;

typedef struct{
	M_num ** matrix;
	int fil;
	int col;
} Matrix;

void printMatrix(Matrix M){
	for(int i = 0; i < M.fil; i++){
		for(int j = 0; j < M.col; j++){
			printf("%d ",M.matrix[i][j]);
		}
		printf("\n");
	}
}

Matrix generateRandomMatrix(int fil, int col){
	srand(time(NULL));
	Matrix m_res;
	M_num ** res;
	res = (M_num **) malloc(sizeof(M_num *) * fil);
	for(int i = 0; i < fil; i++){
		res[i] = (M_num *) malloc(sizeof(M_num) * col);
	}
	for(int i = 0; i < fil; i++){
		for(int j = 0; j < col; j++){
			res[i][j] = rand() % RANDOM_RANGE;
		}
	}
	m_res.matrix = res;
	m_res.fil = fil;
	m_res.col = col;
	return m_res;
}

int min(int A, int B){
	if(A < B) return A;
	else return B;
}