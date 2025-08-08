/*
 * game_logic.h
 *
 *  Created on: 7 lip 2022
 *      Author: Tomasz Jaeschke
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "stdbool.h"
#include "stdint.h"
#include "ssd1327.h"


//#####################################################
//						Constants
//#####################################################

#define num_shots  			35		// Maximum number of shots def.8
#define num_boss_shots 		5		// Number of boss shots !!!! NOT SURE WHY THE GAME CRASHES WITH A DIFFERENT AMOUNT !!! def.5
#define num_enemies  		10		// Maximum number of opponents
#define num_background		250		// Max. background objects (stars) (high RAM consumption) def.40
#define num_background_freq 12		// Background adding frequency (0-100) def.20
#define num_bonus			3		// Max. number of bonuses on the map

#define frequ_bonus		15			// Frequency of bonuses (1-99)
#define duration_bonus	150			// Duration of bonus

#define initial_lives 	10			// Initial number of lives
#define initial_score 	0			// Initial result of the game
#define initial_x		2			// Initial position X of player
#define initial_y		64			// Initial position Y of player
#define initial_level	1			// Initial level of play
#define initial_game_progres 0		// Initial game progress

#define BLACK 			0
#define WHITE 			15
#define screen_width 	SSD1327_WIDTH
#define screen_height 	SSD1327_HEIGHT


//#####################################################
//						Enums
//#####################################################

typedef enum			// Types of opponents
{
	et_diver,
	et_tracker,
	et_bobber,
}enemy_type;

typedef enum			// States of game
{
	st_menu,
	st_playing,
	st_dead,
}gamestate;

typedef enum 			// Type of bonus
{
	bt_live,
	bt_tracker_shoot,
}bonus_type;

typedef enum
{
	st_normal,
	st_tracker,
}shot_type;

//#####################################################
//						Structs
//#####################################################

typedef struct				// Player and its parameters
{
	int x, y;
	int score, high_score;
	int lives;
	int level;
	int game_progres;
	shot_type shoot_type;
	int bonus_duration;
} T_player;

typedef struct				// Player shots, boss shots
{
	bool active;
	int x, y;
	shot_type type;
	int track_number;
}T_shot;

typedef struct				// Background - "stars"
{
	bool active;
	int x;
	int y;
	int next_update;
	int update_delay;
}T_backgrand;

typedef struct				// Opponents
{
	bool active;
	int x;
	int y;
	enemy_type type;
	const uint8_t* bit_map;
	int next_update;
	int update_delay;
	bool tracked_by_missile;
	int track_number;
}T_enemy;

typedef struct 				// Bosses
{
	bool active;
	int x,y;
	int next_update;
	int update_delay;
	int lives;
	int level;
}T_boss;

typedef struct
{
	bool active;
	int x;
	int y;
	bonus_type type;
	const uint8_t* bit_map;
	int next_update;
	int update_delay;
}T_bonus;

//#####################################################
//					Function declarations
//#####################################################

void start_game(void); 							// Starting the game
void add_enemy(void);							// Adding opponents
void run_menu (void);							// Menu operation
void run_game (void);							// The main skeleton of the game sequence
void drow_game(void);							// "Drawing" the game
void update_scene(void);						// Handling of "core game" events
void shot(void);								// Adding a player shot
void boss_shoot(void);							// Adding a boss shot
void play_dead_anim(void);						// Display of animation after death - climate building ;)
void run_dead(void);							// End of game support
void update_lvl(void);							// Player level update
void update_backgrand(void);					// Background handling
void add_background(void);						// Addition of a background unit
bool colliding(int x0, int y0, int x1, int y1);	// Checking for collisions between objects
void update_enemy (T_enemy* enemy);				// Adding an opponent
void add_bonus(int x, int y);					// Adding a bonus on the position of a downed enemy
void update_bonus(void);						// Bonus handling
uint8_t button_pressed (void); 					// Checking whether push-button no.1 is pressed
int joystick_value_y (void);					// Entering the Y axis value of the joystick


#endif /* INC_GAME_LOGIC_H_ */
