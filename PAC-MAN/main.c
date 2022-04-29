#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#define H 19
#define W 39
	
	
void haritaOlustur();
bool canavarYonKontrol(int x, int y);
int canavarMesafeHesap(int koorX, int koorY, int yon);
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


typedef struct Hareket{
	
	int konumX;
	int konumY;
	int hareketX;
	int hareketY;
	int yon;	
} hareket;


struct canavarBilgi{
	
	int tempYon;
	double enKisaMesafe;
	hareket canavar;
}Canavar;


struct pacManBilgi{
	
	hareket pacMan;	
}PacMan;


struct oyuncuBilgi {
	
	char isim[20];
	int skor;	
}Oyuncu;

	
int yem = 0;


int main() {	
		
	int x, y; 
	Oyuncu.skor = 0;
			
	PacMan.pacMan.konumX = 15;
	PacMan.pacMan.konumY = 17;
	PacMan.pacMan.hareketX = 0;
	PacMan.pacMan.hareketY = 0;
	
	Canavar.canavar.konumX = 1;
	Canavar.canavar.konumY = 1;
	
	system("cls");	
	
	while(1) {
		
		set_cursor_position(0,0);
		hidecursor();
		
		haritaOlustur();		
		y = canavarHareketEt();
		canavarYonBelirle();
		pacManHareketInput();
		x = pacManHareketEt();	
			
		if(x == 0 || y == 0) {
			
			return 0;		
		}
	}
}

	
void haritaOlustur() {
	
	usleep(100000);
	
	int i,j;
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", Oyuncu.skor);
}


bool canavarYonKontrol(int x, int y) {
	
	if(harita[y][x] != '#' && harita[y][x] != '+'){
		
		return true;		
	
	} else {
		
		return false;		
	}	
}


int canavarMesafeHesap(int koorX, int koorY, int yon) {
	
	int mesafeX, mesafeY; 
	double Mesafe;
	
	mesafeX = koorX - PacMan.pacMan.konumX;
	mesafeY = koorY - PacMan.pacMan.konumY;
	
	Mesafe = mesafeX * mesafeX + mesafeY * mesafeY;
	
	if(Mesafe < Canavar.enKisaMesafe) {
		
		Canavar.enKisaMesafe = Mesafe;	
		Canavar.tempYon = yon;
	
	} else if(Mesafe == Canavar.enKisaMesafe){
		
		return 0;
	}
}


void canavarYonBelirle(){
	
	if(Canavar.canavar.yon != LEFT && canavarYonKontrol(Canavar.canavar.konumX+1, Canavar.canavar.konumY)) {
		
		canavarMesafeHesap(Canavar.canavar.konumX+1, Canavar.canavar.konumY, RIGHT);	
	}
	
	if(Canavar.canavar.yon != RIGHT && canavarYonKontrol(Canavar.canavar.konumX-1, Canavar.canavar.konumY)) {
		
		canavarMesafeHesap(Canavar.canavar.konumX-1, Canavar.canavar.konumY, LEFT);		
	}
	
	if(Canavar.canavar.yon != UP && canavarYonKontrol(Canavar.canavar.konumX, Canavar.canavar.konumY+1)) {
		
		canavarMesafeHesap(Canavar.canavar.konumX, Canavar.canavar.konumY+1, DOWN);		
	}
	
	if(Canavar.canavar.yon != DOWN && canavarYonKontrol(Canavar.canavar.konumX, Canavar.canavar.konumY-1)) {
		
		canavarMesafeHesap(Canavar.canavar.konumX, Canavar.canavar.konumY-1, UP);		
	}	
	
	Canavar.canavar.yon = Canavar.tempYon;
	Canavar.enKisaMesafe = 1000;	
}


int canavarHareketEt() {		
		
	int kontrolX, kontrolY;
	
	switch(Canavar.canavar.yon) {
		
		case UP:
			Canavar.canavar.hareketX = 0;
			Canavar.canavar.hareketY = -1;
			break;
			
		case DOWN:
			Canavar.canavar.hareketX = 0;
			Canavar.canavar.hareketY = 1;
			break;
		
		case LEFT:
			Canavar.canavar.hareketX = -1;
			Canavar.canavar.hareketY = 0;
			break;
			
		case RIGHT:
			Canavar.canavar.hareketX = 1;         
			Canavar.canavar.hareketY = 0;
			break;							
		}
		
		kontrolX = Canavar.canavar.konumX + Canavar.canavar.hareketX;
		kontrolY = Canavar.canavar.konumY + Canavar.canavar.hareketY;
				
		if(!canavarYonKontrol(kontrolX, kontrolY)) {
		
			Canavar.canavar.hareketX = 0;
			Canavar.canavar.hareketY = 0;		
				
		} else {
				
    		harita[Canavar.canavar.konumY][Canavar.canavar.konumX] =' ';		
			
			Canavar.canavar.konumX += Canavar.canavar.hareketX;
			Canavar.canavar.konumY += Canavar.canavar.hareketY;
		
			harita[Canavar.canavar.konumY][Canavar.canavar.konumX] ='&';	
		}
}


void pacManHareketInput() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.pacMan.hareketX = 0;
				PacMan.pacMan.hareketY = -1;
		  		break;
		  		
		  	case 'W':
				PacMan.pacMan.hareketX = 0;
				PacMan.pacMan.hareketY = -1;
		  		break;

		  	case 's':
				PacMan.pacMan.hareketX = 0;
				PacMan.pacMan.hareketY = +1;
		  		break;
		  		
		  	case 'S':
				PacMan.pacMan.hareketX = 0;
				PacMan.pacMan.hareketY = -1;
		  		break;

		  	case 'a':
				PacMan.pacMan.hareketX = -1;
				PacMan.pacMan.hareketY = 0;
		  		break;
		  		
		  	case 'A':
				PacMan.pacMan.hareketX = -1;
				PacMan.pacMan.hareketY = 0;
		  		break;

		  	case 'd':
				PacMan.pacMan.hareketX = +1;
				PacMan.pacMan.hareketY = 0;
		  		break;	
		  		
		  	case 'D':
				PacMan.pacMan.hareketX = +1;
				PacMan.pacMan.hareketY = 0;
		  		break;		  			
		}
	}		
}	


int pacManHareketEt() {

	int kontrolX, kontrolY;

	kontrolX = PacMan.pacMan.konumX + PacMan.pacMan.hareketX;
	kontrolY = PacMan.pacMan.konumY + PacMan.pacMan.hareketY;

	if(harita[kontrolY][kontrolX] == '#') {

		PacMan.pacMan.hareketX = 0;
		PacMan.pacMan.hareketY = 0;	
	} 
	
	if(harita[kontrolY][kontrolX] == '&') {
		
		return 0;				
	}
	
	else {
		
		harita[PacMan.pacMan.konumY][PacMan.pacMan.konumX] = ' ';
		
		PacMan.pacMan.konumX += PacMan.pacMan.hareketX;
		PacMan.pacMan.konumY += PacMan.pacMan.hareketY;
		
		if(harita[PacMan.pacMan.konumY][PacMan.pacMan.konumX] == '+') {
		
			Oyuncu.skor++;
			yem = 0;				
		}
							
		harita[PacMan.pacMan.konumY][PacMan.pacMan.konumX] = '@';						
	}	
}
