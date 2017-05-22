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

int coprimes(mpz_t a, mpz_t b){
	mpz_t temp;
	mpz_init(temp);
	mpz_gcd(temp,a,b);
	if(mpz_cmp_si(temp,1) == 0){
		mpz_clear(temp);
		return 1;	
	} 
	mpz_clear(temp);
	return 0;
}

char * getCoprime(mpz_t n){
	mpz_t e;
	mpz_t one;
	mpz_t two;
	mpz_t temp;
	mpz_init(e);
	mpz_init(one);
	mpz_init(two);
	mpz_init(temp);
	mpz_set_ui(one,1);
	mpz_set_ui(two,3);
	mpz_set_ui(temp,20);
	mpz_set(e,n);
	mpz_div(e,e,two);
	mpz_sub(e,e,temp);
	do{
		mpz_sub(e,e,one);	
	}while(coprimes(e,n) == 0  && mpz_cmp_si(e,1) != 0);
	char * res = (char *) malloc(mpz_sizeinbase(e,10));
	mpz_get_str(res,10,e);
	mpz_clear(e);
	mpz_clear(one);
	mpz_clear(two);
	mpz_clear(temp);
	return res;
}




