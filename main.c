#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "consoleLib.h"
#include "consoleMenu.h"
#include "gameField.h"
#include "helpMenu.h"

#define VERSION "v1.0.3"

void resetGame(void);
void init(void);
void update(void);
void updateKey(int);
void updateMouse(void);
void updateComputer(void);
void computerShot(int *x, int *y);
int computerShootField(int x, int y);
void playerShootField(int x, int y);
void playerSetFieldAttack(int key);
void drawLine(int);
void drawField(void);
void drawCustomModeSettings(void);
void drawSettings(void);
void drawStartMenu(void);
void initVars(void);

//Set funcPtr to drawField after START_MENU
static void (*draw)(void);
static void (*drawOld)(void);

//Help
static int isHelp;

static const int gameMinWidth = 74;
static const int gameMinHeight = 23;

static enum { //COMPUTER_SHOOT -> Shot, WAIT_COMPUTER_SHOOT -> After computer shot
    START_MENU, SETTINGS, CUSTOM_SETTINGS, SET_BOATS, ATTACK, COMPUTER_SHOOT,
    WAIT_COMPUTER_SHOOT, GAME_OVER
}screen;

//winner: {-1: nobody, 0: computer, 1: player}
static int winner;

//field: {0: player, 1: computer (without boats)}
static int field;
static struct field player, computer;

//Difficulty (save difficutly after new game (not in "initVars"))
static const char *difficultyName[] = {
    "EASY", "NORMAL"
};
static const enum conosleLibColor difficultyColor[] = {
    CL_COLOR_GREEN, CL_COLOR_YELLOW
};
static const int difficultyCount = 2;
static enum {
    EASY, NORMAL
}difficulty = NORMAL;

//Mode (save mode after new game (not in "initVars"))
static const char *modeName[] = {
    "CLASSIC", "BOMBER", "MINES", "CUSTOM"
};
static const enum conosleLibColor modeColor[] = {
    CL_COLOR_GREEN, CL_COLOR_RED, CL_COLOR_BLUE, CL_COLOR_YELLOW
};
static const int modeCount = 4;
static enum { //Only for draw and set
    CLASSIC, BOMBER, MINES, CUSTOM
}mode = CLASSIC;
//Mode: BOMBER (hasBomber = 1)
static int hasBomber;
static const int maxEP = 24;
static const int priceBomb = 8, priceRowColumn = 16;
static int isBomberAlivePlayer;
static int isBomberAliveComputer;
static int epPlayer;
static int epComputer;
static enum {
    NORMAL_SHOOT, BOMB_SHOOT, ROW_SHOOT, COLUMN_SHOOT
}bomberPlayerShootMode;
//Mode: MINES
static int minesCount;
static int hasMines;
//Mode: CUSTOM
static int customHasBomber = 0;
static int customHasMines = 0;
static int customMinesCount = 2;

void resetGame(void) {
    reset();
    freeTableOfContents(&content);
    removeField(&player);
    removeField(&computer);
}

int main(void) {
    init();

    while(1) {
        update();
        sleepMS(100);
    }

    return EXIT_SUCCESS;
}

void init(void) {
    initConsole();

    srand((unsigned)time(NULL));

    int widthCon, heightCon;
    getConsoleSize(&widthCon, &heightCon);
    if(widthCon < gameMinWidth || heightCon < gameMinHeight) {
        reset();
        printf("Console is to small (Min: %d x %d)!\n", gameMinWidth,
        gameMinHeight);

        exit(EXIT_FAILURE);
    }

    //Add reset after console size check
    atexit(resetGame);

    //Help menu
    initHelpMenu(gameMinWidth, gameMinHeight - 4);

    initVars();
}

void update(void) {
    //Input
    if(hasInput()) { //If has key input: call updateKey
        updateKey(getKey());
    }
    updateMouse();

    //Ouput
    updateComputer();

    //Graphics
    clrscr();
    draw();
}

//Player
static int boatHitSize;
static int boatsLivePlayer; //SCREENS: (SB: boatsToSet, A/WCS: leftBoatsPlayer)

//Computer
static int boatsLiveComputer;

static char pos[3]; //Position input field
static const char keyFieldMinMax[6] = {'a', 'j', '0', '9', '0', '1'};
static int escCheck;
void updateKey(int key) {
    //Help
    if(isHelp && key == CL_KEY_ESC) {
        isHelp = 0;
        draw = drawOld;

        return;
    }
    //Block other inputs if [y]es/[n]o
    if(escCheck) {
        if(key == 'y') {
            if(screen == START_MENU) {
                //Exit game
                exit(EXIT_SUCCESS);
            }else {
                //Reset game (to start menu)
                initVars();
            }
        }else if(key == 'n') {
            escCheck = 0;
        }
    }else {
        //Help
        if(key == CL_KEY_F1) {
            isHelp = !isHelp;
            if(isHelp) {
                drawOld = draw;
                draw = drawHelp;
            }else {
                draw = drawOld;
            }

            return;
        }
        if(isHelp) {
            if(key == CL_KEY_UP) {
                helpPage--;
                if(helpPage < 0) {
                    helpPage = maxHelpPage;
                }
            }else if(key == CL_KEY_DOWN) {
                helpPage++;
                if(helpPage > maxHelpPage) {
                    helpPage = 0;
                }
            }

            return;
        }

        //Switch field view
        if(key == 'x' && screen > SET_BOATS)
            field = !field;

        switch(screen) {
            case START_MENU:
                if(key == CL_KEY_ENTER) {
                    screen = SETTINGS;

                    //Set new draw function
                    draw = drawSettings;
                }

                break;
            case SETTINGS:
                switch(key) {
                    case  CL_KEY_UP:
                        difficulty++;
                        if(difficulty >= (unsigned)difficultyCount) {
                            difficulty = 0;
                        }

                        break;
                    case  CL_KEY_DOWN:
                        difficulty--;
                        if(difficulty == (unsigned)-1) {
                            difficulty = difficultyCount - 1;
                        }

                        break;
                    case  CL_KEY_RIGHT:
                        mode++;
                        if(mode >= (unsigned)modeCount) {
                            mode = 0;
                        }

                        break;
                    case  CL_KEY_LEFT:
                        mode--;
                        if(mode == (unsigned)-1) {
                            mode = modeCount - 1;
                        }

                        break;
                    case CL_KEY_ENTER:
                        if(mode == CUSTOM) {
                            draw = drawCustomModeSettings;
                            screen = CUSTOM_SETTINGS;

                            break;
                        }
                        //Reset vars for modes
                        initVars();

                        screen = SET_BOATS;
                        //Reset shot mode
                        bomberPlayerShootMode = NORMAL_SHOOT;

                        //Set new draw function
                        draw = drawField;

                        break;
                }

                break;
            case CUSTOM_SETTINGS:
                switch(key) {
                    case 'b':
                        customHasBomber = !customHasBomber;

                        break;
                    case 'm':
                        customHasMines = !customHasMines;

                        break;
                    case CL_KEY_UP:
                        if(!customHasMines)
                            break;

                        customMinesCount++;
                        if(customMinesCount > 10)
                            customMinesCount = 1;

                        break;
                    case CL_KEY_DOWN:
                        if(!customHasMines)
                            break;

                        customMinesCount--;
                        if(customMinesCount < 1)
                            customMinesCount = 10;

                        break;
                    case CL_KEY_ENTER:
                        //Reset vars for modes
                        initVars();

                        screen = SET_BOATS;
                        //Reset shot mode
                        bomberPlayerShootMode = NORMAL_SHOOT;

                        //Set new draw function
                        draw = drawField;

                        break;
                }

                break;
            case SET_BOATS:
                    if(!hasBomber || boatsLivePlayer > 0) {
                        fillKeyField(pos, 3, '_', keyFieldMinMax, 1, key);

                        //Only arrow keys
                        if(isArrowKey(key))
                            playerSetFieldAttack(key);
                    }else {
                        playerSetFieldAttack(key);
                    }

                    if(hasBomber && boatsLivePlayer == 0) {
                        if(pos[1] != '_' && key == CL_KEY_ENTER) {
                            int x = pos[1] - '0';
                            int y = pos[0] - 'A';

                            //Reset pos
                            pos[0] = pos[1] = '_';

                            if(player.field[x][y] != 1) {
                                player.field[x][y] = 1;

                                isBomberAlivePlayer = 1;

                                //Reset player shootMode
                                bomberPlayerShootMode = NORMAL_SHOOT;
                                //Reset for ATTACK
                                boatsLivePlayer = BOAT_COUNT;
                                screen = ATTACK;

                                if(hasMines) {
                                    //Add mines
                                    int posX, posY;
                                    for(int i = 0;i < minesCount;i++) {
                                        do {
                                            posX = rand()%10;
                                            posY = rand()%10;
                                        }while(player.field[posX][posY] != 0);
                                        player.field[posX][posY] = 4;
                                    }
                                }
                            }
                        }

                        break;
                    }

                    //Turn boat
                    if(pos[1] != '_' && key == 't') {
                        //pos[2] == '_' -> pos[2] = '0'
                        pos[2] = !(pos[2] - '0') + '0';
                    }
                    if(pos[2] != '_' && key == CL_KEY_ENTER) {
                        if(addBoat(&player, BOAT_COUNT - boatsLivePlayer,
                        pos[1] - '0', pos[0] - 'A', pos[2] - '0') != -1) {
                            boatsLivePlayer--;

                            //Setup finished
                            if(!hasBomber && boatsLivePlayer == 0) {
                                //Reset for ATTACK
                                boatsLivePlayer = BOAT_COUNT;
                                screen = ATTACK;

                                if(hasMines) {
                                    //Add mines
                                    int posX, posY;
                                    for(int i = 0;i < minesCount;i++) {
                                        do {
                                            posX = rand()%10;
                                            posY = rand()%10;
                                        }while(player.field[posX][posY] != 0);
                                        player.field[posX][posY] = 4;
                                    }
                                }
                            }
                        }

                        //Reset pos
                        pos[0] = pos[1] = pos[2] = '_';
                    }

                break;
            case ATTACK:
                    if(hasBomber && boatHitSize == -1) {
                        unsigned int oldShootMode = bomberPlayerShootMode;
                        switch(key) {
                            case 'n':
                                bomberPlayerShootMode = NORMAL_SHOOT;

                                break;
                            case 'o':
                                if(epPlayer >= priceBomb) {
                                    bomberPlayerShootMode = BOMB_SHOOT;
                                }

                                break;
                            case 'r':
                                if(epPlayer >= priceRowColumn) {
                                    bomberPlayerShootMode = ROW_SHOOT;
                                }

                                break;
                            case 'l':
                                if(epPlayer >= priceRowColumn) {
                                    bomberPlayerShootMode = COLUMN_SHOOT;
                                }

                                break;
                        }

                        if(oldShootMode != bomberPlayerShootMode) {
                            //Reset pos
                            pos[0] = pos[1] = '_';
                        }
                    }

                    //Set to "Computer shoot"
                    if(boatHitSize != -1 && key == CL_KEY_ENTER) {
                        boatHitSize = -1;

                        //Reset pos
                        pos[0] = pos[1] = '_';

                        if(hasBomber && isBomberAlivePlayer &&
                        bomberPlayerShootMode == NORMAL_SHOOT && epPlayer < maxEP)
                            epPlayer++;

                        //Reset player shootMode
                        bomberPlayerShootMode = NORMAL_SHOOT;
                        screen = COMPUTER_SHOOT;
                    }

                    switch(bomberPlayerShootMode) {
                        case NORMAL_SHOOT:
                            if(boatHitSize == -1)
                                playerSetFieldAttack(key);

                            if(pos[1] != '_' && key == CL_KEY_ENTER) {
                                boatHitSize = 0;
                                playerShootField(pos[1] - '0', pos[0] - 'A');
                            }

                            break;
                        case BOMB_SHOOT:
                            if(boatHitSize == -1)
                                playerSetFieldAttack(key);

                            if(pos[1] != '_' && key == CL_KEY_ENTER) {
                                int x = pos[1] - '0';
                                int y = pos[0] - 'A';

                                boatHitSize = 0;

                                epPlayer -= priceBomb;

                                if(x > 0)
                                    playerShootField(x - 1, y);
                                if(x < 9)
                                    playerShootField(x + 1, y);
                                if(y > 0)
                                    playerShootField(x, y - 1);
                                if(y < 9)
                                    playerShootField(x, y + 1);

                                playerShootField(x, y);

                                //Reset pos for draw
                                pos[0] = (char)y + 'A';
                                pos[1] = (char)x + '0';

                                boatHitSize = 'b';
                            }

                            break;
                        case ROW_SHOOT:
                            if(boatHitSize == -1)  {
                                fillKeyField(pos, 1, '_', keyFieldMinMax, 1, key);
                                if(key == CL_KEY_DOWN) {
                                    if(pos[0] == '_') {
                                        pos[0] = 'J';
                                    }else {
                                        pos[0]++;
                                        if(pos[0] > 'J')
                                            pos[0] = 'A';
                                    }
                                }else if(key == CL_KEY_UP) {
                                    if(pos[0] == '_') {
                                        pos[0] = 'A';
                                    }else {
                                        pos[0]--;
                                        if(pos[0] < 'A')
                                            pos[0] = 'J';
                                    }
                                }
                            }

                            if(pos[0] != '_' && key == CL_KEY_ENTER) {
                                int y = pos[0] - 'A';

                                boatHitSize = 0;

                                epPlayer -= priceRowColumn;

                                for(int x = 0;x < 10;x++)
                                    playerShootField(x, y);

                                //Reset pos for draw
                                pos[0] = (char)y + 'A';

                                boatHitSize = 'r';
                            }

                            break;
                        case COLUMN_SHOOT:
                            if(boatHitSize == -1)  {
                                fillKeyField(pos, 1, '_', "09", 1, key);
                                if(key == CL_KEY_RIGHT) {
                                    if(pos[0] == '_') {
                                        pos[0] = '9';
                                    }else {
                                        pos[0]++;
                                        if(pos[0] > '9')
                                            pos[0] = '0';
                                    }
                                }else if(key == CL_KEY_LEFT) {
                                    if(pos[0] == '_') {
                                        pos[0] = '0';
                                    }else {
                                        pos[0]--;
                                        if(pos[0] < '0')
                                            pos[0] = '9';
                                    }
                                }
                            }

                            if(pos[0] != '_' && key == CL_KEY_ENTER) {
                                int x = pos[0] - '0';

                                boatHitSize = 0;

                                epPlayer -= priceRowColumn;

                                for(int y = 0;y < 10;y++)
                                    playerShootField(x, y);

                                //Reset pos for draw
                                pos[0] = (char)x + '0';

                                boatHitSize = 'c';
                            }

                            break;
                }

                break;
            case WAIT_COMPUTER_SHOOT:
                if(key == CL_KEY_ENTER) {
                    //Reset shot mode
                    bomberPlayerShootMode = NORMAL_SHOOT;

                    screen = ATTACK;
                }

                break;
            case COMPUTER_SHOOT:
            case GAME_OVER:
                break;
        }

        //Exit game
        if(key == CL_KEY_ESC) {
            if(screen == SETTINGS || screen == CUSTOM_SETTINGS) {
                screen = START_MENU;
                draw = drawStartMenu;

                return;
            }
            escCheck = 1;
        }
    }
}
void updateMouse(void) {
    int column, row;
    getMousePosClicked(&column, &row);

    if(column == -1) //No input
        return;

    //Help
    if(isHelp) {
        //"row - 2": Content start at row 2
        int tmpPage = getPageMouseClicked(&content, helpPage, row - 2);
        if(tmpPage != -1)
            helpPage = tmpPage;

        if(row == 22 && column < 8)
            updateKey(CL_KEY_DOWN);

        return;
    }

    if(escCheck) {
        if(row == 13) {
            if(screen == START_MENU) {
                if(column > 28 && column < 34)
                    updateKey('y');
                else if(column > 40 && column < 45)
                    updateKey('n');
            }else {
                if(column > 26 && column < 32)
                    updateKey('y');
                else if(column > 41 && column < 46)
                    updateKey('n');
            }
        }
    }else {
        int fieldX = column/3;
        int fieldY = (row - 1)/2;

        switch(screen) {
            case START_MENU:
                if(row == 16 && column > 26 && column < 32)
                    updateKey(CL_KEY_ENTER);
                if(row == 21 && column > 64 && column < 73)
                    updateKey(CL_KEY_F1);

                break;
            case SETTINGS:
                if(row == 0 && column > 5 && column < 11)
                    updateKey(CL_KEY_ENTER);

                //Settings
                if(row == 4)
                    updateKey(CL_KEY_UP);
                if(row == 5)
                    updateKey(CL_KEY_RIGHT);

                break;
            case CUSTOM_SETTINGS:
                if(row == 0 && column > 5 && column < 11)
                    updateKey(CL_KEY_ENTER);

                //Settings
                if(row == 4)
                    updateKey('b');
                if(row == 5)
                    updateKey('m');
                if(row == 6 && customHasMines)
                    updateKey(CL_KEY_UP);

                break;
            case SET_BOATS:
            case ATTACK:
                if(fieldX > 10 || fieldY > 10 || (fieldX == 0 && fieldY == 0))
                    break;

                int keySetX = '\0';
                int keySetY = '\0';
                if(fieldX != 0)
                    keySetX = fieldX - 1 + '0';
                if(fieldY != 0)
                    keySetY = fieldY - 1 + 'a';

                if(hasBomber && bomberPlayerShootMode == COLUMN_SHOOT) {
                    if(keySetX != 0)
                        pos[0] = (char)keySetX;

                    break;
                }

                if(keySetX != '\0') {
                    if(pos[0] == '_')
                        pos[0] = 'A';
                    pos[1] = (char)keySetX;
                }
                if(keySetY != '\0')
                    pos[0] = (char)toupper(keySetY);

                if(hasBomber && bomberPlayerShootMode == ROW_SHOOT)
                    pos[1] = '_';

                break;
            case COMPUTER_SHOOT:
            case WAIT_COMPUTER_SHOOT:
            case GAME_OVER:
                break;
        }

        //Switch field
        switch(screen) {
            case START_MENU:
            case SETTINGS:
            case CUSTOM_SETTINGS:
            case SET_BOATS:
                break;
            case ATTACK:
            case COMPUTER_SHOOT:
            case WAIT_COMPUTER_SHOOT:
            case GAME_OVER:
                if(row == 7 && column > 40)
                    updateKey('x');

                break;
        }
    }
}

static int computerXPosBoatHit;
static int computerYPosBoatHit;
static int computerBoatHitToCheck; //Up(1), down(2), right(3), left(4)|(bit)
//For drawing
static int computerAttackedXPos;
static int computerAttackedYPos;
void updateComputer(void) {
    if(screen == COMPUTER_SHOOT) {
        screen = WAIT_COMPUTER_SHOOT;

        if(hasBomber && isBomberAliveComputer && epComputer < maxEP)
            epComputer++;

        while(1) {
            int x, y;
            computerShot(&x, &y);
            computerAttackedXPos = x;
            computerAttackedYPos = y;

            if(difficulty == NORMAL) {
                if(hasBomber && epComputer > priceBomb) {
                    epComputer -= priceBomb + 1;
                    if(x > 0)
                        computerShootField(x - 1, y);
                    if(x < 9)
                        computerShootField(x + 1, y);
                    if(y > 0)
                        computerShootField(x, y - 1);
                    if(y < 9)
                        computerShootField(x, y + 1);

                    computerShootField(x, y);

                    return;
                }
            }

            if(computerShootField(x, y))
                return;
        }
    }
}
void computerShot(int *x, int *y) {
    if(difficulty == NORMAL) {
        //Check neighbor fields if hitted (shot whole boat)
        if(computerBoatHitToCheck != 0) {
            //Set x and y
            *x = computerXPosBoatHit;
            *y = computerYPosBoatHit;

            //Select random neighbor field
            int pos;
            do {
                pos = rand()%4;
            }while(!(computerBoatHitToCheck & (1 << pos))); //Repeat if checked
            computerBoatHitToCheck ^= (1 << pos); //Set to checked
            switch(pos) {
                case 0: //Up
                    if(*y != 0)
                        (*y)--;

                    break;
                case 1: //Down
                    if(*y != 9)
                        (*y)++;

                    break;
                case 2: //Right
                    if(*x != 9)
                        (*x)++;

                    break;
                case 3: //Left
                    if(*x != 0)
                        (*x)--;

                    break;
            }

            if(computerBoatHitToCheck == 0) //Reset if all fields are checked
                computerXPosBoatHit = computerYPosBoatHit = -1;

            //If neighbor is invalid try again
            if(*x == computerXPosBoatHit && *y == computerYPosBoatHit)
                computerShot(x, y);

            return;
        }
    }

    *x = rand()%10;
    *y = rand()%10;
}
int computerShootField(int x, int y) {
    if(player.field[x][y] < 2) { //Hit
        player.field[x][y] += 2; //Set to hit
        if(player.field[x][y] == 3) {
            //Set hit pos for computer
            computerXPosBoatHit = x;
            computerYPosBoatHit = y;
            //Set boat neighbor hit check
            computerBoatHitToCheck = 15;

            //If boat is dead remove one boat
            int hitCount = 0;
            int id = getBoatAt(&player, x, y);
            if(id == -1) {
                computerBoatHitToCheck = 0;
                computerXPosBoatHit = computerYPosBoatHit = -1;

                //Bomber player is dead
                isBomberAlivePlayer = 0;

                return 1;
            }
            int size = boatIDSize[id];
            int posX = player.boats[id][0];
            int posY = player.boats[id][1];

            if(player.boats[id][2]) {
                for(int i = posY;i < posY + size;i++)
                    if(player.field[posX][i] == 3)
                        hitCount++;
            }else {
                for(int i = posX;i < posX + size;i++)
                    if(player.field[i][posY] == 3)
                        hitCount++;
            }
            if(hitCount != size)
                return 1;

            //Reset computer boat search
            computerBoatHitToCheck = 0;
            computerXPosBoatHit = -1;
            computerYPosBoatHit = -1;

            //Remove one boat
            boatsLivePlayer--;
            if(boatsLivePlayer == 0) {
                winner = 0;
                screen = GAME_OVER;
            }
        }

        return 1;
    }else if(player.field[x][y] == 4) { //Hit mine
        player.field[x][y] = 5; //Set to hit mine

        for(int i = x-1;i < x + 2;i++) {
            for(int j = y-1;j < y + 2;j++) {
                if(i < 0 || i > 9 || j < 0 || j > 9)
                    continue;

                computerShootField(i, j);
            }
        }

        return 1;
    }

    return 0;
}

void playerShootField(int x, int y) {
    if(computer.field[x][y] < 2) { //Hit
        computer.field[x][y] += 2; //Set to hit
        if(computer.field[x][y] == 3) {
            //If one boat is dead remove one boat
            int hitCount = 0;
            int id = getBoatAt(&computer, x, y);
            if(id == -1) {
                boatHitSize = 1;

                //Bomber computer is dead
                isBomberAliveComputer = 0;

                return;
            }
            int size = boatHitSize = boatIDSize[id];
            int o = computer.boats[id][2];
            int posX = computer.boats[id][0];
            int posY = computer.boats[id][1];

            if(o) {
                for(int i = posY;i < posY + size;i++)
                    if(computer.field[posX][i] == 3)
                        hitCount++;
            }else {
                for(int i = posX;i < posX + size;i++)
                    if(computer.field[i][posY] == 3)
                        hitCount++;
            }
            if(hitCount != size)
                return;

            boatsLiveComputer--;
            if(boatsLiveComputer == 0) {
                winner = 1;
                screen = GAME_OVER;

                //Reset pos
                pos[0] = pos[1] = '_';
            }
        }
    }else if(computer.field[x][y] == 4) { //Hit mine
        computer.field[x][y] = 5; //Set to hit mine

        for(int i = x-1;i < x + 2;i++) {
            for(int j = y-1;j < y + 2;j++) {
                if(i < 0 || i > 9 || j < 0 || j > 9)
                    continue;

                playerShootField(i, j);
            }
        }

        boatHitSize = 'm';
    }else{ //Already hitted
        boatHitSize = -1;

        //Reset pos
        pos[0] = pos[1] = '_';
    }
}
void playerSetFieldAttack(int key) {
    fillKeyField(pos, 2, '_', keyFieldMinMax, 1, key);

    if(pos[0] != '_') {
        switch(key) {
            case CL_KEY_LEFT:
                if(pos[1] == '_') {
                    pos[1] = '0';

                    break;
                }
                    pos[1]--;
                if(pos[1] < '0') {
                    pos[1] = '9';
                }

                break;
            case CL_KEY_UP:
                pos[0]--;
                if(pos[0] < 'A') {
                    pos[0] = 'J';
                }

                break;
            case CL_KEY_RIGHT:
                if(pos[1] == '_') {
                    pos[1] = '9';

                    break;
                }
                pos[1]++;
                if(pos[1] > '9') {
                    pos[1] = '0';
                }

                break;
            case CL_KEY_DOWN:
                pos[0]++;
                if(pos[0] > 'J') {
                    pos[0] = 'A';
                }

                break;
        }
    }else {
        if(key == CL_KEY_UP) {
            pos[0] = 'A';
        }else if(key == CL_KEY_DOWN) {
            pos[0] = 'J';
        }
    }
}

//For drawing (SCREEN: SET_BOATS)
static int boatCount[4];
void drawLine(int line) {
    drawf("        ");

    switch(screen) {
        case SET_BOATS:
            if(line == 3) {
                drawf("Boats to set:");

                break;
            }

            //Count boats
            for(int i = 0;i < boatsLivePlayer;i++)
                boatCount[boatIDSize[BOAT_COUNT - 1 - i] - 2]++;

            //Draw boats
            switch(line) {
                case 4: //5, 4
                    drawf("%dx ", boatCount[3]);
                    setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
                    drawf("<########>");
                    resetColor();
                    drawf(" || %dx ", boatCount[2]);
                    setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
                    drawf("<######>");

                    break;
                case 5: //3, 2
                    drawf("%dx ", boatCount[1]);
                    setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
                    drawf("<####>");
                    resetColor();
                    drawf(" || %dx ", boatCount[0]);
                    setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
                    drawf("<##>");

                    if(hasBomber) {
                        resetColor();
                        drawf(" || 1x ");
                        setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_NO_COLOR);
                        drawf("[]");
                    }

                    break;
                case 6: //Draw set boat
                    drawf("Position: %c%c", pos[0], pos[1]);

                    if(!hasBomber || boatsLivePlayer > 0)
                        drawf(", Orientation: %c", pos[2]);

                    break;
            }

            //Reset boatCount
            boatCount[0] = boatCount[1] = boatCount[2] = boatCount[3] = 0;

            break;
        case ATTACK:
            switch(line) {
                case 3:
                    drawf("Attack:");

                    break;
                case 4:
                    drawf("Boats left:");

                    break;
                case 5:
                    drawf("Computer: %d, Player: %d", boatsLiveComputer,
                    boatsLivePlayer);

                    break;
                case 6:
                    if(hasBomber && (bomberPlayerShootMode == ROW_SHOOT ||
                    bomberPlayerShootMode == COLUMN_SHOOT))
                        drawf("Position: %c", pos[0]);
                    else
                        drawf("Position: %c%c", pos[0], pos[1]);

                    if(hasBomber && boatHitSize == -1) {
                        drawf(" (");
                        switch(bomberPlayerShootMode) {
                            case NORMAL_SHOOT:
                                drawf("Normal shot");
                                break;
                            case BOMB_SHOOT:
                                drawf("Bomb");
                                break;
                            case ROW_SHOOT:
                                drawf("Row bomb");
                                break;
                            case COLUMN_SHOOT:
                                drawf("Column bomb");
                                break;
                        }
                        drawf(")");
                    }

                    if(boatHitSize != -1) {
                        drawf(" (");
                        switch(boatHitSize) {
                            case 0:
                                drawf("No hit");
                                break;
                            case 'b':
                                drawf("Hit: Bomb");
                                break;
                            case 'r':
                                drawf("Hit: Row bomb");
                                break;
                            case 'c':
                                drawf("Hit: Column bomb");
                                break;
                            case 'm':
                                drawf("Hit: Mine");
                                break;
                            default:
                                drawf("Hit: Size: %d", boatHitSize);
                        }
                        drawf(")");
                    }

                    break;
            }

            if(hasBomber) {
                switch(line) {
                    case 7:
                        drawf("EP: [");

                        int epDraw = (int)(((double)epPlayer/maxEP)*10.);
                        while(epDraw--)
                            drawf("#");
                        epDraw = 10 - (int)(((double)epPlayer/maxEP)*10.);
                        while(epDraw--)
                            drawf(" ");
                        drawf("] %d%%", (int)(((double)epPlayer/maxEP)*100.));

                        break;
                    case 8:
                        drawf("Press: ");

                        setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
                        drawf("N");

                        resetColor();
                        drawf(" (Normal shot)");

                        break;
                    case 9:
                        if(epPlayer >= priceBomb)
                            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
                        else
                            setColor(CL_COLOR_RED, CL_COLOR_NO_COLOR);
                        drawf("O");

                        resetColor();
                        drawf(" (Bomb) || ");

                        if(epPlayer >= priceRowColumn)
                            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
                        else
                            setColor(CL_COLOR_RED, CL_COLOR_NO_COLOR);
                        drawf("R");

                        resetColor();
                        drawf("/");

                        if(epPlayer >= priceRowColumn)
                            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
                        else
                            setColor(CL_COLOR_RED, CL_COLOR_NO_COLOR);
                        drawf("L");

                        resetColor();
                        drawf(" (Row/Column bomb)");

                        break;
                }
            }

            break;
        case WAIT_COMPUTER_SHOOT:
            if(line == 3)
                drawf("Computer attacked field %c%c", computerAttackedYPos + 'A',
                computerAttackedXPos + '0');

            break;
        case GAME_OVER:
            switch(line) {
                case 3:
                    drawf("Game Over!");

                    break;
                case 5:
                    drawf("Winner:");

                    break;
                case 6:
                    if(winner == 0) {
                        setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
                        drawf("Computer");
                    }else {
                        setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
                        drawf("Player");
                    }

                    break;
            }

            break;
        case START_MENU:
        case SETTINGS:
        case CUSTOM_SETTINGS:
        case COMPUTER_SHOOT:
            break;
    }

    if(line == 2 && (screen == ATTACK || screen == WAIT_COMPUTER_SHOOT ||
    screen == GAME_OVER)) {
        drawf("Field: ");
        if(field) {
            setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
            drawf("Computer");
        }else {
            setColor(CL_COLOR_GREEN, CL_COLOR_NO_COLOR);
            drawf("Player");
        }
    }
}
void drawField(void) {
    setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
    drawf("---------------------------------\n  | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|"
    "\n");
    for(int i = 0;i < 10;i++) {
        setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
        drawf("---------------------------------\n %c|", 'A' + i);
        for(int j = 0;j < 10;j++) {
            if(field == 0) {
                char *tmp = getStringFromField(&player, j, i);

                //Mode: MINES
                if(tmp == NULL) {
                    switch(player.field[j][i]) {
                        case 4:
                            tmp = "  ";
                            break;
                        case 5: //Mine hit
                            setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_BLUE);
                            tmp = "**";
                            break;
                    }
                }

                //Mark pos
                if(screen == SET_BOATS && pos[0] != '_') {
                    if(pos[0] - 'A' == i && pos[1] == '_') {
                        setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
                    }else if(pos[2] == '_') {
                        if(i == pos[0] - 'A' && j == pos[1] - '0') {
                            setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
                        }
                    }else { //pos[2] != '_'
                        int size = boatIDSize[BOAT_COUNT - boatsLivePlayer];
                        if(pos[0] - 'A' == i && pos[2] == '0') {
                            if(j >= pos[1] - '0' && j < pos[1] - '0' + size) {
                                setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
                            }
                        }else if(pos[1] - '0' == j && pos[2] == '1') {
                            if(i >= pos[0] - 'A' && i < pos[0] - 'A' + size) {
                                setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
                            }
                        }
                    }
                }

                drawf("%s", tmp);

                setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
                drawf("|");
            }else {
                char *str = getStringFromField(&computer, j, i);

                //Mode: MINES
                if(str == NULL) {
                    switch(computer.field[j][i]) {
                        case 4:
                            str = "  ";
                            break;
                        case 5: //Mine hit
                            setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_BLUE);
                            str = "**";
                            break;
                    }
                }

                //See boats pos after game over
                if(computer.field[j][i] == 1 && screen != GAME_OVER) {
                    str = "  ";
                    //Reset for computer field
                    setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
                }

                //Mark pos
                if(hasBomber && bomberPlayerShootMode == BOMB_SHOOT &&
                pos[1] != '_') {
                    int x = pos[1] - '0';
                    int y = pos[0] - 'A';

                    if((j == x && i > y-2 && i < y+2) || (i == y && j > x-2 &&
                    j < x+2))
                        setColor(CL_COLOR_RED, CL_COLOR_YELLOW);
                }else if(hasBomber && bomberPlayerShootMode == COLUMN_SHOOT &&
                pos[0] != '_') {
                    if(pos[0] - '0' == j) {
                        setColor(CL_COLOR_RED, CL_COLOR_YELLOW);
                    }
                }else if(pos[0] != '_') {
                    if(pos[0] - 'A' == i && pos[1] == '_') {
                        setColor(CL_COLOR_RED, CL_COLOR_YELLOW);
                    }else if(i == pos[0] - 'A' && j == pos[1] - '0') {
                        setColor(CL_COLOR_RED, CL_COLOR_YELLOW);
                        str = "><";
                    }
                }

                drawf("%s", str);

                setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
                drawf("|");
            }
        }
        if(i > 1) {
            resetColor();
            drawLine(i);
        }
        drawf("\n");
    }
    setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
    drawf("---------------------------------\n");

    //Exit
    if(escCheck) {
        setColor(CL_COLOR_BLACK, CL_COLOR_YELLOW);
        setCursorPos(27, 10);
        drawf("Back to start menu?");
        setCursorPos(27, 11);
        drawf("-------------------");
        setCursorPos(27, 12);
        drawf("                   ");
        setCursorPos(27, 13);
        drawf("[y]es          [n]o");

        //Draw border
        setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_RED);
        setCursorPos(26, 9);
        drawf("                     ");
        setCursorPos(26, 14);
        drawf("                     ");
        for(int i = 10;i < 14;i++) {
            setCursorPos(26, i);
            drawf(" ");
            setCursorPos(46, i);
            drawf(" ");
        }
    }
}
void drawCustomModeSettings(void) {
    resetColor();
    drawf("Press ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("ENTER");
    resetColor();
    drawf(" to start the game!");

    setCursorPos(0, 2);
    setColor(CL_COLOR_YELLOW, CL_COLOR_NO_COLOR);
    drawf("Settings for CUSTOM mode\n------------------------\n");

    resetColor();
    drawf("Has bomber: %s (Press: ", customHasBomber?"true":"false");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("b");
    resetColor();
    drawf(")\nHas mines: %s (Press: ", customHasMines?"true":"false");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("m");
    resetColor();
    drawf(")\n");

    if(customHasMines) {
        drawf("Mines count: %d (Press: ", customMinesCount);
        setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
        drawf("UP");
        resetColor();
        drawf(", ");
        setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
        drawf("DOWN");
        resetColor();
        drawf(")\n");
    }
}
void drawSettings(void) {
    resetColor();
    drawf("Press ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("ENTER");
    resetColor();
    drawf(" to start the game!");

    setCursorPos(0, 2);
    setColor(CL_COLOR_YELLOW, CL_COLOR_NO_COLOR);
    drawf("Settings\n--------\n");

    resetColor();
    drawf("Difficulty: ");
    setColor(CL_COLOR_WHITE, difficultyColor[difficulty]);
    drawf("%s", difficultyName[difficulty]);
    resetColor();
    drawf(" (Press: ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("UP");
    resetColor();
    drawf(", ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("DOWN");
    resetColor();
    drawf(")\nMode: ");
    setColor(CL_COLOR_WHITE, modeColor[mode]);
    drawf("%s", modeName[mode]);
    resetColor();
    drawf(" (Press: ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("LEFT");
    resetColor();
    drawf(", ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("RIGHT");
    resetColor();
    drawf(")");
}
void drawStartMenu(void) {
    //Draw border (top)
    setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
    drawf("/--------------------------------------------------------------------"
          "----\\\n");

    //Draw text
    setColor(CL_COLOR_LIGHT_YELLOW, CL_COLOR_NO_COLOR);
    drawf("          -----------------------------------------------------\n    "
          "      .----   .----   |   |   --+--   .----   .----   .----\n        "
          "  |       |       |   |     |     |       |       |\n          '---. "
          "  |       +---+     |     +----   +----   +----\n              |   | "
          "      |   |     |     |       |       |\n          ----'   '----   | "
          "  |   --+--   |       |       '----\n  ------------------------------"
          "---------------------------------------\n  |   |   .----   .---.   .-"
          "---   .----   .   .   |  ./   .----   .   .\n  |   |   |       |   | "
          "  |       |       |\\  |   | /'    |       |\\  |\n  (   )   +----   "
          "+.--'   '---.   +----   | \\ |   :{      +----   | \\ |\n   \\ /    |"
          "       | \\.        |   |       |  \\|   | \\.    |       |  \\|\n   "
          " :     '----   |  '\\   ----'   '----   '   '   |  '\\   '----   '   "
          "'\n  ----------------------------------------------------------------"
          "-----");

    //Draw infos
    resetColor();
    char verStr[70]; //69 chars + '\0'
    sprintf(verStr, "Version: " VERSION);
    setCursorPos(71 - (int)strlen(verStr), 14); //69 chars, (3 chars empty)
    drawf("%s", verStr);
    setCursorPos(21, 16);
    drawf("Press ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("ENTER");
    resetColor();
    drawf(" to start the game!");
    setCursorPos(1, 21);
    drawf("By ");
    setColor(CL_COLOR_NO_COLOR, CL_COLOR_YELLOW);
    drawf("JDDev0");

    resetColor();
    setCursorPos(65, 21);
    drawf("Help: ");
    setColor(CL_COLOR_LIGHT_RED, CL_COLOR_NO_COLOR);
    drawf("F1");

    //Draw border
    setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
    for(int i = 1;i < 22;i++) {
        setCursorPos(0, i);
        drawf("|");
        setCursorPos(73, i);
        drawf("|");
    }
    drawf("\n\\-----------------------------------------------------------------"
          "-------/");

    //Exit
    if(escCheck) {
        setColor(CL_COLOR_BLACK, CL_COLOR_YELLOW);
        setCursorPos(29, 10);
        drawf("   Exit game?   ");
        setCursorPos(29, 11);
        drawf("   ----------   ");
        setCursorPos(29, 12);
        drawf("                ");
        setCursorPos(29, 13);
        drawf("[y]es       [n]o");

        //Draw border
        setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_RED);
        setCursorPos(28, 9);
        drawf("                  ");
        setCursorPos(28, 14);
        drawf("                  ");
        for(int i = 10;i < 14;i++) {
            setCursorPos(28, i);
            drawf(" ");
            setCursorPos(45, i);
            drawf(" ");
        }
    }
}

void initVars(void) {
    //Fields
    initField(&player);
    initField(&computer);

    //Help
    isHelp = 0;

    //Add boats randomly to computer
    int posX, posY, o;
    for(int i = 0;i < BOAT_COUNT;i++) {
        //Add boat if error -> retry
        do {
            posX = rand()%10;
            posY = rand()%10;
            o = rand()%2;
        }while(addBoat(&computer, i, posX, posY, o) == -1);
    }

    field = 0;

    //Function pointer
    drawOld = draw = drawStartMenu;
    screen = START_MENU;

    //Mode
    minesCount = 2;
    hasBomber = hasMines = 0;
    isBomberAlivePlayer = 0;
    epPlayer = epComputer = 0;
    bomberPlayerShootMode = NORMAL_SHOOT;
    switch(mode) {
        case BOMBER:
            hasBomber = 1;
            break;
        case MINES:
            hasMines = 1;
            break;
        case CLASSIC:
        case CUSTOM:
            break;
    }
    //Mode: CUSTOM
    if(mode == CUSTOM) {
        hasBomber = customHasBomber;
        hasMines = customHasMines;
        if(hasMines)
            minesCount = customMinesCount;
    }
    //Mode: BOMBER
    if(hasBomber) {
        do {
            posX = rand()%10;
            posY = rand()%10;
        }while(computer.field[posX][posY] == 1);
        computer.field[posX][posY] = 1;
        isBomberAliveComputer = 1;
    }
    //Mode: MINES
    if(hasMines) {
        //Add mines
        for(int i = 0;i < minesCount;i++) {
            do {
                posX = rand()%10;
                posY = rand()%10;
            }while(computer.field[posX][posY] != 0);
            computer.field[posX][posY] = 4;
        }
    }

    //Other
    winner = -1;

    boatHitSize = -1;
    boatsLivePlayer = BOAT_COUNT;

    boatsLiveComputer = BOAT_COUNT;

    computerXPosBoatHit = computerYPosBoatHit = -1;
    computerBoatHitToCheck = 0;

    computerAttackedXPos = computerAttackedYPos = -1;

    for(int i = 0;i < (int)(sizeof(boatCount)/sizeof(boatCount[0]));i++) {
        boatCount[i] = 0;
    }

    for(int i = 0;i < (int)(sizeof(pos)/sizeof(pos[0]));i++) {
        pos[i] = '_';
    }

    escCheck = 0;
}
