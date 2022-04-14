#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <unistd.h>

#define H 13
#define W 34

char harita[H][W] =
{
   { "#################################"},
   { "#                               #" },                             
   { "# ######### ######### ######### #" },                                               
   { "# ######### ######### ######### #" },       
   { "# ######### ######### ######### #" }, 
   { "# ######### ######### ######### #" },  
   { "#                               #" },
   { "# ######### ######### ######### #" },         
   { "# ######### ######### ######### #" },      
   { "# ######### ######### ######### #" },         
   { "# ######### ######### ######### #" },
   { "#                               #" },
   { "#################################" },
};

struct PacManBilgi{

	int konumX;
	int konumY;
	int hareketX;
	int hareketY;
	char yon;
	int skor;

} PacMan;

struct canavarBilgi{
	
	int konumX;
	int konumY;
	int hareketX;
	int hareketY;
	int yon;
	
}Canavar;

int yem = 0;

void haritaOlustur() {

	int i,j;

	system("cls");
		
	for(i = 0; i < H; i++) {

		for(j = 0; j < W; j++) {

			printf("%c", harita[i][j]);		
		}

		printf("\n");
	}	
	
	printf("Skor: %d\n", PacMan.skor);
}

int canavar() {
		
	harita[Canavar.konumY][Canavar.konumX] =' ';	
	
	if((Canavar.hareketX == 0 && Canavar.hareketY == 0) || 
	  ((Canavar.hareketY == 1 || Canavar.hareketY == -1) && (harita[Canavar.konumY][Canavar.konumX+1] != '#' || harita[Canavar.konumY][Canavar.konumX-1] != '#' )) || 
	  ((Canavar.hareketX == 1 || Canavar.hareketX == -1) && (harita[Canavar.konumY+1][Canavar.konumX] != '#' || harita[Canavar.konumY-1][Canavar.konumX] != '#' ))) 
	  {
	
		Canavar.yon = 1 + rand() % 5;    
	}
		
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
			Canavar.hareketX = 1;
			Canavar.hareketY = 0;
			break;
			
		case 4:
			Canavar.hareketX = -1;         
			Canavar.hareketY = 0;
			break;				
			
		}
					
		int kontrolX = Canavar.konumX + Canavar.hareketX;
		int kontrolY = Canavar.konumY + Canavar.hareketY; 
					
		if((harita[kontrolY][kontrolX] == '#') || (harita[kontrolY][kontrolX] == '+')) {
	
			Canavar.hareketX = 0;
			Canavar.hareketY = 0;		
		}	
		
		if(harita[kontrolY][kontrolX] == '@') {
		
		return 0;		
				
		} else {
		
				
		Canavar.konumX += Canavar.hareketX;
		Canavar.konumY += Canavar.hareketY;
		
	}
				
		harita[Canavar.konumY][Canavar.konumX] ='&';	
}

int pacMan() {

	harita[PacMan.konumY][PacMan.konumX] = ' ';

	int kontrolX, kontrolY;

	kontrolX = PacMan.konumX + PacMan.hareketX;
	kontrolY = PacMan.konumY + PacMan.hareketY;

	if(harita[kontrolY][kontrolX] == '#') {

		PacMan.hareketX = 0;
		PacMan.hareketY = 0;	

	} 
	
	if(harita[kontrolY][kontrolX] == '&' /*|| harita[kontrolY][kontrolX] == '#'*/) {
		
		return 0;				
	}
	
	else {

		PacMan.konumX += PacMan.hareketX;
		PacMan.konumY += PacMan.hareketY;						
	}
	
	if(harita[PacMan.konumY][PacMan.konumX] == '+') {
		
		PacMan.skor++;
		yem = 0;
				
	}
	
	harita[PacMan.konumY][PacMan.konumX] = '@';

}

void PacMan_hareket() {

	if(kbhit()){

		switch(getch()) {

			case 'w':
				PacMan.yon = 'w';
		  		break;

		  	case 's':
				PacMan.yon = 's';
		  		break;

		  	case 'a':
				PacMan.yon = 'a';
		  		break;

		  	case 'd':
				PacMan.yon= 'd';
		  		break;				
		}
	}

		switch(PacMan.yon) {

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

main() {

	int x, y;

	PacMan.konumX = 16;
	PacMan.konumY = 11;
	PacMan.hareketX = 0;
	PacMan.hareketY = 0;
	PacMan.skor = 0;
	
	Canavar.konumX = 1;
	Canavar.konumY = 1;
	Canavar.hareketX = 0;
	Canavar.hareketY = 0;
	Canavar.yon = 0;

	while(1) {

		usleep(300000);
		haritaOlustur();
		y = canavar();
		x = pacMan();
		PacMan_hareket();
		
		if(x == 0 || y == 0) {
			
			return 0;
			
		}
	}
}
