#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include "Utils.h"
#include <omp.h>

#define CIFRAR 0
#define DESCIFRAR 1

omp_lock_t mutex;

double secTotal = 0;
double menor = -1;

Keys keyGeneration(int size){
	srand(time(NULL));
	Keys res;
	mpz_t m;
	mpz_t p;
	mpz_t q;
	mpz_t tempP;
	mpz_t tempQ;
	mpz_t tempD;
	mpz_t n;
	mpz_t e;
	mpz_t d;
	mpz_init(m);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(tempP);
	mpz_init(tempQ);
	mpz_init(tempD);
	mpz_init(e);
	mpz_init(d);
	mpz_set_ui(tempP,1);
	mpz_set_ui(tempQ,1);
	mpz_set_ui(tempD,1);
	mpz_set_str(p,getRandomPrime(size,rand() % RANDOM_SEED_RANGE),10);
	mpz_set_str(q,getRandomPrime(size,rand() % RANDOM_SEED_RANGE),10);

	//printf("%d\np->",(int) mpz_sizeinbase(p,2));
	//mpz_out_str(stdout,10,p);
	//printf("\n");

	//printf("%d\nq->",(int) mpz_sizeinbase(q,2));
	//mpz_out_str(stdout,10,q);
	//printf("\n");

	mpz_mul(m,p,q);

	//printf("%d\nm->",(int) mpz_sizeinbase(m,2));
	//mpz_out_str(stdout,10,m);
	//printf("\n");


	mpz_sub(tempP,p,tempP);
	mpz_sub(tempQ,q,tempQ);
	mpz_mul(n,tempP,tempQ);

	//printf("%d\nn->",(int) mpz_sizeinbase(n,2));
	//mpz_out_str(stdout,10,n);
	//printf("\n");

	mpz_set_str(e,getCoprime(n),10);

	//printf("%d\ne->",(int) mpz_sizeinbase(e,2));
	//mpz_out_str(stdout,10,e);
	//printf("\n");	

	mpz_invert(d,e,n);

	//printf("%d\nd->",(int) mpz_sizeinbase(d,2));
	//mpz_out_str(stdout,10,d);
	//printf("\n");	

	res.publicKey = (char *) malloc(mpz_sizeinbase(e,10));
	res.privateKey = (char *) malloc(mpz_sizeinbase(d,10));
	res.modulus = (char *) malloc(mpz_sizeinbase(m,10));

	mpz_get_str(res.publicKey,10,e);
	mpz_get_str(res.privateKey,10,d);
	mpz_get_str(res.modulus,10,m);

	mpz_clear(m);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(tempP);
	mpz_clear(tempQ);
	mpz_clear(tempD);
	mpz_clear(n);
	mpz_clear(e);
//	mpz_clear(d);

	return res;
}

/*void parallelMul(mpz_t base, mpz_t n, mpz_t * result){
	mpz_t res;
	mpz_t one;
	mpz_t i;
	mpz_init(res);
	mpz_init(one);
	mpz_init(i);
	mpz_set_ui(res,1);
	mpz_set_ui(one,1);
	mpz_set_ui(i,0);
	for(;mpz_cmp(n,i) != 0; mpz_add(i,i,one)){
		mpz_mul(res,res,base);
	}
	omp_set_lock(&mutex);
	mpz_mul(*result,*result,res);
	omp_unset_lock(&mutex);


	mpz_clear(res);
	mpz_clear(one);
	mpz_clear(i);
	
}
*/

/*
void parallelMul(mpz_t base, mpz_t expp, mpz_t modulus, mpz_t * result){
	mpz_t ress;
	mpz_init(ress);

	int my_rank = omp_get_thread_num();
	//printf("Soy->%d\n", my_rank);

	double start_time = omp_get_wtime();
	//clock_t ini = clock();
	mpz_powm(ress,base,expp,modulus);
	//clock_t end = clock();
	double seconds = omp_get_wtime() - start_time;	
	//double seconds = (double) (end - ini) / CLOCKS_PER_SEC;

	
	omp_set_lock(&mutex);
	if(menor == -1 || menor > seconds) menor = seconds;
	mpz_mul(*result,*result,ress);
	omp_unset_lock(&mutex);
	

	//mpz_clear(res);
}
*/

void parallelMul(mpz_t base, char ** exps,int size, int k,int h, mpz_t modulus, mpz_t * result){
	mpz_t local_res;
	mpz_t local_exp;
	mpz_t local_r;
	mpz_init(local_res);
	mpz_init(local_exp);
	mpz_init(local_r);
	mpz_set_ui(local_r,2);
	int my_rank = omp_get_thread_num();
	int r = 0;

	if(my_rank == 0) r = 0;
	else{
		r = my_rank * h;
		//printf("saasdasda\n");
		//r = ((int) pow(2.0,(double) (my_rank - 1)) * size) / k;
	}

	//printf("R->%d->%d\n", r,my_rank);
	//printf("R->%d\n", size);
	mpz_set_str(local_exp,exps[my_rank],2);
	mpz_pow_ui(local_r,local_r,r);
	mpz_mul(local_exp,local_exp,local_r);


	//clock_t ini = clock();
	double start_time = omp_get_wtime();
	mpz_powm(local_res,base,local_exp,modulus);
	double seconds = omp_get_wtime() - start_time;	
	//clock_t end = clock();
	//double seconds = (double) (end - ini) / CLOCKS_PER_SEC;

	omp_set_lock(&mutex);
	if(menor == -1 || menor > seconds) menor = seconds;
	mpz_mul(*result,*result,local_res);
	omp_unset_lock(&mutex);
}


char ** cifrar(char * base, Keys keys, int numThreads, int * tamRes){
	mpz_t key;
	mpz_t modulus;
	mpz_t letra;
	mpz_t n;
	mpz_t threads;
	mpz_t mulResult;
	int baseSize = (int) strlen(base);
	int temp = 0;


	mpz_init(key);
	mpz_init(modulus);
	mpz_init(letra);	
	mpz_init(threads);
	mpz_init(n);
	mpz_init(mulResult);
	mpz_set_ui(threads,numThreads);
	mpz_set_ui(mulResult,1);
	mpz_set_str(key,keys.publicKey,10);
	mpz_set_str(modulus,keys.modulus,10);
	mpz_div(n,key,threads);

	char ** res = (char **) malloc(sizeof(void *) * baseSize);
	omp_init_lock(&mutex);

	secTotal = 0;

	for(int i = 0; i < baseSize; i++){
		menor = -1;
		temp = base[i];
		mpz_set_ui(mulResult,1);
		mpz_set_ui(letra,temp);

		//#pragma omp parallel num_threads(numThreads)
		//parallelMul(letra,n,&mulResult);
		//if(verifyDiv(key,threads) == IMPAR) mpz_mul(mulResult,mulResult,letra);	
		//mpz_mod(mulResult,mulResult,modulus);

		//mpz_powm(mulResult,letra,key,modulus);

		
		/*#pragma omp parallel num_threads(numThreads)
		parallelMul(letra,n,modulus,&mulResult);
		int resto = getModulus(key,threads);
		for(int i = 0; i < resto; i++){
			mpz_mul(mulResult,mulResult,letra);
		}
		mpz_mod(mulResult,mulResult,modulus);
		secTotal += menor;
		*/


		int sizeExp = mpz_sizeinbase(key,2);
		int h = 0;
		int resto = 0;
		char * numberExp = (char *) malloc(sizeExp);
		mpz_get_str(numberExp,2,key);
		char ** exps = divideNumber(numberExp,sizeExp,numThreads,&h,&resto);


		
		#pragma omp parallel num_threads(numThreads)
		parallelMul(letra,exps,sizeExp,numThreads,h,modulus,&mulResult);
		mpz_mod(mulResult,mulResult,modulus);
		

		secTotal+=menor;

		res[i] = (char*) malloc(mpz_sizeinbase(mulResult,10));
		
		mpz_get_str(res[i],10,mulResult);

	}

	printf("%lf ", secTotal);

	mpz_clear(key);
	mpz_clear(modulus);
	mpz_clear(letra);
	mpz_clear(n);
	mpz_clear(threads);
	mpz_clear(mulResult);
	*tamRes = baseSize;

	return res;

}

char * descifrar(char ** base, Keys keys, int numThreads, int baseSize){
	mpz_t key;
	mpz_t modulus;
	mpz_t letra;
	mpz_t n;
	mpz_t threads;
	mpz_t mulResult;
	char * temp;


	mpz_init(key);
	mpz_init(modulus);
	mpz_init(letra);	
	mpz_init(threads);
	mpz_init(n);
	mpz_init(mulResult);
	mpz_set_ui(threads,numThreads);
	mpz_set_ui(mulResult,1);
	mpz_set_str(key,keys.privateKey,10);
	mpz_set_str(modulus,keys.modulus,10);
	mpz_div(n,key,threads);

	char * res = (char*) malloc(baseSize);
	omp_init_lock(&mutex);

	secTotal = 0;
	
	
	for(int i = 0; i < baseSize; i++){
		menor = -1;
		mpz_set_ui(mulResult,1);
		mpz_set_str(letra,base[i],10);

		//#pragma omp parallel num_threads(numThreads)
		//parallelMul(letra,n,&mulResult);
		//if(verifyDiv(key,threads) == IMPAR) mpz_mul(mulResult,mulResult,letra);
		//mpz_mod(mulResult,mulResult,modulus);

		//mpz_powm(mulResult,letra,key,modulus);

		/*
		#pragma omp parallel num_threads(numThreads)
		parallelMul(letra,n,modulus,&mulResult);
		int resto = getModulus(key,threads);
		for(int i = 0; i < resto; i++){
			mpz_mul(mulResult,mulResult,letra);
		}
		mpz_mod(mulResult,mulResult,modulus);
		secTotal += menor;
		*/

		int sizeExp = mpz_sizeinbase(key,2);
		int h = 0;
		int resto = 0;
		char * numberExp = (char *) malloc(sizeExp);
		mpz_get_str(numberExp,2,key);
		char ** exps = divideNumber(numberExp,sizeExp,numThreads,&h,&resto);

		
		#pragma omp parallel num_threads(numThreads)
		parallelMul(letra,exps,sizeExp,numThreads,h,modulus,&mulResult);
		mpz_mod(mulResult,mulResult,modulus);
		

		secTotal+=menor;



		temp = (char *) malloc(mpz_sizeinbase(mulResult,10));
		mpz_get_str(temp,10,mulResult);
//		printf("%s\n", temp);
		res[i] = atoi(temp);
		// free(temp);
	}
	
	printf("%lf ", secTotal);

	mpz_clear(key);
	mpz_clear(modulus);
	mpz_clear(letra);
	mpz_clear(n);
	mpz_clear(threads);
	mpz_clear(mulResult);

	return res;
}

