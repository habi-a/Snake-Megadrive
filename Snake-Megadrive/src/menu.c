#include "states.h"

u8 player_play;

void init_menu_params()
{
    player_play = 0;
}

void menuJoyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) {
        if (state & BUTTON_UP)
            player_play = 1;
    }
}

void menu()
{
    VDP_resetScreen();
    VDP_init();
    JOY_setEventHandler(&menuJoyHandler);
    VDP_drawText("Press START", 15, 15);
    init_menu_params();

    while (TRUE) {
        JOY_update();
        if (player_play)
            return;
    }
}