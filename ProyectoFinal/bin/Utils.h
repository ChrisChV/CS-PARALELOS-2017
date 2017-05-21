#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

#define RANDOM_SEED_RANGE 50000000

typedef struct{
	mpz_t m;
	mpz_t e;
	mpz_t q;
}Keys;

char * getRandom(int size, int seed){
	gmp_randstate_t r_state;
	gmp_randinit_default (r_state);
	gmp_randseed_ui(r_state, seed);
	mpz_t  n;
	mpz_init(n);
	mpz_urandomb(n,r_state,size);
	char * res = (char *) malloc(mpz_sizeinbase(n,10));
	mpz_get_str(res,10,n);
	mpz_clear(n);
	return res;
}

char * getRandomPrime(int size, int seed){
	mpz_t n;
	mpz_t prime;
	mpz_init(n);
	mpz_init(prime);
	mpz_set_str(n,getRandom(size,seed),10);
	mpz_nextprime(prime,n);
	char * res = (char *) malloc(mpz_sizeinbase(prime,10));
	mpz_get_str(res,10,prime);
	mpz_clear(n);
	mpz_clear(prime);
	return res;
}
