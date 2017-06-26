def fun(resFile, outFile):
	iteraciones = 25
	endFile = "###\n"
	archivo = open(resFile,'r')
	archivoOut = open(outFile,'w')
	heder = ""
	line = ""
	encry = []
	decry = []
	averache = []
	resEncry = 0
	resDecry = 0
	resAverache = 0
	while(True):
		heder = archivo.readline()
		if(heder == endFile): break
		for i in range(0,25):
			line = archivo.readline()
			line = line.split(' ')
			encry.append(float(line[0]))
			decry.append(float(line[1]))
			averache.append(float(line[0]) + float(line[1]) + float(line[2]))
		archivoOut.write(heder)
		resEncry = sum(encry) / float(iteraciones)
		resDecry = sum(decry) / float(iteraciones)
		resAverache = sum(averache) / float(iteraciones)
		archivoOut.write(str(resEncry) + '\n')
		archivoOut.write(str(resDecry) + '\n')
		archivoOut.write(str(resAverache) + '\n')
		encry = []
		decry = []
		averache = []
	archivo.close()
	archivoOut.close()

fun("set2.lab","set2.lab.res")

