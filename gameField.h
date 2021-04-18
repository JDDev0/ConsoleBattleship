#ifndef GAME_FIELD_H
#define GAME_FIELD_H
    extern const int boatIDSize[];
    extern const int BOAT_COUNT;

    struct field {
        //0: empty, 1: boat, 2: hitEmpty, 3: hitBoat
        int field[10][10];
        //ID[0: posX, 1: posY, 2: orientation (0: -x-, 1: |y|)]
        int **boats;
    };

    void initField(struct field *field);
    void removeField(struct field *field);

    char *getStringFromField(struct field *field, int x, int y);

    int addBoat(struct field *field, int ID, int x, int y, int o);
    int getBoatAt(struct field *field, int x, int y);
#endif
