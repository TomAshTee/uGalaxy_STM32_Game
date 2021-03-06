/*
 * graphics_map.c
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "graphics_map.h"

//-------------- GRAFIKI -------------------

//Grafika pocisku gracza 4x1
const uint8_t player_shot_map[]  = {
	0xFF,
};

//grafika  graczacza 11x11
const uint8_t player_map[]  = {
	0xF8,0x1F,0x60,0x1F,0x3F,0x1F,0x6C,0x1F,0x63,0x1F,0x9D,0xFF,0x63,0x1F,0x6C,0x1F,
	0x3F,0x1F,0x60,0x1F,0xF8,0x1F,
};

//Napis "Best: " 27x10
const uint8_t Best_map[]  = {
	0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x1F,0x78,0x00,0x10,0x1F,0x44,0x00,0x10,0x1F,
	0x44,0xE3,0xB9,0x1F,0x49,0x14,0x10,0x1F,0x79,0x22,0x10,0x1F,0x45,0xC1,0x10,0x1F,
	0x45,0x10,0x91,0x1F,0x78,0xE7,0x10,0x1F,
};

//Grafika Napisu "Level" 20x7
const uint8_t Level_map[]  = {
	0x80,0x00,0x1F,0x80,0x00,0x1F,0x83,0xA9,0xDF,0x84,0xAA,0x5F,0x87,0x2B,0x9F,0x84,
	0x92,0x5F,0xF3,0x11,0x9F,
};

//Serce grafika 8x6
const uint8_t lives_map[]  = {
	0x66,0xFF,0xFF,0x7E,0x3C,0x18,
};

//Napis "Score: " 37x10
const uint8_t Score_map[]  = {
	0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x07,0x1E,0x00,0x00,0x00,0x07,0x20,
	0x00,0x00,0x00,0x07,0x20,0x31,0xC7,0x38,0x87,0x1C,0x4A,0x25,0x44,0x07,0x02,0x82,
	0x24,0x48,0x07,0x02,0x82,0x24,0x70,0x07,0x42,0x8A,0x24,0x44,0x87,0x3C,0x71,0xC4,
	0x38,0x07,
};

//Napis "Defeated' 67x16
const uint8_t Defeated_map[]  = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x70,0x00,
	0x00,0x00,0x00,0x7F,0x60,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x7F,0x7C,0x00,0x01,
	0xC0,0x00,0x03,0x00,0x00,0x7F,0x7F,0x00,0x01,0x80,0x00,0x03,0x00,0x00,0x7F,0x63,
	0x80,0x01,0x80,0x00,0x03,0x00,0x00,0x7F,0x60,0xC3,0xC7,0xE3,0xC3,0xEF,0xC7,0x87,
	0xFF,0x60,0x67,0xE7,0xE7,0xE7,0xEF,0xCF,0xCF,0xFF,0x60,0x6E,0x61,0x8E,0x6E,0x63,
	0x1C,0xDC,0x7F,0x60,0x6C,0xC1,0x8C,0xCC,0x63,0x19,0x98,0x7F,0x60,0x6D,0x81,0x8D,
	0x8C,0x63,0x1B,0x18,0x7F,0x61,0xCE,0x21,0x8E,0x2C,0x63,0x1C,0x5C,0x7F,0x7F,0x8F,
	0xE1,0x8F,0xEF,0xE3,0x1F,0xCF,0xFF,0x3F,0x03,0xC1,0x83,0xC7,0xB3,0x07,0x87,0xFF,
};

// napis "uGalaxy"54x16
const uint8_t uGalaxy_map[] = {
	0x00,0x00,0x00,0x30,0x00,0x00,0x03,0x00,0x1C,0x00,0x30,0x00,0x00,0x03,0x00,0x3F,
	0x00,0x30,0x00,0x00,0x03,0x00,0x63,0x00,0x30,0x00,0x00,0x03,0xCC,0xC0,0x1E,0x31,
	0xE7,0x1B,0x0F,0xCC,0xC0,0x3F,0x33,0xF7,0xBB,0x9F,0xCD,0x9F,0xF3,0x37,0x31,0xF1,
	0x9B,0xCD,0xBF,0xE3,0x36,0x30,0xE1,0xBB,0xCD,0x81,0xE3,0x36,0x31,0xE0,0xF3,0xCD,
	0x87,0x63,0x36,0x33,0xF0,0xF3,0xFC,0xFE,0x7F,0x37,0xF7,0x38,0xE3,0x7C,0x7C,0x3F,
	0x33,0xF6,0x18,0xE3,0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0x00,0x00,0x00,0x00,0x00,
	0x00,0xC3,0x00,0x00,0x00,0x00,0x00,0x01,0x83,0x00,0x00,0x00,0x00,0x00,0x01,0x83,
};

// Napis "Press to start" 66x10
const uint8_t PressToStart_map[]  = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x3F,0x70,0x00,0x00,0x01,0x00,0x00,0x40,0x00,0xBF,0x48,0x00,0x00,0x01,0x00,
	0x00,0x40,0x00,0xBF,0x49,0xCE,0x39,0xC3,0x9C,0x0E,0xE3,0x9D,0xFF,0x49,0x51,0x42,
	0x01,0x22,0x10,0x44,0x94,0xBF,0x49,0x12,0x21,0x01,0x22,0x08,0x48,0x90,0xBF,0x71,
	0x1C,0x10,0x81,0x22,0x04,0x48,0x90,0xBF,0x41,0x11,0x08,0x41,0x22,0x02,0x48,0x90,
	0xBF,0x41,0x0E,0x73,0x81,0x1C,0x1C,0x47,0x50,0xBF,
};

//10x16
const uint8_t player_shield_map[]  = {
	0x78,0x3F,0x84,0x3F,0x02,0x3F,0x02,0x3F,0x01,0x3F,0x00,0xBF,0x00,0xBF,0x00,0x7F,
	0x00,0x7F,0x00,0xBF,0x00,0xBF,0x01,0x3F,0x02,0x3F,0x02,0x3F,0x84,0x3F,0x78,0x3F,
};

//5x5
const uint8_t tracker_map[]  = {
	0xDF,0x57,0xFF,0x8F,0x57,
};

//3x7
const uint8_t driver_map[]  = {
	0x3F,0x5F,0xFF,0x9F,0xFF,0x5F,0x3F,
};

//5x5
const uint8_t bobber_map[]  = {
	0x57,0x8F,0xFF,0x8F,0x57,
};

//10x10
const uint8_t explosion_map[]  = {
	0x08,0x7F,0x88,0xBF,0x5F,0x3F,0x39,0xBF,0x25,0xBF,0x69,0xBF,0x7F,0x3F,0x7E,0x3F,
	0x6C,0xBF,0x80,0x7F,
};

//10x18
const uint8_t boss_map_1[]  = {
	0x1F,0xFF,0x00,0xFF,0x01,0xBF,0x03,0x3F,0x0E,0x3F,0x0C,0x3F,0x1F,0xBF,0x3F,0x3F,
	0xFC,0x3F,0xFC,0x3F,0x3F,0x3F,0x1F,0xBF,0x0C,0x3F,0x0E,0x3F,0x03,0x3F,0x01,0xBF,
	0x00,0xFF,0x1F,0xFF,
};

//10x18
const uint8_t boss_map_2[]  = {
	0x12,0x3F,0xFF,0xFF,0x61,0xBF,0x33,0x3F,0x1E,0x3F,0x0C,0x3F,0x0C,0x3F,0x1E,0x3F,
	0xF3,0xFF,0x73,0xBF,0x7F,0xBF,0x52,0xBF,0x5E,0xBF,0x52,0xBF,0xC0,0xFF,0x40,0xBF,
	0x21,0x3F,0x12,0x3F,
};

//20x20
const uint8_t test_map[]  = {
		0xFF,0xFF,0xFF,0xE0,0x00,0x7F,0xB0,0xF0,0xDF,0x98,0xF1,0x9F,0x8C,0x01,0x1F,0x84,
			0x02,0x1F,0x83,0x06,0x1F,0x81,0x0C,0x1F,0x9C,0x99,0xFF,0x94,0x51,0x3F,0x94,0x21,
			0x3F,0x9C,0x71,0xFF,0x80,0xCC,0x1F,0x80,0x86,0x1F,0x81,0x03,0x1F,0x86,0x79,0x9F,
			0x8C,0x48,0xDF,0xB8,0x78,0x7F,0xF0,0x00,0x3F,0xFF,0xFF,0xFF,
};

//7x7
const uint8_t bonus_live_map[]  = {
		0xC7,0xAB,0x7D,0x7D,0x39,0x93,0xC7,
};

//7x7
const uint8_t bonus_tracker_shoot_map[]  = {
		0xD7,0x93,0x11,0xFF,0x11,0x93,0xD7,
};
