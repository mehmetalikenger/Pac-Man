#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#define H 19
#define W 38


FILE *pdosya;

void ekraniTemizle();
void haritaTemizle();
void anaMenuOlustur();
void haritaOlustur();
void liderlikTablosunaKayitEkle();
void liderlikTablosuYazdir();
bool canavarYonKontrol(int canavarSutunIndeks, int canavarSatirIndeks);
int canavarPacManArasiMesafeHesap(int canavarSutunIndeks, int canavarSatirIndeks);
void canavarYonBelirle();	
void canavarYonHareketDegerleriniHesapla();
int canavarHareketEt();
void pacManHareketInput();
int pacManHareketEt();
int yemOlustur();
void gameOverYaz();		


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


enum anaMenuKomutlari{Oyna = 101, SkorlariYazdir = 119, CikisYap = 113};
enum anaMenuKomutlariCapsLock{OYNA = 69, SKORLARI_YAZDIR = 87, CIKIS_YAP = 81};
enum canavarYon{UP = 119, DOWN = 115, RIGHT = 100, LEFT = 97};


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


struct oyuncuBilgileri{
	
	int oyuncuNumarasi;
	char isim[20];
	int skor;	
	
}Oyuncu, oyuncuOkuma;

	
int yemKontrol = 0;


int main() {

	while(1)
	{	
		anaMenuOlustur();
			
		int anaMenuKomut = getch();
	
		if(anaMenuKomut == CikisYap || anaMenuKomut == CIKIS_YAP)
		{
			return 0;
		}

		if(anaMenuKomut == SkorlariYazdir || anaMenuKomut == SKORLARI_YAZDIR)	
		{
			ekraniTemizle();
		
			liderlikTablosuYazdir();
		
			printf("Ana menuye donmek icin herhangi bir tusa bas");
	
			getch();
		}
	
		if(anaMenuKomut == Oyna || anaMenuKomut == OYNA)
		{
	
			ekraniTemizle();	
	
			int x, y; 
			Oyuncu.skor = 0;
			
			PacMan.pacMan.sutunIndeks = 15;
			PacMan.pacMan.satirIndeks = 17;
			PacMan.pacMan.sutunYonundekiHareket = 0;
			PacMan.pacMan.satirYonundekiHareket = 0;
	
			Canavar.canavar.sutunIndeks = 1;
			Canavar.canavar.satirIndeks = 1;
		
			haritaTemizle();
		
			while(1) {
				
				srand(time(NULL));	
			
				set_cursor_position(0,0);
				hidecursor();
			
				haritaOlustur();
			
				if(yemKontrol == 0)
				{
					yemOlustur();
				}	
				
				canavarYonBelirle();
				y = canavarHareketEt();
				pacManHareketInput();
				x = pacManHareketEt();	
				
				if(x == 0 || y == 0){
			
					gameOverYaz();
				
					liderlikTablosunaKayitEkle();
				
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
	printf("Liderlik tablosu icin w tusuna bas\n\n");
	printf("Oyundan cikmak icin q tusuna bas\n\n");
	
	printf("*********************************************************");
}


void liderlikTablosunaKayitEkle(){
	
	
	if((pdosya=fopen("OyuncuBilgi.txt","ab+")) == NULL)
	{
		printf("dosya acilamadi...\n"); 
		exit(1);
	}
			
	fread(&oyuncuOkuma, sizeof(oyuncuOkuma), 1, pdosya);
			
	fclose(pdosya);
			
	if((pdosya = fopen("OyuncuBilgi.txt", "ab+")) == NULL)
	{
		printf("dosya acilamadi...");   
		exit(1);
	}
			
	else
	{
		Oyuncu.oyuncuNumarasi = oyuncuOkuma.oyuncuNumarasi + 1;
	}
			
	printf("Kullanici adi: "); 
	gets(Oyuncu.isim);
			
	fseek(pdosya, (Oyuncu.oyuncuNumarasi - 1)*sizeof(Oyuncu), SEEK_SET);
			
	if(fwrite(&Oyuncu, sizeof(Oyuncu), 1, pdosya)!= 1)
	{
		printf("Yazma Hatasi\n"); 
		exit(1);
	}
			
	rewind(pdosya);
							
	fclose(pdosya); 	
}


void liderlikTablosuYazdir(){
	
	if((pdosya=fopen("OyuncuBilgi.txt", "ab+")) == NULL)
	{
		printf("dosya acilamadi...\n"); 
		exit(1);
	}
			
	while(fread(&oyuncuOkuma, sizeof(oyuncuOkuma), 1, pdosya))
	{
		printf("Kullanici adi: %s\t", oyuncuOkuma.isim);
		printf("Skoru: %d\n", oyuncuOkuma.skor);
		printf("\n");
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
	
	
void haritaOlustur() {
	
	usleep(100000);
	
	int i, j;
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", Oyuncu.skor);
}


void gameOverYaz(){
	
	ekraniTemizle();
	
	printf("*********************************************************\n\n");
	
	printf("\t\t\t GAME OVER \n\n");
	
	printf("*********************************************************\n\n");
	
	printf("Skorunuz: %d \n\n", Oyuncu.skor);
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
	
	} else if(Mesafe == Canavar.enKisaMesafe){
		
		return 0;
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
	Canavar.enKisaMesafe = 1000;	
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


void pacManHareketInput() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = -1;
		  		break;

		  	case 's':
				PacMan.pacMan.sutunYonundekiHareket = 0;
				PacMan.pacMan.satirYonundekiHareket = +1;
		  		break;

		  	case 'a':
				PacMan.pacMan.sutunYonundekiHareket = -1;
				PacMan.pacMan.satirYonundekiHareket = 0;
		  		break;

		  	case 'd':
				PacMan.pacMan.sutunYonundekiHareket = +1;
				PacMan.pacMan.satirYonundekiHareket = 0;
		  		break;			  				  			
		}
	}		
}	


int pacManHareketEt() {

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
		
			Oyuncu.skor++;
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
