#include "unity.h"
#include "../Tests/mocks/test_rand_stub.h"
#include "../Core/Inc/app/game/game_logic.h"
#include "../Core/Inc/app/input/input.h"

extern GameCtx g;
extern InputSnapshot in;

void test_bonus_enemy_death_spawn_bonus(void)
{
    g.enemies[0].active = true;
    g.enemies[0].type = ET_Diver;
    g.enemies[0].x = 60;
    g.enemies[0].y = 60;

    g.shots[0].active = true;
    g.shots[0].type = ST_Normal;
    g.shots[0].x = 60;
    g.shots[0].y = 60;

    static const int rand_number[] = {0,99,99,99,99,99,99,99,99};
    rand_set_sequence(rand_number,9);
    rand_reset();

    GameTick(&g, &in);

    TEST_ASSERT_FALSE(g.enemies[0].active);
    TEST_ASSERT_FALSE(g.shots[0].active);
    TEST_ASSERT_TRUE(g.bonuses[0].active);
}

void test_bonus_player_collect_lives(void)
{
    GameInit(&g); 

    g.bonuses[0].active = true;
    g.bonuses[0].type = BT_Live;
    g.bonuses[0].x = 60;
    g.bonuses[0].y = 60;
    g.bonuses[0].updateDelay = 0;
    g.bonuses[0].nextUpdate = 0;

    g.player.x = 60;
    g.player.y = 60;
    
    static const int rand_number[] = {0};
    rand_set_sequence(rand_number,1);
    rand_reset();

    int lives_before = g.player.lives;

    GameTick(&g,&in);

    TEST_ASSERT_EQUAL_INT(lives_before + 1, g.player.lives);
    TEST_ASSERT_FALSE(g.bonuses[0].active);

}