if ["$1" -eq ""] | ["$2" -eq ""] | ["$3" -eq ""]
then
	 echo "Faltan Argumentos <numThreads> <bitzSize> <numIteraciones>"
else
	echo "Init"
	echo "$1 $2" >> set1
	for ((i=1; i <=$3;i++));
	do
		echo $i
		./run $1 $2 5000 >> set1
		sleep 2
	done
	echo "Done"
fi


