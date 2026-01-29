#include "unity.h"
#include <string.h>
#include "../Core/Inc/app/game/game_logic.h"

GameCtx g;

// === EXTERN DECLARATIONS ===
extern void test_collision_same_position(void);
extern void test_collision_near(void);
extern void test_collision_far(void);
extern void test_collision_negative(void);

extern void test_game_init_player_lives(void);
extern void test_game_init_player_score(void);
extern void test_game_init_enemies_inactive(void);
extern void test_game_init_shots_inactive(void);
extern void test_game_init_player_x_position(void);
extern void test_game_init_player_y_position(void);
extern void test_game_init_player_level(void);
extern void test_game_init_player_game_progres(void);
extern void test_game_init_player_shoot_type(void);

extern void test_shot_normal_uses_first_free_slot(void);

void setUp(void) {
    memset(&g, 0, sizeof(g));
    
}

void tearDown(void) {
    memset(&g, 0, sizeof(g));
}

// === MAIN - ENTRY POINT ===
int main(void) {
    UNITY_BEGIN();

    // Collisions
    RUN_TEST(test_collision_same_position);
    RUN_TEST(test_collision_near);
    RUN_TEST(test_collision_far);
    RUN_TEST(test_collision_negative);

    // Game Init
    RUN_TEST(test_game_init_player_lives);
    RUN_TEST(test_game_init_player_score);
    RUN_TEST(test_game_init_enemies_inactive);
    RUN_TEST(test_game_init_shots_inactive);
    RUN_TEST(test_game_init_player_x_position);
    RUN_TEST(test_game_init_player_y_position);
    RUN_TEST(test_game_init_player_level);
    RUN_TEST(test_game_init_player_game_progres);
    RUN_TEST(test_game_init_player_shoot_type);

    //Shots
    RUN_TEST(test_shot_normal_uses_first_free_slot);

    return UNITY_END();
}
