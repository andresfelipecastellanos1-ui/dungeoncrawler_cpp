#include "game.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

void initgame(struct game* g)
{
    memset(g, 0, sizeof(struct game));
    g->numrooms = 8;
    g->numenemies = 3;
    g->player.health = 100;
    g->player.roomindex = 0;
    g->player.x = 3;
    g->player.y = 5;
    g->player.inventory = -1;

    int i, j, k;
    for(i = 0; i < g->numrooms; i++)
    {
        for(j = 0; j < room_height; j++)
            for(k = 0; k < room_width; k++)
                g->rooms[i].map[j][k] = ' ';

        for(k = 0; k < room_width; k++) {
            g->rooms[i].map[0][k] = '#';
            g->rooms[i].map[room_height-1][k] = '#';
        }
        for(j = 0; j < room_height; j++) {
            g->rooms[i].map[j][0] = '#';
            g->rooms[i].map[j][room_width-1] = '#';
        }
        g->rooms[i].itemindex = -1;
    }

    for(i=3; i<17; i++) g->rooms[0].map[4][i] = '#';
    for(i=3; i<17; i++) g->rooms[0].map[8][i] = '#';

    for(i=3; i<10; i++) g->rooms[1].map[i][6] = '#';
    for(i=3; i<10; i++) g->rooms[1].map[i][13] = '#';

    for(i=4; i<16; i++) g->rooms[2].map[4][i] = '#';
    for(i=4; i<16; i++) g->rooms[2].map[9][i] = '#';

    for(i=3; i<10; i++) g->rooms[3].map[i][5] = '#';
    for(i=3; i<10; i++) g->rooms[3].map[i][14] = '#';

    for(i=4; i<16; i++) g->rooms[4].map[3][i] = '#';
    for(i=4; i<16; i++) g->rooms[4].map[8][i] = '#';

    for(i=3; i<17; i++) g->rooms[5].map[4][i] = '#';
    for(i=3; i<17; i++) g->rooms[5].map[8][i] = '#';

    // Puertas corregidas
    g->rooms[0].map[5][room_width-1] = ' ';
    g->rooms[1].map[5][0] = ' ';
    g->rooms[1].map[room_height-2][10] = ' ';
    g->rooms[2].map[room_height-2][10] = ' ';
    g->rooms[3].map[5][room_width-1] = ' ';
    g->rooms[4].map[room_height-2][10] = ' ';
    g->rooms[5].map[5][0] = ' ';

    g->rooms[0].connections[dir_east] = &g->rooms[1];
    g->rooms[1].connections[dir_west] = &g->rooms[0];
    g->rooms[1].connections[dir_south] = &g->rooms[2];
    g->rooms[2].connections[dir_north] = &g->rooms[1];
    g->rooms[2].connections[dir_south] = &g->rooms[3];
    g->rooms[3].connections[dir_north] = &g->rooms[2];
    g->rooms[3].connections[dir_east] = &g->rooms[4];
    g->rooms[4].connections[dir_west] = &g->rooms[3];
    g->rooms[4].connections[dir_south] = &g->rooms[5];
    g->rooms[5].connections[dir_north] = &g->rooms[4];

    g->enemies[0].roomindex = 0; g->enemies[0].x = 10; g->enemies[0].y = 6; g->enemies[0].type = 0;
    g->enemies[1].roomindex = 2; g->enemies[1].x = 10; g->enemies[1].y = 5; g->enemies[1].type = 0;
    g->enemies[2].roomindex = 4; g->enemies[2].x = 12; g->enemies[2].y = 6; g->enemies[2].type = 1;

    g->rooms[3].itemindex = 0;
    g->rooms[3].itemx = 10;
    g->rooms[3].itemy = 5;
}

void renderroom(const struct game* g)
{
    const struct room* r = &g->rooms[g->player.roomindex];
    int i, j;

    printf("\n=== HABITACION %d ===\n", g->player.roomindex);

    for(i = 0; i < room_height; i++)
    {
        for(j = 0; j < room_width; j++)
        {
            if(g->enemies[0].roomindex == g->player.roomindex && g->enemies[0].x == j && g->enemies[0].y == i) { printf("X"); continue; }
            if(g->enemies[1].roomindex == g->player.roomindex && g->enemies[1].x == j && g->enemies[1].y == i) { printf("X"); continue; }
            if(g->enemies[2].roomindex == g->player.roomindex && g->enemies[2].x == j && g->enemies[2].y == i) { printf("P"); continue; }

            if(i == g->player.y && j == g->player.x) { printf("@"); continue; }
            if(r->itemindex != -1 && i == r->itemy && j == r->itemx) { printf("K"); continue; }

            printf("%c", r->map[i][j]);
        }
        printf("\n");
    }

    printf("Vida: %d | Inventario: %s\n", g->player.health, g->player.inventory == -1 ? "VACIO" : "LLAVE");
    printf("WASD = mover | E = recoger | Q = salir\n");
}

void handleinput(struct game* g)
{
    char key = _getch();

    if(key == 'q' || key == 'Q') {
        g->gameover = true;
        return;
    }

    int nx = g->player.x;
    int ny = g->player.y;

    if(key == 'w' || key == 'W') ny--;
    if(key == 's' || key == 'S') ny++;
    if(key == 'a' || key == 'A') nx--;
    if(key == 'd' || key == 'D') nx++;

    const struct room* r = &g->rooms[g->player.roomindex];

    if(nx > 0 && nx < room_width-1 && ny > 0 && ny < room_height-1 && r->map[ny][nx] != '#')
    {
        g->player.x = nx;
        g->player.y = ny;
    }

    if((key == 'e' || key == 'E') && r->itemindex != -1 && 
       g->player.x == r->itemx && g->player.y == r->itemy)
    {
        g->player.inventory = 0;
        struct room* room_mod = (struct room*)r;
        room_mod->itemindex = -1;
    }

    if(g->player.x == room_width-2 && r->connections[dir_east] != NULL) {
        g->player.roomindex = (int)(r->connections[dir_east] - g->rooms);
        g->player.x = 2;
    }
    if(g->player.x == 1 && r->connections[dir_west] != NULL) {
        g->player.roomindex = (int)(r->connections[dir_west] - g->rooms);
        g->player.x = room_width-3;
    }
    if(g->player.y == room_height-2 && r->connections[dir_south] != NULL) {
        g->player.roomindex = (int)(r->connections[dir_south] - g->rooms);
        g->player.y = 2;
    }
    if(g->player.y == 1 && r->connections[dir_north] != NULL) {
        g->player.roomindex = (int)(r->connections[dir_north] - g->rooms);
        g->player.y = room_height-3;
    }
}

void updategame(struct game* g)
{
    int i;
    for(i = 0; i < g->numenemies; i++)
    {
        struct enemy* e = &g->enemies[i];
        if(e->roomindex != g->player.roomindex) continue;

        const struct room* r = &g->rooms[e->roomindex];

        if(e->type == 0)
        {
            if(g->player.x > e->x && r->map[e->y][e->x+1] != '#') e->x++;
            else if(g->player.x < e->x && r->map[e->y][e->x-1] != '#') e->x--;
            else if(g->player.y > e->y && r->map[e->y+1][e->x] != '#') e->y++;
            else if(g->player.y < e->y && r->map[e->y-1][e->x] != '#') e->y--;
        }

        if(e->x == g->player.x && e->y == g->player.y)
            g->player.health -= 20;
    }

    if(g->player.health <= 0) g->gameover = true;
    if(g->player.inventory == 0 && g->player.roomindex == 5) g->won = true;
}