#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void initGame(struct Game* game)
{
    int i, j, k;
    memset(game, 0, sizeof(struct Game));

    game->currentRoomCount = 8;
    game->numEnemies = 4;
    game->numItems = 3;

    for (i = 0; i < game->currentRoomCount; i++)
    {
        for (j = 0; j < ROOM_HEIGHT; j++)
            for (k = 0; k < ROOM_WIDTH; k++)
                game->rooms[i].map[j][k] = ' ';

        for (k = 0; k < ROOM_WIDTH; k++) {
            game->rooms[i].map[0][k] = '#';
            game->rooms[i].map[ROOM_HEIGHT-1][k] = '#';
        }
        for (j = 0; j < ROOM_HEIGHT; j++) {
            game->rooms[i].map[j][0] = '#';
            game->rooms[i].map[j][ROOM_WIDTH-1] = '#';
        }
        game->rooms[i].itemIndex = -1;
    }

    game->rooms[0].connections[DIR_SOUTH] = &game->rooms[1];
    game->rooms[1].connections[DIR_NORTH] = &game->rooms[0];
    game->rooms[1].connections[DIR_EAST]  = &game->rooms[2];
    game->rooms[2].connections[DIR_WEST]  = &game->rooms[1];
    game->rooms[2].connections[DIR_SOUTH] = &game->rooms[3];
    game->rooms[3].connections[DIR_NORTH] = &game->rooms[2];
    game->rooms[3].connections[DIR_WEST]  = &game->rooms[4];
    game->rooms[4].connections[DIR_EAST]  = &game->rooms[3];
    game->rooms[4].connections[DIR_NORTH] = &game->rooms[5];
    game->rooms[5].connections[DIR_SOUTH] = &game->rooms[4];
    game->rooms[5].connections[DIR_EAST]  = &game->rooms[6];
    game->rooms[6].connections[DIR_WEST]  = &game->rooms[5];
    game->rooms[6].connections[DIR_SOUTH] = &game->rooms[7];
    game->rooms[7].connections[DIR_NORTH] = &game->rooms[6];
    game->rooms[7].connections[DIR_WEST]  = &game->rooms[0];
    game->rooms[0].connections[DIR_EAST]  = &game->rooms[7];

    game->player.roomIndex = 0;
    game->player.x = 3;
    game->player.y = 3;
    game->player.health = 100;
    game->player.inventoryItem = -1;

    game->enemies[0].roomIndex = 2; game->enemies[0].x = 10; game->enemies[0].y = 5;
    game->enemies[0].health = 30; game->enemies[0].type = 0;

    game->enemies[1].roomIndex = 4; game->enemies[1].x = 8; game->enemies[1].y = 7;
    game->enemies[1].health = 30; game->enemies[1].type = 0;

    game->enemies[2].roomIndex = 6; game->enemies[2].x = 12; game->enemies[2].y = 4;
    game->enemies[2].health = 40; game->enemies[2].type = 1;

    game->enemies[3].roomIndex = 7; game->enemies[3].x = 5; game->enemies[3].y = 8;
    game->enemies[3].health = 40; game->enemies[3].type = 1;

    game->items[0].symbol = 'K'; game->items[0].name = "Llave"; game->items[0].effect = 0;
    game->rooms[3].itemIndex = 0; game->rooms[3].itemX = 10; game->rooms[3].itemY = 5;

    game->items[1].symbol = 'P'; game->items[1].name = "Pocion"; game->items[1].effect = 1;
    game->rooms[5].itemIndex = 1; game->rooms[5].itemX = 12; game->rooms[5].itemY = 6;

    game->gameOver = false;
    game->playerWon = false;
}

void renderRoom(const struct Game* game)
{
    int i, j;
    const struct Room* r = &game->rooms[game->player.roomIndex];

    printf("\n=== HABITACION %d ===\n", game->player.roomIndex);

    for (i = 0; i < ROOM_HEIGHT; i++)
    {
        for (j = 0; j < ROOM_WIDTH; j++)
        {
            if (i == game->player.y && j == game->player.x) { printf("@"); continue; }

            int drawn = 0;
            for (int e = 0; e < game->numEnemies; e++)
            {
                if (game->enemies[e].roomIndex == game->player.roomIndex &&
                    game->enemies[e].x == j && game->enemies[e].y == i)
                {
                    printf(game->enemies[e].type == 0 ? "X" : "P");
                    drawn = 1;
                    break;
                }
            }
            if (drawn) continue;

            if (r->itemIndex != -1 && i == r->itemY && j == r->itemX)
            {
                printf("%c", game->items[r->itemIndex].symbol);
                continue;
            }

            printf("%c", r->map[i][j]);
        }
        printf("\n");
    }

    printf("Vida: %d | Inventario: %s\n", game->player.health,
           game->player.inventoryItem == -1 ? "VACIO" : game->items[game->player.inventoryItem].name);
    printf("WASD mover | E recoger/soltar | Q salir\n");
}

void handleInput(struct Game* game)
{
    char key = getchar();

    if (key == 'q' || key == 'Q') { game->gameOver = true; return; }

    int newX = game->player.x;
    int newY = game->player.y;

    if (key == 'w' || key == 'W') newY--;
    if (key == 's' || key == 'S') newY++;
    if (key == 'a' || key == 'A') newX--;
    if (key == 'd' || key == 'D') newX++;

    const struct Room* r = &game->rooms[game->player.roomIndex];

    if (newX > 0 && newX < ROOM_WIDTH-1 && newY > 0 && newY < ROOM_HEIGHT-1 && r->map[newY][newX] != '#')
    {
        game->player.x = newX;
        game->player.y = newY;
    }

    if (game->player.x == 1 && r->connections[DIR_WEST]) {
        game->player.roomIndex = (int)(r->connections[DIR_WEST] - game->rooms);
        game->player.x = ROOM_WIDTH - 3; return;
    }
    if (game->player.x == ROOM_WIDTH-2 && r->connections[DIR_EAST]) {
        game->player.roomIndex = (int)(r->connections[DIR_EAST] - game->rooms);
        game->player.x = 2; return;
    }
    if (game->player.y == 1 && r->connections[DIR_NORTH]) {
        game->player.roomIndex = (int)(r->connections[DIR_NORTH] - game->rooms);
        game->player.y = ROOM_HEIGHT - 3; return;
    }
    if (game->player.y == ROOM_HEIGHT-2 && r->connections[DIR_SOUTH]) {
        game->player.roomIndex = (int)(r->connections[DIR_SOUTH] - game->rooms);
        game->player.y = 2; return;
    }

    if ((key == 'e' || key == 'E') && r->itemIndex != -1 &&
        game->player.x == r->itemX && game->player.y == r->itemY)
    {
        if (game->player.inventoryItem == -1)
        {
            game->player.inventoryItem = r->itemIndex;
            r->itemIndex = -1;
        }
    }
    else if ((key == 'e' || key == 'E') && game->player.inventoryItem != -1)
    {
        r->itemIndex = game->player.inventoryItem;
        r->itemX = game->player.x;
        r->itemY = game->player.y;
        game->player.inventoryItem = -1;
    }
}

void updateEnemies(struct Game* game)
{
    for (int i = 0; i < game->numEnemies; i++)
    {
        struct Enemy* e = &game->enemies[i];
        if (e->health <= 0 || e->roomIndex != game->player.roomIndex) continue;

        const struct Room* r = &game->rooms[e->roomIndex];

        if (e->type == 0)
        {
            int dx = game->player.x - e->x;
            int dy = game->player.y - e->y;
            if (abs(dx) > abs(dy))
            {
                if (dx > 0 && r->map[e->y][e->x+1] != '#') e->x++;
                else if (dx < 0 && r->map[e->y][e->x-1] != '#') e->x--;
            }
            else
            {
                if (dy > 0 && r->map[e->y+1][e->x] != '#') e->y++;
                else if (dy < 0 && r->map[e->y-1][e->x] != '#') e->y--;
            }
        }
        else
        {
            e->patrolStep = (e->patrolStep + 1) % 6;
            if (e->patrolStep < 3)
                if (r->map[e->y][e->x+1] != '#') e->x++;
            else
                if (r->map[e->y][e->x-1] != '#') e->x--;
        }
    }
}

void checkPlayerEnemyCollisions(struct Game* game)
{
    for (int i = 0; i < game->numEnemies; i++)
    {
        struct Enemy* e = &game->enemies[i];
        if (e->health > 0 && e->roomIndex == game->player.roomIndex &&
            e->x == game->player.x && e->y == game->player.y)
        {
            game->player.health -= 20;
            if (game->player.x > 1) game->player.x--;
        }
    }
}

void checkWinCondition(struct Game* game)
{
    if (game->player.inventoryItem == 0 && game->player.roomIndex == 7)
        game->playerWon = true;

    if (game->player.health <= 0)
        game->gameOver = true;
}

void updateGame(struct Game* game)
{
    updateEnemies(game);
    checkPlayerEnemyCollisions(game);
    checkWinCondition(game);
}

void clearScreen(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[2J\033[H");
    #endif
}

void showTitleScreen(void)
{
    clearScreen();
    printf("=======================================\n");
    printf("     DUNGEON CRAWLER\n");
    printf("=======================================\n\n");
    printf("Presiona ENTER para comenzar...\n");
    getchar();
}

void showGameOverScreen(const struct Game* game)
{
    clearScreen();
    printf("=======================================\n");
    printf("              GAME OVER\n");
    printf("=======================================\n\n");
    printf("Presiona ENTER para salir...\n");
    getchar();
}

void showWinScreen(const struct Game* game)
{
    clearScreen();
    printf("=======================================\n");
    printf("           ¡HAS GANADO!\n");
    printf("=======================================\n\n");
    printf("Presiona ENTER para salir...\n");
    getchar();
}