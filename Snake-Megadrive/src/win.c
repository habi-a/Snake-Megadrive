#include "states.h"

u8 player_relaunch;

void init_win_params()
{
    player_relaunch = 0;
}

void winJoyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) {
        if (state & BUTTON_UP)
            player_relaunch = 1;
    }
}

void win()
{
    VDP_resetScreen();
    VDP_init();
    JOY_setEventHandler(&winJoyHandler);
    VDP_drawText("You win!", 15, 15);
    init_win_params();

    while (TRUE) {
        JOY_update();
        if (player_relaunch)
            return;
    }
}