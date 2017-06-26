#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 16
#define BLUR_SIZE 3

/*
__global__
void blur(int * img, int * res, int altura, int ancho){
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	if(col < ancho && row < altura){
		int pixVal = 0;
		int pixeles = 0;
		for(int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE+1; ++blurRow){
			for(int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE+1; ++blurCol){
				int curRow = row + blurRow;
				int curCol = col + blurCol;
			
				if(curRow > -1 && curRow < altura && curCol > -1 && curCol < ancho){
					pixVal += img[curRow * ancho + curCol];
					pixeles++;
				}
			}
		}
		res[row * ancho + col] = (unsigned char)(pixVal / pixeles);
	}
	
}

void hostblur(int ** img, int ** res, int altura, int ancho){
	int size = sizeof(int) * altura * ancho;
	int * d_img;
	int * d_res;
	for(int i = 0; i < 3; i++){
		cudaMalloc((void**) &d_img, size);
		cudaMalloc((void**) &d_res, size);
		cudaMemcpy(d_img, img[i], size, cudaMemcpyHostToDevice);

		dim3 dimGrid(ceil(ancho/NUM_THREADS),ceil(altura/NUM_THREADS),1);
		dim3 dimBlock(NUM_THREADS,NUM_THREADS,1);

		blur<<<dimGrid,dimBlock>>>(d_img,d_res,altura,ancho);

		cudaMemcpy(res[i], d_res, size, cudaMemcpyDeviceToHost);
		cudaFree(d_img);
		cudaFree(d_res);
	}
}

*/

int ** loadImage(char * file, int * altura, int * ancho){
	FILE * archivo = fopen(file,"r");
	char * line;
	size_t len = 0;
	getline(&line,&len,archivo);
	*altura = atoi(line);
	getline(&line,&len,archivo);
	*ancho = atoi(line);
	int ** res = (int **) malloc(sizeof(void *) * 3);
	for(int i = 0; i < 3; i++){
		res[i] = (int *) malloc(sizeof(int) * (*ancho) * (*altura));
		for(int j = 0; j < *altura; j++){
			for(int k = 0; k < *ancho; k++){
				getline(&line,&len,archivo);
				res[i][j * (*ancho) + k] = atoi(line);
			}
		}
	}
	fclose(archivo);
	return res;
}

void saveImage(int ** res, char * file, int altura, int ancho){
	FILE * archivo = fopen(file,"w");
	fprintf(archivo, "%d\n", altura);
	fprintf(archivo, "%d\n", ancho);
	for(int k = 0; k < 3; k++){
		for(int i = 0; i < altura; i++){
			for(int j = 0; j < ancho; j++){
				fprintf(archivo, "%d\n", res[k][i * ancho + j]);
			}	
		}
	}
	fclose(archivo);
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
	int ** img = loadImage("temp",&altura, &ancho);
	int ** res = (int **) malloc(sizeof(void *) * 3);
	for(int i = 0; i < 3; i++){
		res[i] = (int *) malloc(sizeof(int) * ancho) * altura));
	}
	system("rm temp");
	hostblur(img,res,altura,ancho);
	saveImage(res,"res.txt",altura,ancho);
	system("./img res.txt 2");
	system("convert res.txt res.jpg");
	system("rm res.txt");
}