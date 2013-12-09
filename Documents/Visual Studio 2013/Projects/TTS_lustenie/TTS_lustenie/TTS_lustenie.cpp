// TTS_lustenie.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

map<string,double> frekv_analyza(vector<string> text){
	vector<string>::iterator vectit;
	string::iterator strit,pomit;
	string pom;
	int pocet=0;
	double percenta=0;
	map<string,double> tabulka;
	map<string, int> pomtab;

	for(vectit=text.begin();vectit!=text.end();vectit++){
		for(strit=(*vectit).begin();strit!=(*vectit).end();strit++){
			pom.push_back(*strit);
			if(pom.size()==2){
				if(pomtab.find(pom)==pomtab.end()){
					pomtab[pom]=1;
					pom.erase();
				}
				else{
					pomtab[pom]++;
					pom.erase();
				}
				pocet++;
			}
		}
	}
	for (map<string, int>::iterator mapit  = pomtab.begin(); mapit != pomtab.end(); mapit++){
		if (atof((*mapit).first.c_str()) < 46){
			percenta = ((double)(*mapit).second / pocet) * 100;
			tabulka[(*mapit).first] = percenta;
		}
	}
	return tabulka;
}

vector<string> uprav(vector<string> text){
	vector<string>::iterator vectit;
	string::iterator strit;

	for(vectit=text.begin();vectit!=text.end();vectit++){
		for(strit=(*vectit).begin();strit!=(*vectit).end();strit++){
			if(*strit==' ')
				(*vectit).erase(strit);
		}
	}

	return text;
}

vector<string> nacitaj(){
	vector<string> text;
	int cislo=4;
	text.resize(cislo);
	vector<string>::iterator it=text.begin();
	string pomstring;

	ifstream vstup("vstup.txt");
	char pom;
	while(!vstup.eof()){
		getline(vstup,pomstring);
		for(string::iterator strit=pomstring.begin();strit!=pomstring.end();strit++){
			(*it).push_back(*strit);
		}
		it++;
		pomstring.erase();
	}
	return text;
}

map<string, double> nacitaj_tri(){
	map<string, double>trigramy;
	ifstream vstup("trigramy.tet");
	string pomstr;
	string kluc;
	string hodnota;
	while (!vstup.eof()){
		getline(vstup, pomstr);
		int i = 0;
		for (string::iterator strit = pomstr.begin(); strit != pomstr.end(); strit++){
			if (i < 3){
				kluc.push_back(*strit);
			}
			if (i>7){
				hodnota.push_back(*strit);
			}
			i++;
		}
		trigramy[kluc] = atof(hodnota.c_str());
		kluc.erase();
		hodnota.erase();
	}
	return trigramy;
}

map<string, double> nacitaj_tetra(){
	map<string, double>tetragramy;
	ifstream vstup("tetragramy.tet");
	string pomstr;
	string kluc;
	string hodnota;
	while (!vstup.eof()){
		getline(vstup, pomstr);
		int i = 0;
		for (string::iterator strit = pomstr.begin(); strit != pomstr.end(); strit++){
			if (i < 4){
				kluc.push_back(*strit);
			}
			if (i>8){
				hodnota.push_back(*strit);
			}
			i++;
		}
		tetragramy[kluc] = atof(hodnota.c_str());
		kluc.erase();
		hodnota.erase();
	}
	return tetragramy;
}

map<string, char> vytvor_tabulku(map<string, double>frekvencie){
	map<char, double> vysoke;
	map<string, char> tabulka;
	vector<string>abeceda;
	string kluc;
	vector<int>hodnota;
	double max=0;
	char pomchar;
	char prve, druhe;
	prve = '0';
	druhe = '1';

	while (abeceda.size()<46){
		if (druhe > '9'){
			prve++;
			druhe = '0';
		}
		kluc.push_back(prve);
		kluc.push_back(druhe);
		abeceda.push_back(kluc);
		kluc.erase();
		druhe++;
	}
	map<string, double>::iterator pom;
	for (vector<string>::iterator vectit = abeceda.begin(); vectit != abeceda.end(); vectit++){
		pom = frekvencie.find(*vectit);
		if (pom != frekvencie.end()){
			tabulka[(*pom).first] = (*pom).second;
		}
		else{
			tabulka[(*vectit)] = 0;
		}
	}

	for (map<string, double>::iterator mapit = frekvencie.begin(); mapit != frekvencie.end(); mapit++){
		
	}



	return tabulka;
}

vector<string> vrat_subs (vector<string>text,map<string,double>frekvencie){
	vector<string> subs;
	vector<string>::iterator textit;

	map<string, char> tabulka=vytvor_tabulku(frekvencie);

	for (textit = text.begin(); textit != text.end(); textit++){

	}

	return subs;
}

void main(){
	vector<string> text;
	map<string, double> frekvencie;
	map<string, double>trigramy = nacitaj_tri();
	map<string, double>tetragramy=nacitaj_tetra();
	text=nacitaj();
	text=uprav(text);
	frekvencie=frekv_analyza(text);
	text = vrat_subs(text, frekvencie);
	cin.get();
}
