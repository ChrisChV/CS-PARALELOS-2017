if ["$1" -eq ""] | ["$2" -eq ""] | ["$3" -eq ""]
then
	 echo "Faltan Argumentos <numThreads> <fileSize> <numIteraciones>"
else
	echo "Init"
	echo "$1 $2" >> set2
	for ((i=1; i <=$3;i++));
	do
		echo $i
		./run $1 512 $2 >> set2
		sleep 2
	done
	echo "Done"
fi


