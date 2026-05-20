#ifndef game_h
#define game_h

#include "constants.h"
#include <stdbool.h>

struct room {
    char map[room_height][room_width];
    struct room* connections[4];
    int itemindex;
    int itemx;
    int itemy;
};

struct player {
    int x;
    int y;
    int roomindex;
    int health;
    int inventory;
};

struct enemy {
    int x;
    int y;
    int roomindex;
    int type;
};

struct game {
    struct room rooms[10];
    struct player player;
    struct enemy enemies[4];
    int numrooms;
    int numenemies;
    bool gameover;
    bool won;
};

void initgame(struct game* g);
void renderroom(const struct game* g);
void handleinput(struct game* g);
void updategame(struct game* g);
void LimpiarPantalla();

#endif
