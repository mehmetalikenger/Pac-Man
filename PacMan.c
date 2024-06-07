#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <conio.h>
#define H 19
#define W 38


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


void clearScreen() {
    
    system("cls");
}


char map[H][W] =
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


enum mainMenuCommands {Play = 101, Controls = 107, PrintScores = 119, ExitGame = 113};
enum mainMenuCommandsCapsLock {PLAY = 69, CONTROLS = 75, PRINT_SCORES = 87, EXIT_GAME = 81};
enum gamePause {pauseGame = 112};
enum gamePauseCapsLock {PAUSE_GAME = 80};
enum monsterDirection {UP = 119, DOWN = 115, RIGHT = 100, LEFT = 97};


char CharacterOnPath = ' ';


typedef struct Movement{
    
    int columnIndex;
    int rowIndex;
    int columnMovement;
    int rowMovement;
    int direction;
        
} movement;


struct monsterInfo{
    
    int Direction;
    int tempDirection;
    double shortestDistance;
    movement monster;
    
}Monster;


struct pacManInfo{
    
    movement pacMan;  
    
}PacMan;


typedef struct{
    
    char name[10];
    int score;  
    
}playerInfo;


playerInfo info;
int foodControl = 0;
FILE *pDosya;  


void clearScreen();
void clearMap();
void createMainMenu();
void controlKeys();
void prepareCharactersForGame();
void createMap(playerInfo *playerData);
void addRecordToScoreboard();
void printScoreboard(playerInfo *playerData);
bool checkMonsterDirection(int monsterColumnIndex, int monsterRowIndex);
int calculateMonsterPacManDistance(int monsterColumnIndex, int monsterRowIndex);
void determineMonsterDirection();    
void calculateMonsterMovementValues();
int moveMonster();
int performActionBasedOnInput();
int movePacMan(playerInfo *playerData);
int createFood();
void writeGameOver(playerInfo *playerData);  


int main() {
    
    while(1) {  
    
        playerInfo *player = NULL;

        player = (playerInfo *) malloc (sizeof(playerInfo));  
    
        createMainMenu();
            
        int mainMenuCommand = getch();
    
        if(mainMenuCommand == ExitGame || mainMenuCommand == EXIT_GAME)
        {
            return 0;
        }
        
        if(mainMenuCommand == Controls || mainMenuCommand == CONTROLS)
        {
            clearScreen();
            
            controlKeys();
            
            printf("\nPress any key to return to the main menu");
    
            getch();
        }

        if(mainMenuCommand == PrintScores || mainMenuCommand == PRINT_SCORES)    
        {
            clearScreen();
        
            printScoreboard(player);
        
            printf("\nPress any key to return to the main menu");
    
            getch();
        }
    
        if(mainMenuCommand == Play || mainMenuCommand == PLAY)
        {
    
            clearScreen();    
    
            int monsterEncounterWithPacman, pacmanEncounterWithMonster;  
            
            prepareCharactersForGame();
            
            player->score = 0;
        
            clearMap();
        
            while(1) {
                
                srand(time(NULL));    
            
                set_cursor_position(0,0);
                hidecursor();
            
                createMap(player);
            
                if(foodControl == 0)
                {
                    createFood();
                }    
                
                determineMonsterDirection();
                monsterEncounterWithPacman = moveMonster();
                
                if(performActionBasedOnInput()) {
                    
                    break;
                }
                
                pacmanEncounterWithMonster = movePacMan(player);    
                        
                if(pacmanEncounterWithMonster == 0 || monsterEncounterWithPacman == 0){
            
                    writeGameOver(player);
                
                    addRecordToScoreboard(player);
                    
                    free(player);
                
                    break;        
                }
            }            
        }    
    }    
}


void createMainMenu() {
    
    clearScreen();
    
    printf("*********************************************************\n");
    
    printf("PacMan v1 \n\n");
    printf("Press e to start the game \n\n");
    printf("Press k to view controls \n\n");
    printf("Press w for the scoreboard\n\n");
    printf("Press q to exit the game\n\n");
    
    printf("*********************************************************");
}


void controlKeys() {
    
    printf("*********************************************************\n");
    
    printf("Move Pac-Man up: w \n\n");
    printf("Move Pac-Man down: s \n\n");
    printf("Move Pac-Man left: a \n\n");
    printf("Move Pac-Man right: d \n\n");
    printf("Return to main menu during the game: q \n\n");
    printf("Pause/resume game: p \n\n");
    
    printf("*********************************************************");    
}


void prepareCharactersForGame(){
    
        PacMan.pacMan.columnIndex = 15;
        PacMan.pacMan.rowIndex = 17;
        PacMan.pacMan.rowMovement = 0;
        PacMan.pacMan.columnMovement = 0;
    
        Monster.monster.columnIndex = 1;
        Monster.monster.rowIndex = 1;
        Monster.shortestDistance = 2000;    
}


void addRecordToScoreboard(playerInfo *playerData){
    
    printf("Player: "); 
    scanf(" %s", playerData->name);
    
    if((pDosya = fopen("PlayerInfo.txt", "a+")) == NULL)
    {
        printf("File Could Not Be Opened!\n"); 
        exit(1);
    }
        
    if(fwrite(playerData, sizeof(info), 1, pDosya) != 1) {
        
        printf("File Data Addition Error!");
        getch();
        exit(1);
    }
                    
    fclose(pDosya);        
}


void printScoreboard(playerInfo *playerData){
    
    if((pDosya=fopen("PlayerInfo.txt", "a+")) == NULL)
    {
        printf("File Could Not Be Opened!\n"); 
        exit(1);
    }
            
    while(fread(playerData, sizeof(info), 1, pDosya) != 0)
    {
        printf("\nPlayer: %s    | \t", playerData->name);
        printf("Score: %d\n\n", playerData->score);
        printf("-----------------------------------------\n");
    }
}


void clearMap() {
    
    int i,j;
        
    for(i = 0; i < H; i++) {

        for(j = 0; j < W; j++) {

            if(map[i][j] == '@' || map[i][j] == '&')
            map[i][j] = ' ';    
        }
    }    
}
    
    
void createMap(playerInfo *playerData) {
    
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = 100000000L;  // 100 milliseconds
    nanosleep(&req, NULL);
    
    int i, j;
        
    for(i = 0; i < H; i++) {

        for(j = 0; j < W; j++) {

            printf("%c", map[i][j]);        
        }

        printf("\n");
    }    
    
    printf("Score: %d\n", playerData->score);
}


void writeGameOver(playerInfo *playerData){
    
    clearScreen();
    
    printf("*********************************************************\n\n");
    
    printf("\t\t\t GAME OVER \n\n");
    
    printf("*********************************************************\n\n");
    
    printf("Your Score: %d \n\n", playerData->score);
}


bool checkMonsterDirection(int monsterColumnIndex, int monsterRowIndex) {
    
    bool movementApproval = (map[monsterRowIndex][monsterColumnIndex] != '#') ? true : false;        
}


int calculateMonsterPacManDistance(int monsterColumnIndex, int monsterRowIndex) {

    int columnDistance, rowDistance;
    double distance;

    columnDistance = monsterColumnIndex - PacMan.pacMan.columnIndex;
    rowDistance = monsterRowIndex - PacMan.pacMan.rowIndex;

    distance = columnDistance * columnDistance + rowDistance * rowDistance;

    if (distance < Monster.shortestDistance) {

        Monster.shortestDistance = distance;
        Monster.tempDirection = Monster.Direction;
    }
}


void determineMonsterDirection() {

    if (Monster.monster.direction != LEFT && checkMonsterDirection(Monster.monster.columnIndex + 1, Monster.monster.rowIndex)) {
        Monster.Direction = RIGHT;
        calculateMonsterPacManDistance(Monster.monster.columnIndex + 1, Monster.monster.rowIndex);
    }

    if (Monster.monster.direction != RIGHT && checkMonsterDirection(Monster.monster.columnIndex - 1, Monster.monster.rowIndex)) {
        Monster.Direction = LEFT;
        calculateMonsterPacManDistance(Monster.monster.columnIndex - 1, Monster.monster.rowIndex);
    }

    if (Monster.monster.direction != UP && checkMonsterDirection(Monster.monster.columnIndex, Monster.monster.rowIndex + 1)) {
        Monster.Direction = DOWN;
        calculateMonsterPacManDistance(Monster.monster.columnIndex, Monster.monster.rowIndex + 1);
    }

    if (Monster.monster.direction != DOWN && checkMonsterDirection(Monster.monster.columnIndex, Monster.monster.rowIndex - 1)) {
        Monster.Direction = UP;
        calculateMonsterPacManDistance(Monster.monster.columnIndex, Monster.monster.rowIndex - 1);
    }

    Monster.monster.direction = Monster.tempDirection;
    Monster.shortestDistance = 2000;
}


void calculateMonsterMovementValues() {

    switch (Monster.monster.direction) {
        case UP:
            Monster.monster.columnMovement = 0;
            Monster.monster.rowMovement = -1;
            break;

        case DOWN:
            Monster.monster.columnMovement = 0;
            Monster.monster.rowMovement = 1;
            break;

        case LEFT:
            Monster.monster.columnMovement = -1;
            Monster.monster.rowMovement = 0;
            break;

        case RIGHT:
            Monster.monster.columnMovement = 1;
            Monster.monster.rowMovement = 0;
            break;
    }
}


int moveMonster() {

    calculateMonsterMovementValues();

    int columnCheck, rowCheck;

    columnCheck = Monster.monster.columnIndex + Monster.monster.columnMovement;
    rowCheck = Monster.monster.rowIndex + Monster.monster.rowMovement;

    if (!checkMonsterDirection(columnCheck, rowCheck)) {

        Monster.monster.columnMovement = 0;
        Monster.monster.rowMovement = 0;

    } else if (map[rowCheck][columnCheck] == '@') {

        return 0;

    } else {

        map[Monster.monster.rowIndex][Monster.monster.columnIndex] = CharacterOnPath;

        CharacterOnPath = map[rowCheck][columnCheck];

        Monster.monster.columnIndex += Monster.monster.columnMovement;
        Monster.monster.rowIndex += Monster.monster.rowMovement;

        map[Monster.monster.rowIndex][Monster.monster.columnIndex] = '&';
    }
}


int performActionBasedOnInput() {

    if (kbhit()) {
        switch (getch()) {
            case 'w':
            case 'W':
                PacMan.pacMan.columnMovement = 0;
                PacMan.pacMan.rowMovement = -1;
                return 0;

            case 's':
            case 'S':
                PacMan.pacMan.columnMovement = 0;
                PacMan.pacMan.rowMovement = 1;
                return 0;

            case 'a':
            case 'A':
                PacMan.pacMan.columnMovement = -1;
                PacMan.pacMan.rowMovement = 0;
                return 0;

            case 'd':
            case 'D':
                PacMan.pacMan.columnMovement = 1;
                PacMan.pacMan.rowMovement = 0;
                return 0;

            case ExitGame:
            case EXIT_GAME:
                return 1;

            case pauseGame:
            case PAUSE_GAME:
                getch();
                return 0;

            default:
                return 0;
        }
    }
}


int movePacMan(playerInfo *playerDetails) {

    int columnCheck, rowCheck;

    columnCheck = PacMan.pacMan.columnIndex + PacMan.pacMan.columnMovement;
    rowCheck = PacMan.pacMan.rowIndex + PacMan.pacMan.rowMovement;

    if (map[rowCheck][columnCheck] == '#') {
        PacMan.pacMan.columnMovement = 0;
        PacMan.pacMan.rowMovement = 0;
    }

    if (map[rowCheck][columnCheck] == '&') {
        return 0;
    } else {
        map[PacMan.pacMan.rowIndex][PacMan.pacMan.columnIndex] = ' ';

        PacMan.pacMan.columnIndex += PacMan.pacMan.columnMovement;
        PacMan.pacMan.rowIndex += PacMan.pacMan.rowMovement;

        if (map[PacMan.pacMan.rowIndex][PacMan.pacMan.columnIndex] == '+') {
            playerDetails->score += 1;
            foodControl = 0;
        }

        map[PacMan.pacMan.rowIndex][PacMan.pacMan.columnIndex] = '@';
    }
}


int createFood() {

    int foodPositionX, foodPositionY;

    foodPositionX = 1 + rand() % (W - 1);
    foodPositionY = 1 + rand() % (H - 1);

    if (map[foodPositionY][foodPositionX] != '#' && map[foodPositionY][foodPositionX] != '@' && map[foodPositionY][foodPositionX] != '&') {
        map[foodPositionY][foodPositionX] = '+';
        foodControl++;
    } else {
        return createFood();
    }
}
