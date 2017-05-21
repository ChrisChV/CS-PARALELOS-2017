#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM 30;

int * getRandomVector(int size){
	srand(time(NULL));
	int * res = (int *) malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		res[i] = rand() % MAX_NUM + 1;
	}
	return res;
}

int verifySort(int * vector, int size){
	int actual = -1;
	for(int i = 0; i < size; i++){
		if(actual > vector[i]) return 0;
	}
	return 1;
}

int main(){
	srand(time(NULL));
	int size = 10;
	int * vector = NULL;
	int temp = 0;
	int iteraciones = rand() % 20;
	printf("Iter->%d\n", iteraciones);
	int count = 0;
	for(int j = 0; j < iteraciones; j++){
		vector = getRandomVector(size);
		for(int i = 0; i < size; i++){
			if(i % 2 == 0){
				for(int j = 0; j < size - 1; j+=2){
					if(vector[j] > vector[j+1]){
						temp = vector[j];
						vector[j] = vector[j+1];
						vector[j+1] = temp;
					}
				}
			}
			else{
				for(int j = 1; j < size - 1; j+=2){
					if(vector[j] > vector[j+1]){
						temp = vector[j];
						vector[j] = vector[j+1];
						vector[j+1] = temp;
					}
				}
			}
			if(i != size - 1){
				if(verifySort(vector,size) == 1){
					count++;
					break;
				}
			}
		}

		free(vector);
	}
	printf("Count->%d\n",count);
}