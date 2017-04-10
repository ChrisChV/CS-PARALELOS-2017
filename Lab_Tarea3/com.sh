mpicc -g trapecio.c -o trap_run
mpicc -g scatter_gather.c -o sg_run
mpicc -g matrix_x_vector.c -o mv_run
gcc -g odd_even_serial.c -o oes_run
mpicc -g odd_even_parallel.c -o oep_run
