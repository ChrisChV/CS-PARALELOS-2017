#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>
#include <sys/time.h>

#define TILE_WIDTH 32


__global__ void matrixMulKernel(float* A, float* B, float* C, int n) {
	__shared__ float Mds[TILE_WIDTH][TILE_WIDTH];
	__shared__ float Nds[TILE_WIDTH][TILE_WIDTH];
	int bx = blockIdx.x;
	int by = blockIdx.y;
	int tx = threadIdx.x;
	int ty = threadIdx.y;

	int Row = by * TILE_WIDTH + ty;
	int Col = bx * TILE_WIDTH + tx;

	float Pvalue = 0;
	for (int ph = 0; ph < n/TILE_WIDTH; ++ph) {
		Mds[ty][tx] = A[Row*n + ph*TILE_WIDTH + tx];
		Nds[ty][tx] = B[(ph*TILE_WIDTH + ty)*n + Col];
		__syncthreads();
		for (int k = 0; k < TILE_WIDTH; ++k) {
			Pvalue += Mds[ty][k] * Nds[k][tx];
		}
		__syncthreads();
	}
	C[Row*n + Col] = Pvalue;
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
	dim3 dimGrid(ceil(n/TILE_WIDTH),ceil(n/TILE_WIDTH),1);
	dim3 dimBlock(TILE_WIDTH,TILE_WIDTH,1);

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
	cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, 0);
	printf("Mem->%d\n", prop.sharedMemPerBlock);
	printf("Threads->%d\n", prop.maxThreadsPerBlock);
	printf("ThreadsD1->%d\n", prop.maxThreadsDim[0]);
	printf("ThreadsD2->%d\n", prop.maxThreadsDim[1]);
	printf("GridD1->%d\n", prop.maxGridSize[0]);
	printf("GridD2->%d\n", prop.maxGridSize[1]);
	int n = atoi(argc[1]);
	float * A = (float *) malloc(sizeof(float) * n * n);
	for(int i = 0; i < n * n; i++) A[i] = 2;
	float * B = (float *) malloc(sizeof(float) * n * n);
	for(int i = 0; i < n * n; i++) B[i] = 2;
	float * C = (float *) malloc(sizeof(float) * n * n);
	hostMatrixMul(A,B,C,n);
//	printMatrix(C,n);
}