#include "states.h"

int main()
{
    u8 is_win;

    while (TRUE) {
        menu();
        is_win = play();

        switch (is_win) {
        case 0:
            game_over();
            break;
        case 1:
            win();
            break;
        case 2:
            return (1);
        }
    }
}
