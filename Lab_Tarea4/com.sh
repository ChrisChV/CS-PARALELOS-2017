echo "Linked-List"
rm ll_run
gcc -g linked_list.c -o ll_run -lpthread
echo "----------------------------"
echo "Matrix-Vector Multiplications"
rm mv_run
gcc -g matrix_x_vector.c -o mv_run -lpthread
echo "----------------------------"
echo "PI Bussy"
rm pi_run
gcc -g pi.c -o pi_run -lpthread
echo "----------------------------"
echo "Strtok"
rm str_run
gcc -g strtok_p.c -o str_run -lpthread

