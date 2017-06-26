//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <cuda.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <tuple>

using namespace std;

typedef int Color;

void printColores(Color *** c, int altura, int ancho){
	cout<<altura<<endl;
	cout<<ancho<<endl;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < altura; j++){
			for(int k = 0; k < ancho; k++){
				cout<<c[i][j][k]<<endl;
			}
		}
	}
}

tuple<Color***,int,int> loadImage(string file){
	string file2;
	for(auto iter = file.begin(); iter != file.end(); ++iter){
		if(*iter == '.') break;
		else file2.push_back(*iter);
	}
	string tt = file2;
	file2 += ".txt";
	string comand = "convert " + file + " " + file2;
	system(comand.c_str());
	ifstream archivo(file2.c_str());
	string temp;
	for(int i = 0; i < 4; i++){
		archivo>>temp;
	} 
	int altura;
	int ancho;
	string heder;
	archivo>>heder;
	bool f = false;
	string a;
	string b;
	for(auto iter = heder.begin(); iter != heder.end(); ++iter){
		if(*iter == ',' and !f) f = true;
		else if(*iter == ',' and f) break;
		else if(f) b.push_back(*iter);
		else a.push_back(*iter);
	}
	char linea[300];
	archivo.getline(linea,300);
	ancho = stoi(a);
	altura = stoi(b);
	Color *** res= (Color ***) malloc(sizeof(void **) * 3);
	for(int i = 0; i < 3; i++){
		res[i] = (Color **) malloc(sizeof(void *) * altura);
		for(int j = 0; j < altura; j++){
			res[i][j] = (Color *) malloc(sizeof(Color) * ancho);
			for(int k = 0; k < ancho; k++){
				res[i][j][k] = 0;
			}
		}
	}

	int fil = 0;
	int col = 0;
	int total = 1;
	while(archivo.getline(linea,300)){
		string rgb(linea);
		string r;
		string g;
		string b;
		int estado = 0;
		int sum = 0;
		for(auto iter = rgb.begin(); iter != rgb.end(); ++iter){
			if(estado != 0){
				if(estado == 1){
					if(*iter != ','){
						if(*iter != ' ') r.push_back(*iter);
					} 
					else estado = 2;
				}
				else if(estado == 2){
					if(*iter != ',') {
						if(*iter != ' ') g.push_back(*iter);
					}
					else estado = 3;
				}
				else if(estado == 3){
					if(*iter != ',' and *iter != ')') {
						if(*iter != ' ') b.push_back(*iter);
					}
					else break;
				}
			}
			else if(*iter == '(') estado = 1;
		}
		res[0][fil][col] = stoi(r);
		res[1][fil][col] = stoi(g);
		res[2][fil][col] = stoi(b);
		col++;
		if(col == ancho){
			col = 0;
			fil++;
		}
		total++;
	}
	string del = "rm " + file2;
	system(del.c_str());
	return make_tuple(res,altura,ancho);
}

void saveImage3(string outFile){
	ifstream archivo(outFile.c_str());
	Color temp = 0;
	int altura = 0;
	int ancho = 0;
	archivo>>altura;
	archivo>>ancho;
	Color *** res= (Color ***) malloc(sizeof(void **) * 3);
	for(int i = 0; i < 3; i++){
		res[i] = (Color **) malloc(sizeof(void *) * altura);
		for(int j = 0; j < altura; j++){
			res[i][j] = (Color *) malloc(sizeof(Color) * ancho);
			for(int k = 0; k < ancho; k++){
				archivo>>temp;
				res[i][j][k] = temp;
			}
		}
	}
	archivo.close();
	ofstream archivoB(outFile.c_str());
	archivoB<<"# ImageMagick pixel enumeration: "<<ancho<<","<<altura<<",255,srgb"<<endl;
	for(int j = 0; j < altura; j++){
		for(int k = 0; k < ancho; k++){
			archivoB<<k<<","<<j<<": ("<<res[0][j][k]<<","<<res[1][j][k]<<","<<res[2][j][k]<<")"<<endl;
		}
	}
	archivoB.close();
}

void saveImage(string outFile){
	ifstream archivo(outFile.c_str());
	Color temp = 0;
	int altura = 0;
	int ancho = 0;
	archivo>>altura;
	archivo>>ancho;
	Color ** res = (Color **) malloc(sizeof(void*) * altura);
	for(int i = 0; i < altura; i++){
		res[i] = (Color *) malloc(sizeof(Color) * ancho);
	}
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < ancho; j++){
			archivo>>temp;
			res[i][j] = temp;
		}
	}

	archivo.close();
	string file = outFile + ".pgm";
	ofstream archivoB(file.c_str());
	archivoB<<"P2"<<endl;
	archivoB<<"# BalckWhite Image"<<endl;
	archivoB<<ancho<<" "<<altura<<endl;
	archivoB<<255<<endl;
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < ancho; j++){
			archivoB<<res[i][j]<<endl;
		}
	}
	archivoB.close();
}

int main(int argv, char ** argc){
	if(argv != 3){
		cout<<"Faltan Argumentos <file> <0:loadImage/1:saveImage>"<<endl;
		return 0;
	}
	string file = string(argc[1]);
	int type = stoi(argc[2]);
	if(type == 0){
		Color *** colors;
		int altura = 0;
		int ancho = 0;
		tie(colors,altura,ancho) = loadImage(file);
		printColores(colors,altura,ancho);
		free(colors);
	}
	else if(type == 1){
		saveImage(file);
	}
	else if(type == 2){
		cout<<"Heloo"<<endl;
		saveImage3(file);
	}
}
