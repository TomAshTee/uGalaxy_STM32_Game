#include "unity.h"
#include "../Tests/mocks/test_rand_stub.h"
#include "../Core/Inc/app/game/game_logic.h"

extern GameCtx g;

void test_shot_normal_uses_first_free_slot(void)
{
    GameInit(&g);

    g.player.y = 30;
    g.player.shootType = ST_Normal;

    TEST_ASSERT_FALSE(g.shots[0].active);

    GameShot(&g);

    TEST_ASSERT_TRUE(g.shots[0].active);
    TEST_ASSERT_EQUAL_INT(ST_Normal, g.shots[0].type);
    TEST_ASSERT_EQUAL_INT(SHOT_INIT_X, g.shots[0].x);
    TEST_ASSERT_EQUAL_INT(g.player.y + SHOT_INIT_Y_OFFSET, g.shots[0].y);
}