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

void test_shot_normal_skip_used_slot(void)
{
    GameInit(&g);

    g.player.y = 30;
    g.player.shootType = ST_Normal;

    g.shots[0].active = true;
    g.shots[1].active = false;

    GameShot(&g);

    TEST_ASSERT_TRUE(g.shots[1].active);
    TEST_ASSERT_EQUAL_INT(ST_Normal, g.shots[1].type);
}

void test_shot_tracker_assigns_track_number_from_rand_to_enemy_and_shot(void)
{
    GameInit(&g);

    g.player.y = 25;
    g.player.shootType = ST_Tracker;

    g.enemies[0].active = true;
    g.enemies[0].trackedByMissile = false;
    g.enemies[0].x = 60;
    g.enemies[0].y = 50;

    static const int rand_number[] = {123,1,2,3};
    rand_set_sequence(rand_number,4);
    rand_reset();

    GameShot(&g);

    TEST_ASSERT_TRUE(g.shots[0].active);
    TEST_ASSERT_EQUAL_INT(ST_Tracker,g.shots[0].type);
    TEST_ASSERT_EQUAL_INT(123, g.shots[0].trackNumber);

    TEST_ASSERT_TRUE(g.enemies[0].active);
    TEST_ASSERT_TRUE(g.enemies[0].trackedByMissile);
    TEST_ASSERT_EQUAL_INT(123, g.enemies[0].trackNumber);
}

void test_shot_tracker_falls_back_to_normal_when_no_enemy_available(void)
{
    GameInit(&g);

    g.player.y = 25;
    g.player.shootType = ST_Tracker;

    for (int i = 0; i < NUMBER_ENEMIES; i++){
        g.enemies[i].active = false;
    }

    GameShot(&g);

    TEST_ASSERT_EQUAL_INT(ST_Normal,g.shots[0].type);
}