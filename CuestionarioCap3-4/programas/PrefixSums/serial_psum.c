#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"

int main(){
	int n = 10;
	int * vec = getRandomVector(n);
	int * res = malloc(sizeof(int) * n);
	int sum = 0;
	printVector(vec,n);
	for(int i = 0; i < n; i++){
		sum = 0;
		for(int j = 0; j <= i; j++){
			sum += vec[j];
		}
		res[i] = sum;
	}
	printVector(res,n);
}