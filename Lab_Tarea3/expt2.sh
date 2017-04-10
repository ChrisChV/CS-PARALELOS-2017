for i in {1..5}
do
    mpiexec -n 16 ./mv_run 16384 1
done
