#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

Matrix matrixProduct(Matrix A, Matrix B){
	Matrix m_res;
	m_res.matrix = NULL;
	if(A.col != B.fil) return m_res;
	M_num ** res;
	res = (M_num **) malloc(sizeof(M_num*) * A.fil);
	for(int i = 0; i < A.fil; i++){
		res[i] = (M_num *) malloc(sizeof(M_num) * B.col);
	}
	for(int i = 0; i < A.fil; i++){
		for(int j = 0; j < B.col; j++){
			res[i][j] = 0;
		}
	}
	clock_t begin = clock();
	for(int i = 0; i < A.fil; i++){
		for(int j = 0; j < B.col; j++){
			for(int k = 0; k < A.col; k++){
				res[i][j] += A.matrix[i][k] * B.matrix[k][j];
			}
		}
	}
	clock_t end = clock();
	double timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Tiempo->%lf\n", timeSpend);
	m_res.matrix = res;
	m_res.fil = A.fil;
	m_res.col = B.col;
	return m_res;
}

int main(){
	Matrix A = generateRandomMatrix(1000,1000);
	Matrix B = generateRandomMatrix(1000,1000);
	//printMatrix(A);
	//printMatrix(B);
	Matrix C = matrixProduct(A,B);
	printf("Done\n");
	//printMatrix(C);
}


