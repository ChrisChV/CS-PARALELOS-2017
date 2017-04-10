touch res.csv
for i in 1 2 4 8 16
do
    echo  "$i-1024,$i-2048,$i-4096,$i-8192,$i-16384" >> res.csv
    for j in {1..5}
        do
	    for k in 1024 2048 4096 8192 16384
		do
                    mpiexec -n $i ./mv_run $k 1 >> res.csv
		    echo -n "," >> res.csv
		done
	    echo "" >> res.csv
	    echo "$i-$j"
        done
	echo -e "\n" >> res.csv
done
