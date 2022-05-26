#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#define H 19
#define W 38


FILE *pDosya;	


void set_cursor_position(int x, int y)
{
   COORD coord = { x, y };
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
} 


void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}


void ekraniTemizle() {
	
	system("cls");
}


char harita[H][W] =
{
   { "######################################" },
   { "#                                    #" },                             
   { "# ########### ########### ########## #" },                                               
   { "# #         # #         # #          #" },       
   { "# # ####### # # ####### # # ######## #" },  
   { "# #         # # ####### # # ######## #" }, 
   { "# # ######### #         # # ######## #" }, 
   { "# # ######### # ####### # #          #" }, 
   { "# # ######### # ####### # ########## #" }, 
   { "#                                    #" },
   { "# ############# ########## ######### #" },        
   { "# #           # #        # #    ##   #" },      
   { "# # ### # ### # # ###### # # ## ## # #" },         
   { "# # ### # ### # # ###### # # ## ## # #" },
   { "# # ### # ### # #        # # ## ## # #" },                                    
   { "# # ### # ### # # ###### # # ## ## # #" },
   { "# # ### # ### # # ###### # # ## ## # #" },
   { "#                                    #" },
   { "######################################" }
};


enum anaMenuKomutlari {Oyna = 101, Kontroller = 107, SkorlariYazdir = 119, OyundanCikisYap = 113};
enum anaMenuKomutlariCapsLock {OYNA = 69, KONTROLLER = 75, SKORLARI_YAZDIR = 87, OYUNDAN_CIKIS_YAP = 81};
enum oyunDurdur {oyunuDurdur = 112};
enum oyunDurdurCapsLock {OYUNU_DURDUR = 80};
enum canavarYon {UP = 119, DOWN = 115, RIGHT = 100, LEFT = 97};


char YolUzerindekiKarakter = ' ';


typedef struct Hareket{
	
	int sutunIndeks;
	int satirIndeks;
	int sutunYonundekiHareket;
	int satirYonundekiHareket;
	int yon;
		
} hareket;


struct canavarBilgileri{
	
	int Yon;
	int tempYon;
	double enKisaMesafe;
	hareket canavar;
	
}Canavar;


struct pacManBilgileri{
	
	hareket pacMan;	
	
}PacMan;


typedef struct{
	
	char isim[10];
	int skor;	
	
}oyuncuBilgileri;

oyuncuBilgileri bilgiler;


int yemKontrol = 0;


void ekraniTemizle();
void haritaTemizle();
void anaMenuOlustur();
void kontrolTuslari();
void karakterleriOyunIcinHazirla();
void haritaOlustur(oyuncuBilgileri *oyuncuVerileri);
void skorTablosunaKayitEkle();
void skorTablosunuYazdir(oyuncuBilgileri *oyuncuVerileri);
bool canavarYonKontrol(int canavarSutunIndeks, int canavarSatirIndeks);
int canavarPacManArasiMesafeHesap(int canavarSutunIndeks, int canavarSatirIndeks);
void canavarYonBelirle();	
void canavarYonHareketDegerleriniHesapla();
int InputaGoreIslemYap();
int pacManHareketEt(oyuncuBilgileri *oyuncuVerileri);
int yemOlustur();
void gameOverYaz(oyuncuBilgileri *oyuncuVerileri);	


int main() {
	
	while(1) {	
	
		oyuncuBilgileri *oyuncu = NULL;

		oyuncu = (oyuncuBilgileri *) malloc (sizeof(oyuncuBilgileri));	
	
		anaMenuOlustur();
			
		int anaMenuKomut = getch();
	
		if(anaMenuKomut == OyundanCikisYap || anaMenuKomut == OYUNDAN_CIKIS_YAP)
		{
			return 0;
		}
		
		if(anaMenuKomut == Kontroller || anaMenuKomut == KONTROLLER)
		{
			ekraniTemizle();
			
			kontrolTuslari();
			
			printf("\nAna menuye donmek icin herhangi bir tusa bas");
	
			getch();
		}

		if(anaMenuKomut == SkorlariYazdir || anaMenuKomut == SKORLARI_YAZDIR)	
		{
			ekraniTemizle();
		
			skorTablosunuYazdir(oyuncu);
		
			printf("\nAna menuye donmek icin herhangi bir tusa bas");
	
			getch();
		}
	
		if(anaMenuKomut == Oyna || anaMenuKomut == OYNA)
		{
	
			ekraniTemizle();	
	
			int canavarinPacmanleKarsilasmasi, pacmaninCanavarlaKarsilasmasi;  
			
			karakterleriOyunIcinHazirla();
			
			oyuncu->skor = 0;
		
			haritaTemizle();
		
			while(1) {
				
				srand(time(NULL));	
			
				set_cursor_position(0,0);
				hidecursor();
			
				haritaOlustur(oyuncu);
			
				if(yemKontrol == 0)
				{
					yemOlustur();
				}	
				
				canavarYonBelirle();
				canavarinPacmanleKarsilasmasi = canavarHareketEt();
				
				if(InputaGoreIslemYap()) {
					
					break;
				}
				
				pacmaninCanavarlaKarsilasmasi = pacManHareketEt(oyuncu);	
						
				if(pacmaninCanavarlaKarsilasmasi == 0 || canavarinPacmanleKarsilasmasi == 0){
			
					gameOverYaz(oyuncu);
				
					skorTablosunaKayitEkle(oyuncu);
					
					free(oyuncu);
				
					break;		
				}
	  		}			
		}	
	}	
}


void anaMenuOlustur() {
	
	ekraniTemizle();
	
	printf("*********************************************************\n");
	
	printf("PacMan v1 \n\n");
	printf("Oyuna baslamak icin e tusuna bas \n\n");
	printf("Kontrolleri goruntulemek icin k tusuna bas \n\n");
	printf("Skor tablosu icin w tusuna bas\n\n");
	printf("Oyundan cikmak icin q tusuna bas\n\n");
	
	printf("*********************************************************");
}


void kontrolTuslari() {
	
	printf("*********************************************************\n");
	
	printf("Pac-Man'i yukari hareket ettir: w \n\n");
	printf("Pac-Man'i asagi hareket ettir: s \n\n");
	printf("Pac-Man'i sola hareket ettir: a \n\n");
	printf("Pac-Man'i saga hareket ettir: d \n\n");
	printf("Oyun sirasinda ana menuye don: q \n\n");
	printf("Oyunu durdur/devam ettir: p \n\n");
	
	printf("*********************************************************");	
}


void karakterleriOyunIcinHazirla(){
	
		PacMan.pacMan.sutunIndeks = 15;
		PacMan.pacMan.satirIndeks = 17;
		PacMan.pacMan.satirYonundekiHareket = 0;
		PacMan.pacMan.sutunYonundekiHareket = 0;
	
		Canavar.canavar.sutunIndeks = 1;
		Canavar.canavar.satirIndeks = 1;
		Canavar.enKisaMesafe = 2000;	
}


void skorTablosunaKayitEkle(oyuncuBilgileri *oyuncuVerileri){
	
	printf("Oyuncu: "); 
	scanf(" %s", oyuncuVerileri->isim);
	
	if((pDosya = fopen("OyuncuBilgi.txt", "a+")) == NULL)
	{
		printf("Dosya Acilamadi !\n"); 
		exit(1);
	}
		
	if(fwrite(oyuncuVerileri, sizeof(bilgiler), 1, pDosya) != 1) {
		
		printf("Dosya Veri Ekleme Hatasi !");
		getch();
		exit(1);
	}
					
	fclose(pDosya);		
}


void skorTablosunuYazdir(oyuncuBilgileri *oyuncuVerileri){
	
	if((pDosya=fopen("OyuncuBilgi.txt", "a+")) == NULL)
	{
		printf("Dosya Acilamadi !\n"); 
		exit(1);
	}
			
	while(fread(oyuncuVerileri, sizeof(bilgiler), 1, pDosya) != 0)
	{
		printf("\nOyuncu: %s    | \t", oyuncuVerileri->isim);
		printf("Skoru: %d\n\n", oyuncuVerileri->skor);
		printf("-----------------------------------------\n");
	}
}


void haritaTemizle() {
	
	int i,j;
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			if(harita[i][j] == '@' || harita[i][j] == '&')
			harita[i][j] = ' ';	
		}
	}	
}
	
	
void haritaOlustur(oyuncuBilgileri *oyuncuVerileri) {
	
	usleep(100000);
	
	int i, j;
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", oyuncuVerileri->skor);
}


void gameOverYaz(oyuncuBilgileri *oyuncuVerileri){
	
	ekraniTemizle();
	
	printf("*********************************************************\n\n");
	
	printf("\t\t\t GAME OVER \n\n");
	
	printf("*********************************************************\n\n");
	
	printf("Skorunuz: %d \n\n", oyuncuVerileri->skor);
}


bool canavarYonKontrol(int canavarSutunIndeks, int canavarSatirIndeks) {
	
	bool hareketOnay = (harita[canavarSatirIndeks][canavarSutunIndeks] != '#') ? true : false;		
}


int canavarPacManArasiMesafeHesap(int canavarSutunIndeks, int canavarSatirIndeks) {
	
	int sutunUzakligi, satirUzakligi; 
	double Mesafe;
	
	sutunUzakligi = canavarSutunIndeks - PacMan.pacMan.sutunIndeks;
	satirUzakligi = canavarSatirIndeks - PacMan.pacMan.satirIndeks;
	
	Mesafe = sutunUzakligi * sutunUzakligi + satirUzakligi * satirUzakligi;
	
	if(Mesafe < Canavar.enKisaMesafe) {
		
		Canavar.enKisaMesafe = Mesafe;	
		Canavar.tempYon = Canavar.Yon;	
	} 
}


void canavarYonBelirle(){
	
	if(Canavar.canavar.yon != LEFT && canavarYonKontrol(Canavar.canavar.sutunIndeks + 1, Canavar.canavar.satirIndeks)) {
		
		Canavar.Yon = RIGHT;
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks + 1, Canavar.canavar.satirIndeks);	
	}
	
	if(Canavar.canavar.yon != RIGHT && canavarYonKontrol(Canavar.canavar.sutunIndeks - 1, Canavar.canavar.satirIndeks)) {
		
		Canavar.Yon = LEFT;
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks - 1, Canavar.canavar.satirIndeks);		
	}
	
	if(Canavar.canavar.yon != UP && canavarYonKontrol(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks + 1)) {
		
		Canavar.Yon = DOWN;
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks + 1);		
	}
	
	if(Canavar.canavar.yon != DOWN && canavarYonKontrol(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks - 1)) {
		
		Canavar.Yon = UP;
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks - 1);		
	}	
	
	Canavar.canavar.yon = Canavar.tempYon;
	Canavar.enKisaMesafe = 2000;	
}


void canavarYonHareketDegerleriniHesapla() {

	switch(Canavar.canavar.yon) {
		
	case UP:
		Canavar.canavar.sutunYonundekiHareket = 0;
		Canavar.canavar.satirYonundekiHareket = -1;
		break;
			
	case DOWN:
		Canavar.canavar.sutunYonundekiHareket = 0;
		Canavar.canavar.satirYonundekiHareket = 1;
		break;
		
	case LEFT:
		Canavar.canavar.sutunYonundekiHareket = -1;
		Canavar.canavar.satirYonundekiHareket = 0;
		break;
			
	case RIGHT:
		Canavar.canavar.sutunYonundekiHareket = 1;         
		Canavar.canavar.satirYonundekiHareket = 0;
		break;							
	}
}


int canavarHareketEt() {	

	canavarYonHareketDegerleriniHesapla();
		
	int sutunKontrol, satirKontrol;
		
	sutunKontrol = Canavar.canavar.sutunIndeks + Canavar.canavar.sutunYonundekiHareket;
	satirKontrol = Canavar.canavar.satirIndeks + Canavar.canavar.satirYonundekiHareket;
				
	if(!canavarYonKontrol(sutunKontrol, satirKontrol)) {
		
		Canavar.canavar.sutunYonundekiHareket = 0;
		Canavar.canavar.satirYonundekiHareket = 0;		
				
	} else if(harita[satirKontrol][sutunKontrol] == '@') {
			
		return 0;
		
	} else {
				
    	harita[Canavar.canavar.satirIndeks][Canavar.canavar.sutunIndeks] = YolUzerindekiKarakter;	
			
		YolUzerindekiKarakter = harita[satirKontrol][sutunKontrol];
			
		Canavar.canavar.sutunIndeks += Canavar.canavar.sutunYonundekiHareket;
		Canavar.canavar.satirIndeks += Canavar.canavar.satirYonundekiHareket;
		
		harita[Canavar.canavar.satirIndeks][Canavar.canavar.sutunIndeks] ='&';	
	}
}


int InputaGoreIslemYap() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = -1;
				return 0;
		  		
		  	case 'W':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = -1;
				return 0;

		  	case 's':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = +1;
				return 0;
		  		
		  	case 'S':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = +1;
				return 0;

		  	case 'a':
				PacMan.pacMan.sutunYonundekiHareket = -1;
				PacMan.pacMan.satirYonundekiHareket = 0;
				return 0;
		  		
		  	case 'A':
				PacMan.pacMan.sutunYonundekiHareket = -1;
				PacMan.pacMan.satirYonundekiHareket = 0;
				return 0;

		  	case 'd':
				PacMan.pacMan.sutunYonundekiHareket = +1;
				PacMan.pacMan.satirYonundekiHareket = 0;
				return 0;	
		  		
		  	case 'D':
				PacMan.pacMan.sutunYonundekiHareket = +1;
				PacMan.pacMan.satirYonundekiHareket = 0;
				return 0;
				
			case OyundanCikisYap:  
				return 1;
				
			case OYUNDAN_CIKIS_YAP:  
				return 1;
				
			case oyunuDurdur:
				getch();
				return 0;
			
			case OYUNU_DURDUR:
				getch();
				return 0;		
		  		
		  	default:
				return 0;
		}		
	}	
}


int pacManHareketEt(oyuncuBilgileri *oyuncuVerileri) {

	int sutunKontrol, satirKontrol;

	sutunKontrol = PacMan.pacMan.sutunIndeks + PacMan.pacMan.sutunYonundekiHareket;
	satirKontrol = PacMan.pacMan.satirIndeks + PacMan.pacMan.satirYonundekiHareket;

	if(harita[satirKontrol][sutunKontrol] == '#') {

		PacMan.pacMan.sutunYonundekiHareket = 0;
		PacMan.pacMan.satirYonundekiHareket = 0;	
	} 
	
	if(harita[satirKontrol][sutunKontrol] == '&') {
		
		return 0;				
	}
	
	else {
		
		harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] = ' ';
		
		PacMan.pacMan.sutunIndeks += PacMan.pacMan.sutunYonundekiHareket;
		PacMan.pacMan.satirIndeks += PacMan.pacMan.satirYonundekiHareket;
		
		if(harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] == '+') {
		
			oyuncuVerileri->skor += 1;
			yemKontrol = 0;				
		}
							
		harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] = '@';						
	}	
}


int yemOlustur() {
	
	int yemKonumX, yemKonumY; 
		
	yemKonumX = 1 + rand() % (W - 1); 
	
	yemKonumY = 1 + rand() % (H - 1); 
	
	if(harita[yemKonumY][yemKonumX]!= '#' && harita[yemKonumY][yemKonumX]!= '@' && harita[yemKonumY][yemKonumX]!= '&')
	{
		harita[yemKonumY][yemKonumX] = '+';
		yemKontrol++;
		
	}
	else
	{
		return yemOlustur();
	}
}
