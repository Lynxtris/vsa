#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;

class TTS{
public:
	wstring text;
	string heslo1, heslo2;
	map<wchar_t,string>tabulka;
	int datum;

	void nacitaj_hesla(int datum){				//nacitanie hesiel zo suboru
		ifstream subor("hesla.txt");
		string::iterator it;
		string pom;
		int i=1;
		bool medzera=false;
		while(!subor.eof()){
			getline(subor,pom);
			if(i==datum){
				break;
			}
			i++;
		}
		for(it=pom.begin();it!=pom.end();it++){
			if(*it==' '){
				medzera=true;
				it++;
			}
			if(medzera==false)
				this->heslo1.push_back(*it);
			else
				this->heslo2.push_back(*it);
		}
	}

	void vytvor_substituciu(int datum){					//funkcia vyvara substitucnu tabulku  vstup: den sifrovania
		map<wchar_t,string> tabulka;
		map<wchar_t,string>::iterator itt=tabulka.begin();
		wstring kluc;
		wstring::iterator klucovy;
		vector<string> hodnota;
		vector<string>::iterator ith;
		ifstream subor("substitucia_zaklad.txt");
		string pom,medzikrok;
		string::iterator its;
		bool kontrola=false;
		while(!subor.eof()){									//zo suboru sa nacitava zakladna substitucna tabulka
			getline(subor,pom);
			for(its=pom.begin();its!=pom.end();its++){
				if(*its==' ')
					kontrola=true;
				else if(kontrola==false)
					kluc.push_back(*its);
				else
					medzikrok.push_back(*its);
			}
			hodnota.push_back(medzikrok);
			medzikrok.erase();
			kontrola=false;
		}
		ith=hodnota.begin();
		for(int i=1;i<datum;i++){
			ith++;
		}
		for(klucovy=kluc.begin();klucovy!=kluc.end();klucovy++){		//tabulka sa posuva podla dna sifrovania
			if(ith==hodnota.end())
				ith=hodnota.begin();
			tabulka[*klucovy]=*ith;
			ith++;
		}
		subor.close();
		this->tabulka=tabulka;				//vystup: substitucna tabulka
	}

	string substituuj_znak(wchar_t znak,map<wchar_t,string>tabulka){
		map<wchar_t,string>::iterator tabulkovy;
		string::iterator tt;
		string vymeneny;
		for(tabulkovy=tabulka.begin();tabulkovy!=tabulka.end();tabulkovy++){
			if((*tabulkovy).first==znak){										//pokial sa znaky zhoduju zameni sa za ciselne vyjadrenie
				for(tt=(*tabulkovy).second.begin();tt!=(*tabulkovy).second.end();tt++)
					vymeneny.push_back(*tt);
					break;
				}
		}
		return vymeneny;
	}	

	wstring substitucia(wstring text, map<wchar_t,string> tabulka){				//funkcia vytvara substituciu znakov  vstupy: text, substitucna tabulka
		wstring substituovany;
		wstring::iterator textovy;
		string::iterator tt;
		string pom;
		map<wchar_t,string>::iterator tabulkovy;
		for(textovy=text.begin();textovy!=text.end();textovy++){			//postupne sa prechadza text a substitucna tabulka
			pom=this->substituuj_znak(*textovy,tabulka);
			for(tt=pom.begin();tt!=pom.end();tt++){
				substituovany.push_back(*tt);
			}
		}
		return substituovany;			//vystup: substituovany text
	}

	void vypis_sifrovany(vector<wstring> text){				//vypis sifrovaneho textu na obrazovku a do suboru
	int i=0;
	ofstream vystup("vystup.txt");
	for(vector<wstring>::iterator it=text.begin();it!=text.end();it++){
		cout<<(*it).size()<<endl;
		cout<<this->datum<<"-"<<this->heslo1.size()<<"-"<<this->heslo2.size()<<endl;	//vypis hlavicky spravy
		for(wstring::iterator it2=(*it).begin();it2!=(*it).end();it2++){
			if(i==5){															//vypis po 5 znakov
				cout<<" ";
				vystup<<" ";
				i=0;
			}
			cout<<*it2;
			vystup<<*it2;
			i++;
		}
		cout<<'\n';								//oddelenie jednotlivych casti sprav
		cout<<'\n';
		vystup<<'\n';
		vystup<<'\n';
	}
	vystup.close();
}
};

vector<int> preved_heslo(string heslo){			//funkcia na prevedenie hesla do ciselneho vyjadrenia  vstup: text hesla
	string::iterator strit;
	vector<int> vysledne;						//struktura pre ulozenie vysledneho ciselneho vyjadrenia
	vector<int>::iterator wstring;
	bool kontrola=false;
	int counter=0,por1=0,por2;
	for(strit=heslo.begin();strit!=heslo.end();strit++){		//v cykle sa upravi heslo tak aby obsahovalo len velke pismena
		if(*strit==' ')
			heslo.erase(strit);
		if(*strit>'Z')
			*strit=*strit-32;
	}

	vysledne.resize(heslo.size());				//nastavenie velkosti vysledneho vektora

	for(char i='A';i<='Z';i++){					//postupne sa prechadza abeceda
		for(strit=heslo.begin();strit!=heslo.end();strit++){
			if(*strit==i){
				counter++;						//premenna pre ulozenie poradia daneho pismena v hesle
				por2=0;
				for(wstring=vysledne.begin();wstring!=vysledne.end();wstring++){   //cyklus sa stara o ulozenie ciselneho vyjadrenia na spravne miesto
					if(por1==por2){
						*wstring=counter;
						kontrola=true;
					}
					por2++;
				}
			}
			por1++;
		}
		kontrola=false;
		por1=0;
	}
	return vysledne;					//vystup: ciselne vyjadrenie hesla
}

wstring zmena(wstring text){
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);
	wifstream subor("uprava.txt");
	subor.imbue(utf8_locale);
	wstring pom;
	wstring::iterator it;
	map<wchar_t,wchar_t>tab;
	wchar_t kluc, hodnota;
	bool medzera=false;
	while(!subor.eof()){
		getline(subor,pom);
		for(it=pom.begin();it!=pom.end();it++){
			if(*it==' '){
				medzera=true;
				it++;
			}
			if(medzera==false)
				kluc=*it;
			else
				hodnota=*it;	
		}
		medzera=false;
		tab[kluc]=hodnota;
	}
	for(wstring::iterator it=text.begin();it!=text.end();it++){
		*it=tab[*it];
	}
	subor.close();
	return text;
}

vector<wstring> uprav_text(wstring text, map<wchar_t,string>tabulka){					//funkcia sa stara o korektnost vstupneho textu a jeho rozdelenie na casti    vstup: text na sifrovanie
	wstring::iterator strit,stritx;

	text=zmena(text);

	for(strit=text.begin();strit!=text.end();strit++){
		
		if(*strit==' ' && (*stritx=='.' || *stritx=='?' || *stritx=='/' || *stritx=='-'))
			text.erase(strit);
		else if(*strit==' ')
			*strit='-';
		else if (tabulka.find(*strit)==tabulka.end()==false){
			text.erase(strit);
			strit--;
		}
		stritx=strit;
	}

	vector<wstring> rozdeleny;
	vector<wstring>::iterator it_rozdel=rozdeleny.begin();
	wstring pom;
	wstring::iterator it_pom;

	if(text.size()<100){
		for(wstring::iterator it=text.begin();it!=text.end();it++){
			pom.push_back(*it);
		}
		rozdeleny.push_back(pom);
	}
	else{
		int i=0, poc=0;
		char znak='A';
		for(strit=text.begin();strit!=text.end();strit++){
			if(i>95 && i<120 && (*strit)=='-'){
				if(text.size()>poc+30){
					pom.push_back('/');
					pom.push_back(znak);
					rozdeleny.push_back(pom);
					pom.erase();
					pom.push_back(znak);
					pom.push_back('/');
					znak++;
					i=0;
				}
				else {
					for(wstring::iterator it=strit;it!=text.end();it++){
						pom.push_back(*it);
					}
					rozdeleny.push_back(pom);
					break;
				}
			}
			else{
				pom.push_back(*strit);
				i++;
				poc++;
			}
		}
	}
	rozdeleny.push_back(pom);

	return rozdeleny;					//vystup: upraveny text
}

wstring transpozicia (wstring text,vector<int> heslo){			//funkcia zabezpecuje transpoziciu textu  vstupy: text na transpoziciu, heslo v ciselnom vyjadreni
	vector<int>::iterator heslovy=heslo.begin();
	wstring::iterator textovy,textovy2;
	map<int,wstring> tabulka;
	map<int,wstring>::iterator tabulkovy;
	wstring transponovany;
	int pozicia=1,velkost=0;
	wstring pom;

	for(textovy=text.begin();textovy!=text.end();textovy++){
		for(textovy2=textovy;textovy2!=text.end();textovy2++){			//cyklus vytvara priamo stlpce transpozicnej tabulky
			if(pozicia==1){
				pom.push_back(*textovy2);
			}
			if(pozicia==heslo.size()){
				pozicia=0;
			}
			pozicia++;
		}
		tabulka[*heslovy]=pom;						//do transpozicnej tabulky sa ulozi stlpec tabulky pod prislusne cislo hesla
		pom.erase();
		heslovy++;
		pozicia=1;
		for(tabulkovy=tabulka.begin();tabulkovy!=tabulka.end();tabulkovy++){		//testovanie velkosti tabulky, ci obsahuje cely text
			velkost=velkost+(*tabulkovy).second.length();
		}
		if(velkost==text.size())
			break;
		velkost=0;
	}

	for(map<int,wstring>::iterator it=tabulka.begin();it!=tabulka.end();it++){
		for(wstring::iterator it2=(*it).second.begin();it2!=(*it).second.end();it2++){
			transponovany.push_back(*it2);
		}
	}

	return transponovany;
}

wstring uprav_sifrovany(wstring text){			//zasifrovany text sa upravuje aby na konci bola plna patica znakov
	//string::iterator itt;
	while(text.size()%5 !=0){
		text.push_back(rand()%5+'5');
	}
	return text;
}

void sifra_TTS(){
	TTS sifra;

	cout<<"zadaj den ";
	cin>>sifra.datum;

	sifra.nacitaj_hesla(sifra.datum);

	vector<int> prevedene_heslo1=preved_heslo(sifra.heslo1);
	vector<int> prevedene_heslo2=preved_heslo(sifra.heslo2);
	vector<wstring> rozdelene;

	sifra.vytvor_substituciu(sifra.datum);

	int zadaj;

	cout<<"1..........nacitanie zo suboru"<<endl;
	cout<<"2..........nacitanie z konzoly"<<endl;
	cin>>zadaj;
	switch(zadaj){
	case 1:
		{
		wifstream vstup("text.txt");
		while(!vstup.eof()){
			wstring pom;
			vstup>>pom;
			pom.push_back(' ');
			for(wstring::iterator it=pom.begin();it!=pom.end();it++){
				sifra.text.push_back(*it);
			}
		}
		break;
		}
	case 2:
		cin.clear();
		cin.ignore(20,'\n');
		getline(wcin,sifra.text);
		break;
	}

	rozdelene=uprav_text(sifra.text,sifra.tabulka);

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=transpozicia((*it),prevedene_heslo1);
	}
	
	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=transpozicia((*it),prevedene_heslo2);
	}

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=sifra.substitucia((*it),sifra.tabulka);
	}

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=uprav_sifrovany((*it));
	}

	sifra.vypis_sifrovany(rozdelene);
}

void sifra_STT(){
	TTS sifra;

	cout<<"zadaj den ";
	cin>>sifra.datum;

	sifra.nacitaj_hesla(sifra.datum);

	vector<int> prevedene_heslo1=preved_heslo(sifra.heslo1);
	vector<int> prevedene_heslo2=preved_heslo(sifra.heslo2);
	vector<wstring> rozdelene;

	sifra.vytvor_substituciu(sifra.datum);

	int zadaj;

	cout<<"1..........nacitanie zo suboru"<<endl;
	cout<<"2..........nacitanie z konzoly"<<endl;
	cin>>zadaj;
	switch(zadaj){
	case 1:
		{
		wifstream vstup("text.txt");
		while(!vstup.eof()){
			wstring pom;
			vstup>>pom;
			pom.push_back(' ');
			for(wstring::iterator it=pom.begin();it!=pom.end();it++){
				sifra.text.push_back(*it);
			}
		}
		break;
		}
	case 2:
		cin.clear();
		cin.ignore(20,'\n');
		getline(wcin,sifra.text);
		break;
	}

	rozdelene=uprav_text(sifra.text,sifra.tabulka);

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=transpozicia((*it),prevedene_heslo1);
	}
	
	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=transpozicia((*it),prevedene_heslo2);
	}

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=sifra.substitucia((*it),sifra.tabulka);
	}

	for(vector<wstring>::iterator it=rozdelene.begin();it!=rozdelene.end();it++){
		(*it)=uprav_sifrovany((*it));
	}

	sifra.vypis_sifrovany(rozdelene);
}

void main(){
	int zadaj;

	cout<<"1.......TTS"<<endl;
	cout<<"2.......STT"<<endl;
	cin>>zadaj;
	switch(zadaj){
		case 1: sifra_TTS();
			break;
		case 2:sifra_STT();
			break;
	}

	cin.get();
	cin.get();
}