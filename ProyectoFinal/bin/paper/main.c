#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"

//Time encriptacion
//Time desencriptacion
//Time keyGeneration


void printCypher(char ** cypher, int tamC){
	int tamL = 0;
	for(int i = 0; i < tamC; i++){
		printf("%s\n", cypher[i]);
	}
	printf("\n");
}

char * getFile(char * file){
	FILE * archivo = fopen(file,"r");
	char temp = fgetc(archivo);
	char * cc;
	int count = 1;
	while(temp != EOF){
		if(count == 1) cc = (char *) malloc(count);
		else cc = (char *) realloc(cc,count);
		cc[count - 1] = temp;
		count++;
		temp = fgetc(archivo);
	}
	fclose(archivo);
	count--;
	return cc;
}

void pp(){
	int my_rank = omp_get_thread_num();
	printf("Soy->%d\n", my_rank);
}



int main(int argc, char ** argv){
	if(argc != 4){
		printf("Faltan argumentos <numThreads> <tamNumber> <file>\n");
		return 0;
	}
	int numThreads = atoi(argv[1]);
	int tamNumber = atoi(argv[2]);
	char * file = argv[3];
	char * cc = getFile(file);
	clock_t keyGini = clock();
	Keys keys = keyGeneration(tamNumber);
	clock_t keyGend = clock();
	int tamCypher = 0;
	//clock_t cifrarIni = clock();
	char ** cypher = cifrar(cc,keys,numThreads,&tamCypher);
	//clock_t cifrarEnd = clock();
	//printf("ENCRIPTADO\n");
	//clock_t descifrarIni = clock();
	char * res = descifrar(cypher, keys, numThreads, tamCypher);
	//clock_t descifrarEnd = clock();
	printf("%s\n",res);
	double key = (double) (keyGend - keyGini) / CLOCKS_PER_SEC;
	//double ci = (double) (cifrarEnd - cifrarIni) / CLOCKS_PER_SEC;
	//ouble deci = (double) (descifrarEnd - descifrarIni) / CLOCKS_PER_SEC;
	printf("%lf\n", key);
	//printf("C->%lf\n", ci);
	//printf("D->%lf\n", deci);
}


/*
int main(){
	mpz_t test;
	mpz_t divi;
	mpz_t mod;
	mpz_init(test);
	mpz_init(divi);
	mpz_init(mod);
	mpz_set_ui(divi,1000000000);
	mpz_set_str(test,"28371283712833435",10);
	mpz_set_str(mod,"21212132",10);
	int size = mpz_sizeinbase(test,2);
	char * number = (char *) malloc(size);
	mpz_get_str(number,2,test);
	printf("%d\n", size);
	printf("%s\n", number);
	int k = 1;
	int h = 0;
	int resto = 0;
	char ** res = divideNumber(number,size,k,&h,&resto);
	for(int i = 0; i < k; i++){
		printf("%s\n", res[i]);
	}
	mpz_set_str(mod,number,2);
	mpz_out_str(stdout,2,mod);
	printf("\n");

}
*/


