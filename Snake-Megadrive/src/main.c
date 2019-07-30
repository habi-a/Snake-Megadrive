#include <genesis.h>

#include "body.h"
#include "tiles.h"

u8 count = 0;
u8 direction = 0;
u8 steer = 0;
u8 current_level = 1;
u8 level_speed = 5;

void joyHandler(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1) {
        if (state & BUTTON_UP)
        {
            if (direction != down) {
                steer = up;
            }
        }
        if (state & BUTTON_DOWN)
        {
            if (direction != up) {
                steer = down;
            }
        }
        if (state & BUTTON_LEFT)
        {
            if (direction != right) {
                steer = left;
            }
        }
        if (state & BUTTON_RIGHT)
        {
            if (direction != left) {
                steer = right;
            }
        }
    }
}

void steerSnake(Vect2D_s16 *snakeHead)
{
    JOY_update();

    if (count > level_speed) {
        switch (steer) {
        case up:
            snakeHead->y -= 8;
            direction = up;
            break;
        case down:
            snakeHead->y += 8;
            direction = down;
            break;
        case left:
            snakeHead->x -= 8;
            direction = left;
            break;
        case right:
            snakeHead->x += 8;
            direction = right;
            break;
        }
        count = 0;
    }
    else ++count;

}

void updateSnakePosition(Vect2D_s16 *snakeHead, Vect2D_u16 *resolution)
{
    if (snakeHead->x >= resolution->x)
        snakeHead->x = snakeHead->x % resolution->x;
    else if (snakeHead->x < 0)
        snakeHead->x = resolution->x + (snakeHead->x % resolution->x);

    if (snakeHead->y >= resolution->y)
        snakeHead->y = snakeHead->y % resolution->y;
    else if (snakeHead->y < 0)
        snakeHead->y = resolution->y + (snakeHead->y % resolution->y);
}

u8 isSnakeEatFruit(Vect2D_s16 *snakeHead, Vect2D_u16 *fruit)
{
    return ((snakeHead->x == fruit->x) && (snakeHead->y == fruit->y));
}

u8 isSnakeEatHimself(t_body_list *snakeBody, Vect2D_s16 *snakeHead)
{
    t_body_node *current = snakeBody->first;

    while (current != NULL) {
        if ((snakeHead->x / 8 == current->x) && (snakeHead->y / 8 == current->y))
            return (1);
        current = current->next;
    }
    return (0);
}

u8 isFruitOnSnake(t_body_list *snakeBody, Vect2D_u16 *fruit)
{
    t_body_node* current = snakeBody->first;

    while (current != NULL) {
        if ((fruit->x / 8 == current->x) && (fruit->y / 8 == current->y))
            return (1);
        current = current->next;
    }
    return (0);
}

void generateRandomFruitPos(Vect2D_u16 *fruit, t_body_list *snakeBody, Vect2D_s16 *snakeHead, Vect2D_u16 resolution)
{
    while (TRUE) {
        fruit->x = (random() % screenWidth) / 8 * 8;
        fruit->y = (random() % screenHeight) / 8 * 8;

        if (!isSnakeEatFruit(snakeHead, fruit) && !isFruitOnSnake(snakeBody, fruit))
            break;
    }
}

void VDP_drawBody(t_body_list *snakeBody)
{
    t_body_node* current = snakeBody->first;

    while (current != NULL) {
        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL3, 1, 0, 0, TILE_USERINDEX + 1), current->x, current->y);
        current = current->next;
    }
}

int main()
{
    Vect2D_u16 resolution;
    resolution.x = VDP_getScreenWidth();
    resolution.y = VDP_getScreenHeight();

    u16 score = 0;

    Vect2D_s16 snakeHeadLastPosition;
    snakeHeadLastPosition.x = 0;
    snakeHeadLastPosition.y = 0;

    Vect2D_s16 snakeBodyLastPosition;
    snakeBodyLastPosition.x = 0;
    snakeBodyLastPosition.y = 0;

    Vect2D_s16 snakeHead;
    snakeHead.x = (screenWidth / 2) / 8 * 8;
    snakeHead.y = (screenHeight / 2) / 8 * 8;

    t_body_list *snakeBody;
    snakeBody = init_body();

    Vect2D_u16 fruit;
    generateRandomFruitPos(&fruit, snakeBody, &snakeHead, resolution);

    VDP_setPaletteColor(0, 0x2a2);
    VDP_setBackgroundColor(37);

    JOY_setEventHandler(&joyHandler);

    VDP_loadTileData((const u32*)tileSnake, TILE_USERINDEX, 1, CPU);
    VDP_loadTileData((const u32*)tileBody, TILE_USERINDEX + 1, 1, CPU);
    VDP_loadTileData((const u32*)tileFruit, TILE_USERINDEX + 2, 1, CPU);

    VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX + 2), fruit.x / 8, fruit.y / 8);

    while (TRUE)
    {
        switch (current_level)
        {
        case 1:
            level_speed = 5;
            break;

        case 2:
            level_speed = 3;
            break;

        case 3:
            level_speed = 1;
            break;
        }

        steerSnake(&snakeHead);
        updateSnakePosition(&snakeHead, &resolution);

        if (isSnakeEatHimself(snakeBody, &snakeHead))
            return (0); // Game Over

        if (isSnakeEatFruit(&snakeHead, &fruit)) {
            generateRandomFruitPos(&fruit, snakeBody, &snakeHead, resolution);
            VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL1, 1, 0, 0, TILE_USERINDEX + 2), fruit.x / 8, fruit.y / 8);
            push_front_body(snakeBody);
            score++;
        }

        if (count == 0)
            body_follow_head(snakeBody, &snakeHeadLastPosition);

        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0), snakeHeadLastPosition.x, snakeHeadLastPosition.y);
        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, 0), snakeBodyLastPosition.x, snakeBodyLastPosition.y);
        VDP_setTileMapXY(PLAN_B, TILE_ATTR_FULL(PAL3, 1, 0, 0, TILE_USERINDEX), snakeHead.x / 8, snakeHead.y / 8);
        VDP_drawBody(snakeBody);

        snakeHeadLastPosition.x = snakeHead.x / 8;
        snakeHeadLastPosition.y = snakeHead.y / 8;

        if (snakeBody->size > 0) {
            snakeBodyLastPosition.x = snakeBody->first->x;
            snakeBodyLastPosition.y = snakeBody->first->y;
        }
        else {
            snakeBodyLastPosition.x = snakeHeadLastPosition.x;
            snakeBodyLastPosition.y = snakeHeadLastPosition.y;
        }

        char scoreStr[10];
        intToStr(score, scoreStr, 0);
        VDP_drawText("Score:", 20, 0);
        VDP_drawText(scoreStr, 30, 0);

        if (score > 9 && score < 20)
            current_level = 2;
        else if (score > 19)
            current_level = 3;

        char levelStr[1];
        intToStr(current_level, levelStr, 0);
        VDP_drawText("Level:", 1, 0);
        VDP_drawText(levelStr, 10, 0);

        VDP_waitVSync();
    }

    free_body(snakeBody);
    return (0);
}
