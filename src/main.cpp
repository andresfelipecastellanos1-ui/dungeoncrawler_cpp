#include "game.h"
#include <stdio.h>

int main(void)
{
    struct game g;

    printf("=== DUNGEON CRAWLER ===\n\n");
    printf("Presiona ENTER para comenzar...\n");
    getchar();

    initgame(&g);

    while (!g.gameover && !g.won)
    {
        renderroom(&g);
        handleinput(&g);
        updategame(&g);
    }

    if (g.won)
        printf("\n¡GANASTE! Escapaste del dungeon.\n");
    else
        printf("\nGame Over...\n");

    printf("\nPresiona ENTER para salir...\n");
    getchar();

    return 0;
}