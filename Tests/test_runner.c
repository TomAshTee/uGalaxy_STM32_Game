#include "unity.h"

// === EXTERN DECLARATIONS ===
extern void test_collision_same_position(void);
extern void test_collision_near(void);
extern void test_collision_far(void);
extern void test_collision_negative(void);

extern void test_game_init_player_lives(void);
extern void test_game_init_player_score(void);
extern void test_game_init_enemies_inactive(void);
extern void test_game_init_shots_inactive(void);

// === MAIN - ENTRY POINT ===
int main(void) {
    UNITY_BEGIN();

    // Kolizje
    RUN_TEST(test_collision_same_position);
    RUN_TEST(test_collision_near);
    RUN_TEST(test_collision_far);
    RUN_TEST(test_collision_negative);

    // Game Init
    RUN_TEST(test_game_init_player_lives);
    RUN_TEST(test_game_init_player_score);
    RUN_TEST(test_game_init_enemies_inactive);
    RUN_TEST(test_game_init_shots_inactive);

    return UNITY_END();
}
