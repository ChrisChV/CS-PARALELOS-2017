mpicc -g Trapecio/trapecio.c -o Trapecio/trap_run
mpicc -g ScatterGather/scatter_gather.c -o ScatterGather/sg_run
mpicc -g MatrizVectorMul/matrix_x_vector.c -o MatrizVectorMul/mv_run
gcc -g OddEven/odd_even_serial.c -o OddEven/oes_run
mpicc -g OddEven/odd_even_parallel.c -o OddEven/oep_run
