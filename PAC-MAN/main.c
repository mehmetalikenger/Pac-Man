#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#define H 19
#define W 39
	
FILE *pdosya;

void liderlikTablosunaKayitEkle();
void liderlikTablosuYazdir();
void ekraniTemizle();
void gameOverYaz();	
void anaMenuOlustur();	
void haritaTemizle();
void haritaOlustur();
int yemOlustur();	
void haritaOlustur();
bool canavarYonKontrol(int x, int y);
int canavarPacManArasiMesafeHesap(int koorX, int koorY, int yon);
void canavarYonBelirle();		
int canavarHareketEt();
void pacManHareketInput();
int pacManHareketEt();


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


enum canavarYon{UP = 119, DOWN = 115, RIGHT = 100, LEFT = 97};

char YolUzerindekiKarakter = ' ';

typedef struct Hareket{
	
	int sutunIndeks;
	int satirIndeks;
	int sutunYonundeHareket;
	int satirYonundeHareket;
	int yon;	
} hareket;


struct canavarBilgileri{
	
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
	
		int anaMenuKomutAl = getch();
	
		if(anaMenuKomutAl == 'q' || anaMenuKomutAl == 'Q')
		{
			return 0;
		}
		
		if(anaMenuKomutAl == 'w' || anaMenuKomutAl == 'W')	
		{
			ekraniTemizle();
		
			liderlikTablosuYazdir();
		
			printf("Ana menuye donmek icin herhangi bir tusa bas");
	
			getch();
		}
	
		if(anaMenuKomutAl == 'e'||anaMenuKomutAl == 'E')
		{
	
			ekraniTemizle();	
	
			int x, y; 
			Oyuncu.skor = 0;
			
			PacMan.pacMan.sutunIndeks = 15;
			PacMan.pacMan.satirIndeks = 17;
			PacMan.pacMan.sutunYonundeHareket = 0;
			PacMan.pacMan.satirYonundeHareket = 0;
	
			Canavar.canavar.sutunIndeks = 1;
			Canavar.canavar.satirIndeks = 1;
		
			haritaTemizle();
		
			while(1) {
			
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
		printf("dosya acilamady...\n"); 
		exit(1);
	}
			
	while(fread(&oyuncuOkuma,sizeof(oyuncuOkuma), 1, pdosya))
	{
		printf("Kullanici adi: %s\t", oyuncuOkuma.isim);
		printf("Skoru: %d\n", oyuncuOkuma.skor);
		printf("\n");
	}
}


void haritaTemizle() {
	
	int i, j;
		
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
	
	system("cls");
	
	printf("*********************************************************\n\n");
	
	printf("\t\t\t GAME OVER \n\n");
	
	printf("*********************************************************\n\n");
	
	printf("Skorunuz: %d \n\n", Oyuncu.skor);
}


void anaMenuOlustur() {
	
	system("cls");
	
	printf("*********************************************************\n");
	
	printf("PacMan v1 \n\n");
	printf("Oyuna baslamak icin e tusuna bas \n\n");
	printf("Liderlik tablosu icin w tusuna bas\n\n");
	printf("Oyundan cikmak icin q tusuna bas\n\n");
	
	printf("*********************************************************");
}


void ekraniTemizle(){
	
	system("cls");
}


bool canavarYonKontrol(int canavarSutunIndeks, int canavarSatirIndeks) {
	
	if(harita[canavarSatirIndeks][canavarSutunIndeks] != '#'){
		
		return true;		
	
	} else {
		
		return false;		
	}	
}


int canavarPacManArasiMesafeHesap(int canavarSutunIndeks, int canavarSatirIndeks, int yon) {
	
	int sutunUzakligi, satirUzakligi; 
	double Mesafe;
	
	sutunUzakligi = canavarSutunIndeks - PacMan.pacMan.sutunIndeks;
	satirUzakligi = canavarSatirIndeks - PacMan.pacMan.satirIndeks;
	
	Mesafe = sutunUzakligi * sutunUzakligi + satirUzakligi * satirUzakligi;
	
	if(Mesafe < Canavar.enKisaMesafe) {
		
		Canavar.enKisaMesafe = Mesafe;	
		Canavar.tempYon = yon;
	
	} else if(Mesafe == Canavar.enKisaMesafe){
		
		return 0;
	}
}


void canavarYonBelirle(){
	
	if(Canavar.canavar.yon != LEFT && canavarYonKontrol(Canavar.canavar.sutunIndeks + 1, Canavar.canavar.satirIndeks)) {
		
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks + 1, Canavar.canavar.satirIndeks, RIGHT);	
	}
	
	if(Canavar.canavar.yon != RIGHT && canavarYonKontrol(Canavar.canavar.sutunIndeks - 1, Canavar.canavar.satirIndeks)) {
		
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks - 1, Canavar.canavar.satirIndeks, LEFT);		
	}
	
	if(Canavar.canavar.yon != UP && canavarYonKontrol(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks + 1)) {
		
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks + 1, DOWN);		
	}
	
	if(Canavar.canavar.yon != DOWN && canavarYonKontrol(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks - 1)) {
		
		canavarPacManArasiMesafeHesap(Canavar.canavar.sutunIndeks, Canavar.canavar.satirIndeks - 1, UP);		
	}	
	
	Canavar.canavar.yon = Canavar.tempYon;
	Canavar.enKisaMesafe = 1000;	
}


int canavarHareketEt() {		
		
	int sutunKontrol, satirKontrol;
	
	switch(Canavar.canavar.yon) {
		
		case UP:
			Canavar.canavar.sutunYonundeHareket = 0;
			Canavar.canavar.satirYonundeHareket = -1;
			break;
			
		case DOWN:
			Canavar.canavar.sutunYonundeHareket = 0;
			Canavar.canavar.satirYonundeHareket = 1;
			break;
		
		case LEFT:
			Canavar.canavar.sutunYonundeHareket = -1;
			Canavar.canavar.satirYonundeHareket = 0;
			break;
			
		case RIGHT:
			Canavar.canavar.sutunYonundeHareket = 1;         
			Canavar.canavar.satirYonundeHareket = 0;
			break;							
		}
		
		sutunKontrol = Canavar.canavar.sutunIndeks + Canavar.canavar.sutunYonundeHareket;
		satirKontrol = Canavar.canavar.satirIndeks + Canavar.canavar.satirYonundeHareket;
				
		if(!canavarYonKontrol(sutunKontrol, satirKontrol)) {
		
			Canavar.canavar.sutunYonundeHareket = 0;
			Canavar.canavar.satirYonundeHareket = 0;		
				
		} else if(harita[satirKontrol][sutunKontrol] == '@') {
			
			return 0;
		
		} else {
				
    		harita[Canavar.canavar.satirIndeks][Canavar.canavar.sutunIndeks] = YolUzerindekiKarakter;	
			
			YolUzerindekiKarakter = harita[satirKontrol][sutunKontrol];
			
			Canavar.canavar.sutunIndeks += Canavar.canavar.sutunYonundeHareket;
			Canavar.canavar.satirIndeks += Canavar.canavar.satirYonundeHareket;
		
			harita[Canavar.canavar.satirIndeks][Canavar.canavar.sutunIndeks] ='&';	
		}
}


void pacManHareketInput() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.pacMan.sutunYonundeHareket = 0;
				PacMan.pacMan.satirYonundeHareket = -1;
		  		break;
		  		
		  	case 'W':
				PacMan.pacMan.sutunYonundeHareket = 0;
				PacMan.pacMan.satirYonundeHareket = -1;
		  		break;

		  	case 's':
				PacMan.pacMan.sutunYonundeHareket = 0;
				PacMan.pacMan.satirYonundeHareket = +1;
		  		break;
		  		
		  	case 'S':
				PacMan.pacMan.sutunYonundeHareket = 0;
				PacMan.pacMan.satirYonundeHareket = -1;
		  		break;

		  	case 'a':
				PacMan.pacMan.sutunYonundeHareket = -1;
				PacMan.pacMan.satirYonundeHareket = 0;
		  		break;
		  		
		  	case 'A':
				PacMan.pacMan.sutunYonundeHareket = -1;
				PacMan.pacMan.satirYonundeHareket = 0;
		  		break;

		  	case 'd':
				PacMan.pacMan.sutunYonundeHareket = +1;
				PacMan.pacMan.satirYonundeHareket = 0;
		  		break;	
		  		
		  	case 'D':
				PacMan.pacMan.sutunYonundeHareket = +1;
				PacMan.pacMan.satirYonundeHareket = 0;
		  		break;		  			
		}
	}		
}	


int yemOlustur() {
	
	int yemKonumX, yemKonumY;
	
	//srand(time(NULL));	
		
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


int pacManHareketEt() {

	int sutunKontrol, satirKontrol;

	sutunKontrol = PacMan.pacMan.sutunIndeks + PacMan.pacMan.sutunYonundeHareket;
	satirKontrol = PacMan.pacMan.satirIndeks + PacMan.pacMan.satirYonundeHareket;

	if(harita[satirKontrol][sutunKontrol] == '#') {

		PacMan.pacMan.sutunYonundeHareket = 0;
		PacMan.pacMan.satirYonundeHareket = 0;	
	} 
	
	if(harita[satirKontrol][sutunKontrol] == '&') {
		
		return 0;				
	}
	
	else {
		
		harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] = ' ';
		
		PacMan.pacMan.sutunIndeks += PacMan.pacMan.sutunYonundeHareket;
		PacMan.pacMan.satirIndeks += PacMan.pacMan.satirYonundeHareket;
		
		if(harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] == '+') {
		
			Oyuncu.skor++;
			yemKontrol = 0;				
		}
							
		harita[PacMan.pacMan.satirIndeks][PacMan.pacMan.sutunIndeks] = '@';						
	}	
}
