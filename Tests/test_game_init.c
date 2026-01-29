#include "unity.h"
#include "../Core/Inc/app/game/game_logic.h"

extern GameCtx g;

void test_game_init_player_lives(void) {
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_LIVES, g.player.lives);
}

void test_game_init_player_score(void) {
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_SCORE, g.player.score);
}

void test_game_init_enemies_inactive(void) {
    GameInit(&g);

    // All enemies inactive
    for (int i = 0; i < NUMBER_ENEMIES; i++) {
        TEST_ASSERT_FALSE(g.enemies[i].active);
    }
}

void test_game_init_shots_inactive(void) {
    GameInit(&g);

    // All missiles inactive
    for (int i = 0; i < NUMBER_SHOTS; i++) {
        TEST_ASSERT_FALSE(g.shots[i].active);
    }
}

void test_game_init_player_x_position(void) {
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_X,g.player.x);
}

void test_game_init_player_y_position(void) {
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_Y,g.player.y);
}

void test_game_init_player_level(void){
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_LEVEL,g.player.level);
}

void test_game_init_player_game_progres(void){
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(INITIAL_GAME_PROGRES,g.player.gameProgres);
}

void test_game_init_player_shoot_type(void){
    GameInit(&g);
    TEST_ASSERT_EQUAL_INT(ST_Normal,g.player.shootType);
}
