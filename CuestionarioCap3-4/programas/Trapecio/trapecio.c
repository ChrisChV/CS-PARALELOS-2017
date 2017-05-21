#include <stdio.h>
#include <mpi.h>
#include "time.h"

double f(double x){
	return x * x;
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len){
	double estimate;
	double x;
	estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
	for(int i = 1; i <= trap_count-1; i++){
		x = left_endpt + i * base_len;
		estimate += f(x);
	}
	estimate = estimate * base_len;
	return estimate;
}

int main(){
	int my_rank;
	int comm_sz;
	int n = 16384;
	int local_n = 0;
	double a = 0.0;
	double b = 3.0;
	double h = 0;
	double local_a;
	double local_b;
	double local_int;
	double total_int = 0;
	double seconds_min = 0;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	clock_t ini = clock();
	h = (b-a) /n;
	if(my_rank == comm_sz -1) local_n = n / comm_sz + (n % comm_sz);
	else local_n = n / comm_sz;
	local_a = a + my_rank * (n/comm_sz) * h;
	local_b = local_a + local_n * h;
	
	local_int = Trap(local_a, local_b, local_n, h);
	
	
	MPI_Reduce(&local_int,&total_int,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	clock_t end = clock();
	double seconds = (double) (end - ini) / CLOCKS_PER_SEC;
	//MPI_Reduce(&seconds,&seconds_min,1,MPI_DOUBLE,MPI_MIN,0,MPI_COMM_WORLD);
	/*
	if(my_rank != 0){
		MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
	}
	else{
		total_int = local_int;
		for(int i = 1; i < comm_sz; i++){
			MPI_Recv(&local_int,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			total_int += local_int;
		}
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
	}
	*/
	if(my_rank == 0){
		printf("With n = %d trapezoids, our estimate\n", n);
		printf("of the integral from %f to %f = %.15e\n", a, b, total_int);	
		printf("TIME->%lf\n",seconds);
	}

	MPI_Finalize();
	return 0;
}