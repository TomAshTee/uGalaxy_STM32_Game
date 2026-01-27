#include "unity.h"
#include <string.h>
#include "../Core/Inc/app/game/game_logic.h"

static GameCtx g;

void setUp(void) {
    memset(&g, 0, sizeof(g));
}

void tearDown(void) {
    memset(&g, 0, sizeof(g));
}

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

    // Wszystkie wrogowie nieaktywni
    for (int i = 0; i < NUMBER_ENEMIES; i++) {
        TEST_ASSERT_FALSE(g.enemies[i].active);
    }
}

void test_game_init_shots_inactive(void) {
    GameInit(&g);

    // Wszystkie pociski nieaktywne
    for (int i = 0; i < NUMBER_SHOTS; i++) {
        TEST_ASSERT_FALSE(g.shots[i].active);
    }
}
