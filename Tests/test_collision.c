#include "unity.h"
#include "../Core/Inc/app/game/game_logic.h"

// Testy kolizji
void test_collision_same_position(void) {
    // Dwa obiekty w tym samym miejscu
    TEST_ASSERT_TRUE(Colliding(50, 50, 50, 50));
}

void test_collision_near(void) {
    // Obiekty blisko siebie
    TEST_ASSERT_TRUE(Colliding(0, 0, 5, 5));
}

void test_collision_far(void) {
    // Obiekty daleko
    TEST_ASSERT_FALSE(Colliding(0, 0, 20, 20));
}

void test_collision_negative(void) {
    // Z ujemnymi współrzędnymi
    TEST_ASSERT_TRUE(Colliding(-5, -5, 0, 0));
}
