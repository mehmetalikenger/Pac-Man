#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
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

struct Bilgi{
	
	int konumX;
	int konumY;
	int hareketX;
	int hareketY;
	int yon;	
}Canavar, PacMan;
		
	int skor=0;
	int yem = 0;
	int yon_temp = 1;
	
//FONKSÝYONLAR

//RANDOM SAYI ÜRETÝCÝ
int randnum(int min, int max)
	{		
		int dizi[2] = {min,max}, i;
			
		i = rand() % 2;
			
		return dizi[i];
	}

//HARÝTA OLUÞTURMA FONKSÝYONU	
	void haritaOlustur() {
	
	usleep(200000);
	
	int i,j;

	system("cls");
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", skor);
}

//CANAVAR FONKSÝYONU
int  canavar() {	
	
		int n,m;
		
		int kontrolY = Canavar.konumY; 
		int kontrolX = Canavar.konumX;
		
		if((yon_temp == 1) && ((harita[kontrolY][kontrolX-1] != '#') && (harita[kontrolY][kontrolX-1] != '+') ) ){
				
			n = 1;
			m = 3;		
			Canavar.yon = randnum(n, m);
		}			
		
		if((yon_temp == 1) && ((harita[kontrolY][kontrolX+1] != '#') && (harita[kontrolY][kontrolX+1] != '+') ) ){
				
			n = 1;
			m = 4;		
			Canavar.yon = randnum(n, m);
		}
		
		if((yon_temp == 2) && ((harita[kontrolY][kontrolX-1] != '#') && (harita[kontrolY][kontrolX-1] != '+') ) ){
			
			n = 2;
			m = 3;		
			Canavar.yon = randnum(n, m);
		}
		
		if((yon_temp == 2) && ((harita[kontrolY][kontrolX+1] != '#') && (harita[kontrolY][kontrolX+1] != '+') ) ){
			
			n = 2;
			m = 4;		
			Canavar.yon = randnum(n, m);
		}
				
		if((yon_temp == 3) && ((harita[kontrolY-1][kontrolX] != '#') && (harita[kontrolY-1][kontrolX] != '+') ) ){
			
			n = 1;
			m = 3;	
			Canavar.yon = randnum(n, m);
		}
				
		if((yon_temp == 3) && ((harita[kontrolY+1][kontrolX] != '#') && (harita[kontrolY+1][kontrolX] != '+') ) ){
			
			n = 2;
			m = 3;		
			Canavar.yon = randnum(n, m);
		}
			
		if((yon_temp == 4) && ((harita[kontrolY-1][kontrolX] != '#') && (harita[kontrolY-1][kontrolX] != '+') ) ){
			
			n = 1;
			m = 4;		
			Canavar.yon = randnum(n, m);
		}
				
		if((yon_temp == 4) && ((harita[kontrolY+1][kontrolX] != '#') && (harita[kontrolY+1][kontrolX] != '+') ) ){
			
			n = 2;
			m = 4;		
			Canavar.yon = randnum(n, m);
		}
					
		yon_temp = Canavar.yon;	
		
		switch(Canavar.yon) {
		
		case 1:
			Canavar.hareketX = 0;
			Canavar.hareketY = -1;
			break;
			
		case 2:
			Canavar.hareketX = 0;
			Canavar.hareketY = 1;
			break;
		
		case 3:
			Canavar.hareketX = -1;
			Canavar.hareketY = 0;
			break;
			
		case 4:
			Canavar.hareketX = 1;         
			Canavar.hareketY = 0;
			break;							
		}
		
		kontrolX = Canavar.konumX + Canavar.hareketX;
		kontrolY = Canavar.konumY + Canavar.hareketY;
				
		if((harita[kontrolY][kontrolX] == '#') || (harita[kontrolY][kontrolX] == '+')) {
	
			Canavar.hareketX = 0;
			Canavar.hareketY = 0;
			Canavar.yon = 0;		
		}
		
		if(harita[kontrolY][kontrolX] == '@') {
		
		return 0;		
				
		} else {
				
    	harita[Canavar.konumY][Canavar.konumX] =' ';		
		
		Canavar.konumX += Canavar.hareketX;
		Canavar.konumY += Canavar.hareketY;
		
		harita[Canavar.konumY][Canavar.konumX] ='&';			
	}	
}

//PAC-MAN FONKSÝYONU
int pacMan() {

	int kontrolX, kontrolY;

	kontrolX = PacMan.konumX + PacMan.hareketX;
	kontrolY = PacMan.konumY + PacMan.hareketY;

	if(harita[kontrolY][kontrolX] == '#') {

		PacMan.hareketX = 0;
		PacMan.hareketY = 0;	
	} 
	
	if(harita[kontrolY][kontrolX] == '&') {
		
		return 0;				
	}
	
	else {
		
		harita[PacMan.konumY][PacMan.konumX] = ' ';
		
		PacMan.konumX += PacMan.hareketX;
		PacMan.konumY += PacMan.hareketY;
		
		if(harita[PacMan.konumY][PacMan.konumX] == '+') {
		
		skor++;
		yem = 0;				
		}
							
		harita[PacMan.konumY][PacMan.konumX] = '@';						
	}	
}

//PAC-MAN HAREKET INPUT FONKSÝYONU
void PacMan_hareket() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.hareketX = 0;
				PacMan.hareketY = -1;
		  		break;

		  	case 's':
				PacMan.hareketX = 0;
				PacMan.hareketY = +1;
		  		break;

		  	case 'a':
				PacMan.hareketX = -1;
				PacMan.hareketY = 0;
		  		break;

		  	case 'd':
				PacMan.hareketX = +1;
				PacMan.hareketY = 0;
		  		break;				
		}
	}		
}		

//MAIN FONKSÝYONU
int main() {

	int x, y;
	
	PacMan.konumX = 15;
	PacMan.konumY = 17;
	PacMan.hareketX = 0;
	PacMan.hareketY = 0;
	
	Canavar.konumX = 10;
	Canavar.konumY = 11;
	Canavar.hareketX = 0;
	Canavar.hareketY = 0;
	Canavar.yon = 0;

	while(1) {

		haritaOlustur();
		y = canavar();
		x = pacMan();
		PacMan_hareket();
		
		if(x == 0 || y == 0) {
			
			return 0;		
		}
	}
}
