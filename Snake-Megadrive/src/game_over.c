#include "states.h"

u8 player_restart;

void init_gameover_params()
{
    player_restart = 0;
}

void gameoverJoyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) {
        if (state & BUTTON_UP)
            player_restart = 1;
    }
}

void game_over()
{
    VDP_resetScreen();
    VDP_init();
    JOY_setEventHandler(&gameoverJoyHandler);
    VDP_drawText("Game OVER", 15, 15);
    init_gameover_params();

    while (TRUE) {
        JOY_update();
        if (player_restart)
            return;
    }
}