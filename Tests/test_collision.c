#include "unity.h"
#include "../Core/Inc/app/game/game_logic.h"

// Collision tests
void test_collision_same_position(void) {
    // Two objects in the same place
    TEST_ASSERT_TRUE(Colliding(50, 50, 50, 50));
}

void test_collision_near(void) {
    // Objects close to each other
    TEST_ASSERT_TRUE(Colliding(0, 0, 5, 5));
}

void test_collision_far(void) {
    // Distant objects
    TEST_ASSERT_FALSE(Colliding(0, 0, 20, 20));
}

void test_collision_negative(void) {
    // With negative coordinates
    TEST_ASSERT_TRUE(Colliding(-5, -5, 0, 0));
}
