#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include "Utils.h"
#include <mpi.h>
#include <pthread.h>

#define CIFRAR 0
#define DESCIFRAR 1

char *** opMatrix;

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
	//f("\n");

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


typedef struct{
	int rank;
	int i_init;
	int i_end;
	int h; //H del r - numero de threads - dependiendo de la funcion
	char * local_exp;
	char * modulus;
	char * base_c;
	char ** base_d;
	char *** res_c; // resultado para cuando se multiplican los resultados
	char ** res_d;
} PthreadArgs;

void * parallelMulC(void * args){
	mpz_t local_res;
	mpz_t m_local_exp;
	mpz_t local_r;
	mpz_t m_modulus;
	mpz_t m_base;
	mpz_init(local_res);
	mpz_init(m_base);
	mpz_init(m_local_exp);
	mpz_init(local_r);
	mpz_init(m_modulus);
	mpz_set_ui(local_r,2);

	PthreadArgs pArgs = *((PthreadArgs * ) args);

	int my_rank = pArgs.rank;
	int i_init = pArgs.i_init;
	int i_end = pArgs.i_end;
	int h = pArgs.h;
	char * local_exp = pArgs.local_exp;
	char * modulus = pArgs.modulus;
	char * base = pArgs.base_c;
	int r = 0;
	if(my_rank == 0) r = 0;
	else{
		r = my_rank * h;
	}
	mpz_set_str(m_local_exp,local_exp,2);

	mpz_set_str(m_modulus,modulus,10);
	mpz_pow_ui(local_r,local_r,r);
	mpz_mul(m_local_exp,m_local_exp,local_r);

	int temp = 0;

	for(int i = i_init; i < i_end; i++){
		temp = base[i];
		mpz_set_ui(m_base,temp);
		mpz_powm(local_res,m_base,m_local_exp,m_modulus);
		opMatrix[i - i_init][my_rank] = (char *) malloc(mpz_sizeinbase(local_res,10));		
		mpz_get_str(opMatrix[i - i_init][my_rank],10,local_res);
	}

	mpz_clear(local_res);
	mpz_clear(m_local_exp);
	mpz_clear(local_r);
	mpz_clear(m_modulus);
	mpz_clear(m_base);
}

void * parallelMulD(void * args){
	mpz_t local_res;
	mpz_t m_local_exp;
	mpz_t local_r;
	mpz_t m_modulus;
	mpz_t m_base;
	mpz_init(local_res);
	mpz_init(m_base);
	mpz_init(m_local_exp);
	mpz_init(local_r);
	mpz_init(m_modulus);
	mpz_set_ui(local_r,2);

	PthreadArgs pArgs = *((PthreadArgs * ) args);

	int my_rank = pArgs.rank;
	int i_init = pArgs.i_init;
	int i_end = pArgs.i_end;
	int h = pArgs.h;
	char * local_exp = pArgs.local_exp;
	char * modulus = pArgs.modulus;
	char ** base = pArgs.base_d;

	int r = 0;
	if(my_rank == 0) r = 0;
	else{
		r = my_rank * h;
	}
	mpz_set_str(m_local_exp,local_exp,2);
	mpz_set_str(m_modulus,modulus,10);
	mpz_pow_ui(local_r,local_r,r);
	mpz_mul(m_local_exp,m_local_exp,local_r);

	for(int i = i_init; i < i_end; i++){

		mpz_set_str(m_base,base[i],10);
		mpz_powm(local_res,m_base,m_local_exp,m_modulus);
		opMatrix[i - i_init][my_rank] = (char *) malloc(mpz_sizeinbase(local_res,10));
		mpz_get_str(opMatrix[i - i_init][my_rank],10,local_res);
	}

	mpz_clear(local_res);
	mpz_clear(m_local_exp);
	mpz_clear(local_r);
	mpz_clear(m_modulus);
	mpz_clear(m_base);
}

void * parallelMulMatrixC(void * args){
	mpz_t local_res;
	mpz_t local_num;
	mpz_t m_modulus;
	mpz_init(local_res);
	mpz_init(local_num);
	mpz_init(m_modulus);
	mpz_set_ui(local_res,1);
	PthreadArgs pArgs = *((PthreadArgs * ) args);
	int my_rank = pArgs.rank;
	int i_init = pArgs.i_init;
	int i_end = pArgs.i_end;
	char * modulus = pArgs.modulus;
	char *** res = pArgs.res_c;
	int numThreads = pArgs.h;
	mpz_set_str(m_modulus,modulus,10);
	for(int i = i_init; i < i_end; i++){
		for(int j = 0; j < numThreads; j++){
			mpz_set_str(local_num,opMatrix[i][j],10);
			mpz_mul(local_res,local_res,local_num);

		}

		mpz_mod(local_res,local_res,m_modulus);
		(*res)[i] = (char *) malloc(mpz_sizeinbase(local_res,10));
		mpz_get_str((*res)[i],10,local_res);

	}
	mpz_clear(local_res);
	mpz_clear(local_num);
	mpz_clear(m_modulus);
	

}

void * parallelMulMatrixD(void * args){
	mpz_t local_res;
	mpz_t local_num;
	mpz_t m_modulus;
	mpz_init(local_res);
	mpz_init(local_num);
	mpz_init(m_modulus);
	mpz_set_ui(local_res,1);
	PthreadArgs pArgs = *((PthreadArgs * ) args);
	int my_rank = pArgs.rank;
	int i_init = pArgs.i_init;
	int i_end = pArgs.i_end;
	char * modulus = pArgs.modulus;
	char ** res = pArgs.res_d;
	int numThreads = pArgs.h;
	mpz_set_str(m_modulus,modulus,10);
	char * temp;
	for(int i = i_init; i < i_end; i++){
		for(int j = 0; j < numThreads; j++){
			mpz_set_str(local_num,opMatrix[i][j],10);
			mpz_mul(local_res,local_res,local_num);
		}
		mpz_mod(local_res,local_res,m_modulus);
		temp = (char *) malloc(mpz_sizeinbase(local_res,10));
		mpz_get_str(temp,10,local_res);
		(*res)[i] = atoi(temp);
	}
	mpz_clear(local_res);
	mpz_clear(local_num);
	mpz_clear(m_modulus);
}

void cifrar(char * base, int tamBase, Keys keys, int numThreads, char *** res, int mpi_rank, int mpi_size){
	mpz_t key;
	mpz_init(key);
	mpz_set_str(key,keys.publicKey,10);
	int sizeExp = mpz_sizeinbase(key,2);
	char * numberExp = (char *) malloc(sizeExp);
	mpz_get_str(numberExp,2,key);
	int h = 0;
	int resto = 0;
	char ** exps = divideNumber(numberExp,sizeExp,numThreads,&h,&resto);
	int local_n = 0; // numero de caracteres que va a operar el nodo
	int i_init = 0;
	int i_end = 0;
	local_n = tamBase / mpi_size;
	i_init = mpi_rank * local_n;
	if(mpi_rank == mpi_size - 1){
		local_n += tamBase % mpi_size;
		i_end = tamBase;
	}
	else{
		i_end = i_init + local_n;
	}
	llenarMatrix(&opMatrix,local_n,numThreads);
	(*res) = (char **) malloc(sizeof(void *) * local_n);
	pthread_t threads[numThreads];
	PthreadArgs args[numThreads];

	for(int i = 0; i < numThreads; i++){
		args[i].rank = i;
		args[i].i_init = i_init;
		args[i].i_end = i_end;
		args[i].h = h;
		args[i].local_exp = exps[i];
		args[i].modulus = keys.modulus;
		args[i].base_c = base;
		pthread_create(&threads[i],NULL,parallelMulC,(void *) &args[i]);
	}
	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}
	int characterPerThread = local_n / numThreads;
	for(int i = 0; i < numThreads; i++){
		args[i].i_init = i * characterPerThread;
		if(i == numThreads - 1){
			args[i].i_end = local_n;
			//args[i].h = characterPerThread + (local_n % numThreads);
		}
		else{
			args[i].i_end = args[i].i_init + characterPerThread;
			//args[i].h = characterPerThread;
		}
		args[i].res_c = res;
		args[i].h = numThreads;

		pthread_create(&threads[i],NULL,parallelMulMatrixC,(void *) &args[i]);

	}

	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}


	//for(int i = 0; i < local_n; i++){
	//	printf("%s\n", (*res)[i]);
	//}
}

void descifrar(char ** base, int tamBase, Keys keys, int numThreads, char ** res, int mpi_rank, int mpi_size){
	mpz_t key;
	mpz_init(key);
	mpz_set_str(key,keys.privateKey,10);
	int sizeExp = mpz_sizeinbase(key,2);
	char * numberExp = (char *) malloc(sizeExp);
	mpz_get_str(numberExp,2,key);
	int h = 0;
	int resto = 0;
	char ** exps = divideNumber(numberExp,sizeExp,numThreads,&h,&resto);
	int local_n = 0; // numero de caracteres que va a operar el nodo
	int i_init = 0;
	int i_end = 0;
	local_n = tamBase / mpi_size;
	i_init = mpi_rank * local_n;
	if(mpi_rank == mpi_size - 1){
		local_n += tamBase % mpi_size;
		i_end = tamBase;
	}
	else{
		i_end = i_init + local_n;
	}
	llenarMatrix(&opMatrix,local_n,numThreads);
	(*res) = (char *) malloc(local_n);
	pthread_t threads[numThreads];
	PthreadArgs args[numThreads];
	for(int i = 0; i < numThreads; i++){
		args[i].rank = i;
		args[i].i_init = i_init;
		args[i].i_end = i_end;
		args[i].h = h;
		args[i].local_exp = exps[i];
		args[i].modulus = keys.modulus;
		args[i].base_d = base;
		pthread_create(&threads[i],NULL,parallelMulD,(void *) &args[i]);
	}

	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}

	int characterPerThread = local_n / numThreads;
	for(int i = 0; i < numThreads; i++){
		args[i].i_init = i * characterPerThread;
		if(i == numThreads - 1){
			args[i].i_end = local_n;
			//args[i].h = characterPerThread + (local_n % numThreads);
		}
		else{
			args[i].i_end = i_init + characterPerThread;
			//args[i].h = characterPerThread;
		}
		args[i].res_d = res;
		args[i].h = numThreads;
		pthread_create(&threads[i],NULL,parallelMulMatrixD,(void *) &args[i]);
	}

	for(int i = 0; i < numThreads; i++){
		pthread_join(threads[i],NULL);
	}	
}

