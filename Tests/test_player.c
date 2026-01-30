#include "unity.h"
#include "../Tests/mocks/test_rand_stub.h"
#include "../Core/Inc/app/game/game_logic.h"
#include "../Core/Inc/drivers/ssd1327.h"
#include "../Core/Inc/app/input/input.h"

extern GameCtx g;
extern InputSnapshot in;

void test_player_does_not_leave_screen_top_bottom(void)
{
    GameInit(&g);

    g.player.y = PLAYER_Y_MIN - 1;
    in.joystickYValue = JOYSTICK_LOW_THRESH - 10;
    for(int i = 0; i < 4 ; i++){
        GameTick(&g, &in);
    }

    TEST_ASSERT_EQUAL_INT(PLAYER_Y_MIN, g.player.y);

    g.player.y = PLAYER_Y_MAX + 1;
    in.joystickYValue = JOYSTICK_HIGH_THRESH + 10;
    for(int i = 0; i < 4 ; i++){
        GameTick(&g, &in);
    }

    TEST_ASSERT_EQUAL_INT(PLAYER_Y_MAX, g.player.y);
}