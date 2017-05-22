#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>
#include "Utils.h"

void keyGeneration(int size){
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


	mpz_sub(tempP,p,tempP);
	mpz_sub(tempQ,q,tempQ);
	mpz_mul(n,tempP,tempQ);

	printf("%d\nn->",(int) mpz_sizeinbase(n,2));
	mpz_out_str(stdout,10,n);
	printf("\n");

	mpz_set_str(e,getCoprime(n),10);

	printf("%d\ne->",(int) mpz_sizeinbase(e,2));
	mpz_out_str(stdout,10,e);
	printf("\n");	

	//mpz_div(tempD,tempD,e);
	//mpz_mod(d,tempD,n);

	//mpz_set_str(d,getD(e,n),10);

	mpz_invert(d,e,n);

	printf("%d\nd->",(int) mpz_sizeinbase(d,2));
	mpz_out_str(stdout,10,d);
	printf("\n");	



}



