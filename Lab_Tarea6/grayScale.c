#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 64

/*
__global__
void grayScale(int * img, int * res, int altura, int ancho){
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	if(col < ancho && row < altura){
		int r = img[altura * ancho * 0 + row * ancho + col];
		int g = img[altura * ancho * 1 + row * ancho + col];
		int b = img[altura * ancho * 2 + row * ancho + col];
		int gray = 0.21 * r + 0.72 * g + 0.07 * b;
		res[row * ancho + col] = gray;
	}
}

void hostGrayScale(int * img, int * res, int altura, int ancho){
	int size = sizeof(int) * altura * ancho;
	int * d_img;
	int * d_res;
	cudaMalloc((void**) &d_img, size * 3);
	cudaMalloc((void**) &d_res, size);
	cudaMemcpy(d_img, img, size * 3, cudaMemcpyHostToDevice);

	dim3 dimGrid(ceil(ancho/NUM_THREADS),ceil(altura/NUM_THREADS),1);
	dim3 dimBlock(NUM_THREADS,NUM_THREADS,1);

	grayScale<<<dimGrid,dimBlock>>>(d_img,d_res,altura,ancho);

	cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
	cudaFree(d_img);
	cudaFree(d_res);
}

int * loadImage(char * file, int * altura, int * ancho){
	FILE * archivo = fopen(file,"r");
	char * line;
	size_t len = 0;
	getline(&line,&len,archivo);
	*altura = atoi(line);
	getline(&line,&len,archivo);
	*ancho = atoi(line);
	int * res = (int *) malloc(sizeof(int) * (*ancho) * (*altura) * 3);
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < *altura; j++){
			for(int k = 0; k < *ancho; k++){
				getline(&line,&len,archivo);
				res[(*altura) * (*ancho) * i + j * (*ancho) + k] = atoi(line);
			}
		}
	}
	fclose(archivo);
	return res;
}
*/
void saveImage(int * res, char * file, int altura, int ancho){
	FILE * archivo = fopen(file,"w");
	fprintf(archivo, "%d\n", altura);
	fprintf(archivo, "%d\n", ancho);
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < ancho; j++){
			fprintf(archivo, "%d\n", res[i * ancho + j]);
		}
	}
	fclose(archivo);
}

void print(int * res, int altura, int ancho){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < altura; j++){
			for(int k = 0; k < ancho; k++){
				printf("%d\n", res[altura * ancho * i + j * ancho + k]);
			}
		}
	}
}


int main(int argv, char ** argc){
	if(argv != 2){
		printf("Faltan Argumentos <img>\n");
		return 0;
	}
	char * fileImg = argc[1];
    char * programImg = "./img ";
	char * temp = " 0 >> temp"; 
	char * comp = (char *) malloc(strlen(programImg) + strlen(fileImg) + strlen(temp));
	comp = strcpy(comp, programImg);
	comp = strcat(comp, fileImg);
	comp = strcat(comp, temp);
	printf("%s\n", comp);
	system(comp);
	int altura = 0;
	int ancho = 0;
	int * img = loadImage("temp",&altura, &ancho);
	int * res = (int *) malloc(sizeof(int) * altura * ancho);
	system("rm temp");
	hostGrayScale(img,res,altura,ancho);
	saveImage(res,"res",altura,ancho);
	system("./img res 1");
	system("rm res");
}