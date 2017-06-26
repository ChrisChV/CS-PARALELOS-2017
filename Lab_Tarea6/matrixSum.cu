#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>

__global__
void matrixSum(float * A, float * B, float * C, int n){
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	if(col < n && row < n){
		C[row * n + col] = A[row * n + col] + B[row * n + col];
	}
}


void hostMatrixSum(float * A, float * B, float * C, int n){
	int size = sizeof(float) * n * n;
	float * d_A;
	float * d_B;
	float * d_C;
	cudaMalloc((void **) &d_A, size);
	cudaMalloc((void **) &d_B, size);
	cudaMalloc((void **) &d_C, size);
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	dim3 dimGrid(ceil(n/16.0),ceil(n/16.0),1);
	dim3 dimBlock(16,16,1);
	matrixSum<<<dimGrid,dimBlock>>>(d_A,d_B,d_C,n);
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
	for(int i = 0; i < n * n; i++) A[i] = 1;
	float * B = (float *) malloc(sizeof(float) * n * n);
	for(int i = 0; i < n * n; i++) B[i] = 1;
	float * C = (float *) malloc(sizeof(float) * n * n);
	hostMatrixSum(A,B,C,n);
	//for(int i = 0; i < n * n; i++) printf("%f ", C[i]);
}
