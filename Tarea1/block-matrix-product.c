#include <stdlib.h>
#include "utils.h"

#define TILE_SIZE 32

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
	M_num sum = 0;
	clock_t begin = clock();
	for(int i = 0; i < A.fil; i += TILE_SIZE){
		for(int j = 0; j < B.col; j += TILE_SIZE){
			for(int k = 0; k < A.col; k += TILE_SIZE){
				for(int ii = i; ii < min(i + TILE_SIZE, A.fil); ii++){
					for(int jj = j; jj < min(j + TILE_SIZE, B.col); jj++){
						sum = 0;
						for(int kk = k; kk < min(k + TILE_SIZE, A.col); kk++){
							sum += A.matrix[ii][kk] * B.matrix[kk][jj];
						}
						res[ii][jj] += sum; 
					}
				}
			}
		}
	}
	clock_t end = clock();
	double timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time->%lf\n",timeSpend);
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