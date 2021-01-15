#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

//Definirea fisierelor
ifstream f("copii_cuminti.txt");
ifstream g("copii_rai.txt");
ifstream k("inventar.txt");

//Functie care va fi friend function pentru anumite clase
int CopilCuminte_Rau(string nume, string prenume){
//Parcurg doar lista copiilor cuminti, daca nu se gaseste pe acea lista, cu siguranta este pe lista copiilor rai
	f.clear();
	f.seekg(0, ios::beg);

	string nume_fisier, prenume_fisier;
	while(f){
		f >> nume_fisier;
		f >> prenume_fisier;
		if(nume_fisier == nume && prenume_fisier == prenume){
			return 1;
		}
	}
	
	return 0;

}

//Definirea claselor
class Scrisoare{
private:
	int buget;
public:
	string Nume;
	string Prenume;
	int Varsta;
	string Oras;
	char Wishlist[100];
	string Culoare;
	int nr_cadouri = 0;
	int acadele = 0;
	
	
	Scrisoare(string nume, string prenume, int varsta, string oras, char wishlist[100], string culoare){
		Nume = nume;
		Prenume = prenume;
		Varsta = varsta;
		Oras = oras;
		strcpy(Wishlist, wishlist);
		Culoare = culoare;
	}

	void setBuget(int n){
		this->buget = n;
	}

	void operator +(){
		nr_cadouri = nr_cadouri + 1;
	}


	int getBuget(){
		return this->buget;
	}

	int getNr_cadouri(){
		return nr_cadouri;
	}

	void show(){
		cout << "Nume: " << Nume << endl;
		cout << "Prenume: " << Prenume << endl;
		cout << "Varsta: " << Varsta << endl;
		cout << "Oras: " << Oras << endl;
		cout << "Wishlist: " << Wishlist << endl;
		cout << "Culoare: " << Culoare << endl;
	}
};




class Elf{
public:
int Nr_acadele = 0;
string Orase;
int cadouri_baieti;
int cadouri_fete;

	friend int CopilCuminte_Rau(string nume, string prenume);

	void alocareJucarii(Scrisoare letter){
		cout << letter.Nume << " va primi: ";

		f.clear();
		f.seekg(0, ios::beg);

		g.clear();
		g.seekg(0, ios::beg);		

		//Se seteaza bugetul

		if(CopilCuminte_Rau(letter.Nume, letter.Prenume) == 1) letter.setBuget(100);

		if(CopilCuminte_Rau(letter.Nume, letter.Prenume) == 0) letter.setBuget(10);

		//In cazul in care in inventar nu exista nimic se ofera un cadou default

		k.seekg(0, ios::end);

		
		if (k.tellg() == 0) {    //Verific daca inventarul este gol vazand daca distanta de la inceputul fisierului pana la cursor (care este plasat la sfarsitul fisierului) este 0, adica in fisier nu se afla nimic
 			cout << "Cadou default" << " ";
 			+letter;
		}

		//Spargem wishlistul in jucarii individuale si le cautam in inventar, daca exista si exista buget, adaugam jucaria in punga cu cadouri

		char *token;
		int bug = letter.getBuget();

		token = strtok (letter.Wishlist, ";"); //Folosim ca separator caracterul ";" deoarece jucariile pot contine spatii (ex: masina de pompieri)
		while (token != NULL){
			k.clear();
			k.seekg(0, ios::beg);

			char jucarie[50];
			int pret ;
			int exista = 0;

			while(k){ 
				k >> jucarie;
				k >> pret;
				
				if (strcmp (jucarie, token) == 0){
					if (bug - pret >= 0 && exista != 1){
						+letter;
						bug = bug - pret;
						cout << token << " ";
						exista = 1;
					}
				}	
			}

			token = strtok (NULL, ";");
		}


		//In cazul in care nu s-a alocat niciun cadou in prima faza, se ofera un cadou default;
		if(letter.getNr_cadouri() == 0){
			cout << "Cadou default" << " ";
			+letter;
		}

		//In cazul in care a mai ramas buget necheltuit se adauga acadele pentru fiecare dolar necheltuit

		if (bug > 0){
			while (bug != 0){
				letter.acadele++;
				Nr_acadele++;
				bug--;
			}
			cout << "si " << letter.acadele << " acadele ";
		}

		cout << endl;

		//Numar cadourile pe care le primeste fiecare copil in functie de culoarea plicului pentru a tine evidenta de cate ambalaje albastre si roz trebuie folosite

		if(letter.Culoare == "Albastru"){ 
			cadouri_baieti = letter.getNr_cadouri();
		}

		if(letter.Culoare == "Roz"){
			cadouri_fete = letter.getNr_cadouri();

		}

	}

};




class Trol: public Elf{
public:
int nr_carbuni = 0;
int Cadouri_baieti = 0;
int Cadouri_fete = 0;


	void Impachetare(Scrisoare letter){

		try{
			if("Albastru" == letter.Culoare){
				Cadouri_baieti += cadouri_baieti;
			}
	
			else throw "Date incorecte";
		}

		catch (...){
		
			Cadouri_fete += cadouri_fete;
		}	
	}

	void Adaugare_carbune(Scrisoare letter){

		if(CopilCuminte_Rau(letter.Nume, letter.Prenume) == 0) nr_carbuni++;
	}

	void getAmbalaje(){

		cout << "Au fost folosite " << Cadouri_fete << " ambalaje pentru fete si " << Cadouri_baieti << " ambalaje pentru baieti.\n";
	}	
};



class DoamnaCraciun: public Trol{
public:


	template <typename X>
	X AcadeleSiCarbuni(){
		X suma = Nr_acadele + (nr_carbuni/2.0);
		return suma;
	}
};




class MosCraciun{
public:

	MosCraciun(){
		cout << "Mos craciun pleca spre Filipine, dar mai intai arata drumul Doamnei Craciun!\n";
	}

	//Aceasta este matricea de adiacenta pentru orasele alese de catre mine, unde matrice_distanta[i][j] reprezinta distanta dintre orasul i si orasul j
	//Fiind un graf hamiltonian, matricea este simetrica fata de diagonala principala
	int matrice_distanta[5][5] = {
		{0, 9307, 9528, 8739, 8829},
		{9307, 0, 221, 576, 483},
		{9528, 221, 0, 794, 702},
		{8739, 576, 794, 0, 92},
		{8829, 483, 702, 92, 0}
	};

	//Folosesc un algoritm de gasirea drumului "de cost minim" pentru a gasi drumul optim (distanta cea mai scurta parcursa) optimizat strict pentru cazul meu, o matrice de 5 linii si 5 coloane
	void gasire_drum(){
		int visitat[5], muchie, suma_min = 0;
		const char *orase[5] = { "Laponia", "Cebu", "Cagayan de Oro", "Quezon", "Tayabas"}; // folosesc acest vector de stringuri pentru a afisa numele oraselor in loc de indicele acestora
   
  		for(int i=0;i<5;i++){
    		visitat[i]=false;
		}
  
		
   		muchie = 0;
  		
   		visitat[0] = true;
  		
   		int x, y;            
  		
   		cout << endl;
   		while (muchie < 4){
 		
   		    int min = 100000;
   		    x = 0;
   		    y = 0;
  		
   		    for (int i = 0; i < 5; i++) {
   		      	if (visitat[i]){
   		          	for (int j = 0; j < 5; j++) {
   		           		 if (!visitat[j] && matrice_distanta[i][j]){
   		                	if (min > matrice_distanta[i][j]){
   		                    	min = matrice_distanta[i][j];
   		                    	x = i;
   		                    	y = j;
   		                	}
   		            	}
   		       		}
   		      	}
   		    }

   		    suma_min += matrice_distanta[x][y];
   		    cout << orase[x] <<  " ---> " << orase[y] << " :  " << matrice_distanta[x][y];
   		    cout << endl;
   		    visitat[y] = true;
   		    muchie++;
   		}

   		cout << endl << "Distanta minima parcursa este de " << suma_min << " km.\n";
	}

	~MosCraciun(){
		cout << "\nMos Craciun a reusit sa distribuie cadourile cu ajutorul elfilor, trolilor si al Doamnei Craciun!\n";
	}

};



int main(){

	f.clear();
	f.seekg(0, ios::beg);

	g.clear();
	g.seekg(0, ios::beg);

	//Afisez listele pentru o vizualizare mai buna

	cout << "Lista cu copii cuminti:\n";
	while(f){
		string nume, prenume;
		f >> nume >> prenume;

		cout << nume << " " << prenume << endl;
	}

	cout << endl;

	cout << "Lista cu copii rai (cei carora li se va adauga un carbune la cadou):\n";
	while(g){
		string nume, prenume;
		g >> nume >> prenume;

		cout << nume << " " << prenume << endl;
	}

	//Imi apelez toate functiile pentru a executa tot procesul

	cout << "Lista cu cadouri:\n";

	char wishlist[100] = "A;B;C";
	char wishlist1[100] = "B;D";
	char wishlist2[100] = "C";
	char wishlist3[100] = "D";
	char wishlist4[100] = "D;C";

	Scrisoare letter("Jessa", "Joshua", 7, "Cebu", wishlist, "Albastru");
	Scrisoare letter1("Mae", "Christian", 10, "Manila", wishlist1, "Albastru");
	Scrisoare letter2("Trisha", "Jasmine", 10, "Quezon", wishlist2, "Roz"); 
	Scrisoare letter3("Vincent", "Angelo", 10, "Manila", wishlist3, "Albastru");
	Scrisoare letter4("Grace", "Bea", 10, "Quezon", wishlist4, "Roz"); 

	DoamnaCraciun worker; //Declar un singur obiect de tip DoamnaCraciun deoarece aceasta clasa mosteneste si clasa Trol si clasa Elf

	worker.alocareJucarii(letter);
	worker.alocareJucarii(letter1);
	worker.alocareJucarii(letter2);
	worker.alocareJucarii(letter3);
	worker.alocareJucarii(letter4);

	cout << endl;

	worker.Impachetare(letter);
	worker.Impachetare(letter1);
	worker.Impachetare(letter2);
	worker.Impachetare(letter3);
	worker.Impachetare(letter4);

	worker.Adaugare_carbune(letter);
	worker.Adaugare_carbune(letter1);
	worker.Adaugare_carbune(letter2);
	worker.Adaugare_carbune(letter3);
	worker.Adaugare_carbune(letter4);

	worker.getAmbalaje();

	cout << endl;

	cout << "Bugetul extra este de " << worker.AcadeleSiCarbuni<float>() << "$.\n"; 

	cout << endl;

	MosCraciun mos;
	mos.gasire_drum();
	

	return 0;
}