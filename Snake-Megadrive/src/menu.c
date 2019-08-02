#include "menu-background.h"
#include "intro.h"
#include "states.h"

u8 player_play;

void init_menu_params()
{
    player_play = 0;
}

void menuJoyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) {
        if (state & BUTTON_START)
            player_play = 1;
    }
}

void menu()
{
    VDP_resetScreen();
    VDP_init();
    JOY_setEventHandler(&menuJoyHandler);
    VDP_drawImage(PLAN_A, &menu_background, 0, 0);
    VDP_drawText("Press START", 15, 15);
    XGM_setLoopNumber(-1);
    SND_setPCM_XGM(64, intro, sizeof(intro));
    SND_startPlayPCM_XGM(64, 2, SOUND_PCM_CH1);
    init_menu_params();

    u16 i = 0;
    while (TRUE) {
        if (i == 2500)
            VDP_clearText(15, 15, 11);
        else if (i == 5000) {
            VDP_drawText("Press START", 15, 15);
            i = 0;
        }
        JOY_update();
        if (player_play) {
            SND_stopPlayPCM_XGM(SOUND_PCM_CH1);
            return;
        }
        i++;
    }
}