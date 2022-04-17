#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#define H 19
#define W 39

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

enum yon{UP = 119, DOWN = 115, RIGHT = 100, LEFT = 97};

typedef struct Hareket{
	
	int konumX;
	int konumY;
	int hareketX;
	int hareketY;
	int yon;	
} hareket;

struct CanavarBilgi{
	
	double enKisaMesafe;
	hareket canavar;
}Canavar;

struct PacManBilgi{
	
	hareket pacMan;	
}PacMan;

struct OyuncuBilgi {
	
	char isim[20];
	int skor;	
}Oyuncu;
	
int yem = 0;

	
void haritaOlustur() {
	
	usleep(200000);
	
	int i,j;
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", Oyuncu.skor);
}


bool CanavarYonKontrol(int x, int y) {
	
	if(harita[y][x] != '#' && harita[y][x] != '+'){
		
		return true;		
	
	} else {
		
		return false;		
	}	
}


int mesafeHesap(int koorX, int koorY, int yon) {
	
	int mesafeX, mesafeY; 
	double toplamMesafe;
	
	mesafeX = koorX - PacMan.pacMan.konumX;
	mesafeY = koorY - PacMan.pacMan.konumY;
	
	toplamMesafe = sqrt(mesafeX * mesafeX + mesafeY * mesafeY);
	
	if(toplamMesafe < Canavar.enKisaMesafe) {
		
		Canavar.enKisaMesafe = toplamMesafe;	
		Canavar.canavar.yon = yon;
	
	} else if(toplamMesafe == Canavar.enKisaMesafe){
		
		return 0;
	}
}


int canavar() {		
	
	int kontrolX, kontrolY;
	
	if(CanavarYonKontrol(Canavar.canavar.konumX+1, Canavar.canavar.konumY)) {
		
		mesafeHesap(Canavar.canavar.konumX+1, Canavar.canavar.konumY, RIGHT);	
	}
	
	if(CanavarYonKontrol(Canavar.canavar.konumX-1, Canavar.canavar.konumY)) {
		
		mesafeHesap(Canavar.canavar.konumX-1, Canavar.canavar.konumY, LEFT);		
	}
	
	if(CanavarYonKontrol(Canavar.canavar.konumX, Canavar.canavar.konumY+1)) {
		
		mesafeHesap(Canavar.canavar.konumX, Canavar.canavar.konumY+1, DOWN);		
	}
	
	if(CanavarYonKontrol(Canavar.canavar.konumX, Canavar.canavar.konumY-1)) {
		
		mesafeHesap(Canavar.canavar.konumX, Canavar.canavar.konumY-1, UP);		
	}	
	
	Canavar.enKisaMesafe = 100;
	
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
				
		if(!CanavarYonKontrol(kontrolX, kontrolY)) {
	
			Canavar.canavar.hareketX = 0;
			Canavar.canavar.hareketY = 0;
			Canavar.canavar.yon = 0;		
		}
		
		if(harita[kontrolY][kontrolX] == '@') {
		
			return 0;		
				
		} else {
				
    			harita[Canavar.canavar.konumY][Canavar.canavar.konumX] =' ';		
			
			Canavar.canavar.konumX += Canavar.canavar.hareketX;
			Canavar.canavar.konumY += Canavar.canavar.hareketY;
		
			harita[Canavar.canavar.konumY][Canavar.canavar.konumX] ='&';	
		}
}


int pacMan() {

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


void PacMan_hareket() {

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
		y = canavar();
		PacMan_hareket();
		x = pacMan();	
			
		if(x == 0 || y == 0) {
			
			return 0;		
		}
	}
}
