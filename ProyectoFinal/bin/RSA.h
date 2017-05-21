#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "Utils.h"

void keyGeneration(int size){
	mpz_t m;
	mpz_t p;
	mpz_t q;
	mpz_init(m);
	mpz_init(p);
	mpz_init(q);
	mpz_set_str(p,getRandomPrime(size,rand() % RANDOM_SEED_RANGE),10);
	mpz_set_str(q,getRandomPrime(size,rand() % RANDOM_SEED_RANGE),10);

	printf("%d\np->",(int) mpz_sizeinbase(p,2));
	mpz_out_str(stdout,10,p);
	printf("\n");

	printf("%d\nq->",(int) mpz_sizeinbase(q,2));
	mpz_out_str(stdout,10,q);
	printf("\n");

	mpz_mul(m,p,q);

	printf("%d\nm->",(int) mpz_sizeinbase(m,2));
	mpz_out_str(stdout,10,m);
	printf("\n");




}



