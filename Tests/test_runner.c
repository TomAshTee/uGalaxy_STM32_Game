#include "unity.h"
#include <string.h>
#include "../Core/Inc/app/game/game_logic.h"
#include "../Core/Inc/app/input/input.h"

GameCtx g;
InputSnapshot in;

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
extern void test_shot_normal_skip_used_slot(void);
extern void test_shot_tracker_assigns_track_number_from_rand_to_enemy_and_shot(void);
extern void test_shot_tracker_falls_back_to_normal_when_no_enemy_available(void);
extern void test_shot_tracker_hit_the_target(void);

extern void test_player_does_not_leave_screen_top_bottom(void);

extern void test_bonus_enemy_death_spawn_bonus(void);
extern void test_bonus_player_collect_lives(void);

void setUp(void) {
    memset(&g, 0, sizeof(g));
    memset(&in, 0, sizeof(in));
    
}

void tearDown(void) {
    memset(&g, 0, sizeof(g));
    memset(&in, 0, sizeof(in));
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
    RUN_TEST(test_shot_normal_skip_used_slot);
    RUN_TEST(test_shot_tracker_assigns_track_number_from_rand_to_enemy_and_shot);
    RUN_TEST(test_shot_tracker_falls_back_to_normal_when_no_enemy_available);
    RUN_TEST(test_shot_tracker_hit_the_target);

    //Player move
    RUN_TEST(test_player_does_not_leave_screen_top_bottom);

    //Bonus
    RUN_TEST(test_bonus_enemy_death_spawn_bonus);
    RUN_TEST(test_bonus_player_collect_lives);

    return UNITY_END();
}
