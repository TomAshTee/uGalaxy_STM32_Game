#include <stdint.h>

/* ---- GFX function stubs (PC unit tests) ---- */
void GFX_PutInt(int x, int y, int value, int color)
{
    (void)x; (void)y; (void)value; (void)color;
}

void GFX_DrawBitMap_P(int x, int y, const uint8_t *bmp, int w, int h, int color)
{
    (void)x; (void)y; (void)bmp; (void)w; (void)h; (void)color;
}

/* ---- SSD1327 stubs ---- */
void SSD1327_SetPixel(int x, int y, uint8_t color)
{
    (void)x; (void)y; (void)color;
}

/* ---- Bitmap data stubs (referenced by game_logic.c) ---- */
const uint8_t bobber_map[1] = {0};
const uint8_t tracker_map[1] = {0};
const uint8_t driver_map[1] = {0};
const uint8_t explosion_map[1] = {0};
const uint8_t bonus_tracker_shoot_map[1] = {0};
const uint8_t bonus_live_map[1] = {0};
const uint8_t boss_map_2[1] = {0};
const uint8_t boss_map_1[1] = {0};
const uint8_t player_map[1] = {0};
const uint8_t player_shield_map[1] = {0};
const uint8_t player_shot_map[1] = {0};
const uint8_t Level_map[1] = {0};
const uint8_t lives_map[1] = {0};
