#include <stdlib.h>
#include "gameField.h"
#include "consoleLib.h"

const int boatIDSize[] = {
    5, 4, 4, 3, 3, 2, 2, 2
};
const int BOAT_COUNT = sizeof(boatIDSize)/sizeof(boatIDSize[0]);

void initField(struct field *field) {
    field->boats = malloc(BOAT_COUNT*sizeof(int *));
    for(int i = 0;i < BOAT_COUNT;i++)
        field->boats[i] = malloc(3*sizeof(int));

    for(int i = 0;i < 10;i++)
        for(int j = 0;j < 10;j++)
            field->field[i][j] = 0;

    for(int i = 0;i < BOAT_COUNT;i++)
        for(int j = 0;j < 2;j++)
            field->boats[i][j] = 0;
}
void removeField(struct field *field) {
    //Free boats
    for(int i = 0;i < BOAT_COUNT;i++)
        free(field->boats[i]);
    free(field->boats);
}

char *getStringFromField(struct field *field, int x, int y) {
    int id, size;

    switch(field->field[x][y]) {
        case 0:
            setColor(CL_COLOR_WHITE, CL_COLOR_BLUE);
            return "  ";
        case 1: //Boat
            setColor(CL_COLOR_LIGHT_BLACK, CL_COLOR_BLUE);
            id = getBoatAt(field, x, y);
            if(id == -1)
                return "[]";

            size = boatIDSize[id];
            if(field->boats[id][2]) { //Orientation
                if(y == field->boats[id][1]) {
                    return "/\\";
                }else if(y == (field->boats[id][1]+size-1)) {
                    return "\\/";
                }
            }else if(x == field->boats[id][0]) {
                return "<#";
            }else if(x == (field->boats[id][0]+size-1)) {
                return "#>";
            }

            return "##";
        case 2:
            setColor(CL_COLOR_YELLOW, CL_COLOR_BLUE);
            return "--";
        case 3:
            setColor(CL_COLOR_RED, CL_COLOR_BLUE);
            return "XX";
    }

    return NULL;
}

int addBoat(struct field *field, int ID, int x, int y, int o) {
    if(ID < 0 || ID >= BOAT_COUNT)
        return -1;

    int size = boatIDSize[ID];

    if(o) {
        if(y + size > 10)
            return -1;

        for(int i = y;i < y + size;i++)
            if(field->field[x][i] == 1)
                return -1;

        //Set boat in map
        for(int i = y;i < y + size;i++)
            field->field[x][i] = 1;
    }else {
        if(x + size > 10)
            return -1;

        for(int i = x;i < x + size;i++)
            if(field->field[i][y] == 1)
                return -1;

        //Set boat in map
        for(int i = x;i < x + size;i++)
            field->field[i][y] = 1;
    }

    int *ptr = field->boats[ID];
    *(ptr++) = x;
    *(ptr++) = y;
    *ptr = o;

    return 0;
}
int getBoatAt(struct field *field, int x, int y) {
    for(int i = 0;i < BOAT_COUNT;i++) {
        int size = boatIDSize[i];
        int posX = field->boats[i][0];
        int posY = field->boats[i][1];

        if(field->boats[i][2]) {
            if(posX != x)
                continue;

            for(int j = posY;j < posY + size;j++)
                if(j == y)
                    return i;
        }else {
            if(posY != y)
                continue;

            for(int j = posX;j < posX + size;j++)
                if(j == x)
                    return i;
        }
    }

    return -1;
}
