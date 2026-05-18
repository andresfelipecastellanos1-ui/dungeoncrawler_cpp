#include "game.h"
#include <stdio.h>

int main(void)
{
    struct Game game;

    showTitleScreen();

    initGame(&game);

    while (!game.gameOver && !game.playerWon)
    {
        clearScreen();
        renderRoom(&game);
        handleInput(&game);
        updateGame(&game);
    }

    if (game.playerWon)
        showWinScreen(&game);
    else
        showGameOverScreen(&game);

    return 0;
}