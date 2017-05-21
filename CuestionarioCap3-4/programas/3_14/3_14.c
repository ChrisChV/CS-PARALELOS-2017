#include <stdio.h>
#include <stdlib.h>

void print_two_d(int  two_d[][4], int rows, int cols){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			printf("%d ", two_d[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


int main(){
	int two_d[3][4];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 4; j++){
			two_d[i][j] = j;
		}
	}
	print_two_d(two_d, 3 , 4);
}