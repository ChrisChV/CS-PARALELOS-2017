#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>
#include <sys/time.h>

__global__ void matrixMulKernel(float* A, float* B, float* C, int n) {
	int Row = blockIdx.y*blockDim.y+threadIdx.y;
	int Col = blockIdx.x*blockDim.x+threadIdx.x;
	if ((Row < n) && (Col < n)) {
		float Pvalue = 0;
		for (int k = 0; k < n; ++k) {
			Pvalue += A[Row*n+k]*B[k*n+Col];
		}
		C[Row*n+Col] = Pvalue;
	}
}

void printMatrix(float * M, int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%f ", M[n * i + j]);
		}
		printf("\n");
	}
	printf("\n");
}


void hostMatrixMul(float * A, float * B, float * C, int n){
	int size = sizeof(float) * n * n;
	float * d_A;
	float * d_B;
	float * d_C;
	struct timeval t1, t2;
	cudaMalloc((void **) &d_A, size);
	cudaMalloc((void **) &d_B, size);
	cudaMalloc((void **) &d_C, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	dim3 dimGrid(ceil(n/16.0),ceil(n/16.0),1);
	dim3 dimBlock(16,16,1);
	gettimeofday(&t1, 0);
	matrixMulKernel<<<dimGrid,dimBlock>>>(d_A,d_B,d_C,n);
	cudaThreadSynchronize();
	gettimeofday(&t2, 0);
	double time = (1000000.0*(t2.tv_sec-t1.tv_sec) + t2.tv_usec-t1.tv_usec)/1000.0;
	printf("Time:  %f ms \n", time);
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	cudaFree(d_A);
}


int main(int argv, char ** argc){
	if(argv != 2){
		printf("Faltan Argumentos <tamMatrix>\n");
		return 0;
	}
	int n = atoi(argc[1]);
	float * A = (float *) malloc(sizeof(float) * n * n);
	for(int i = 0; i < n * n; i++) A[i] = 2;
	float * B = (float *) malloc(sizeof(float) * n * n);
	for(int i = 0; i < n * n; i++) B[i] = 2;
	float * C = (float *) malloc(sizeof(float) * n * n);
	hostMatrixMul(A,B,C,n);
//	printMatrix(C,n);
}