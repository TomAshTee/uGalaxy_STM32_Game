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
#include "input.h"


//#####################################################
//						Constants
//#####################################################

#define num_shots  			25		// Maximum number of shots def.8
#define num_enemies  		10		// Maximum number of opponents
#define num_background		80		// Max. background objects (stars) (high RAM consumption) def.40
#define num_background_freq 12		// Background adding frequency (0-100) def.20
#define num_bonus			3		// Max. number of bonuses on the map

#define num_boss_shots 		5		// Number of boss shots !!!! NOT SURE WHY THE GAME CRASHES WITH A DIFFERENT AMOUNT !!! def.5
#define initial_boss_x		(SSD1327_WIDTH + 12)	// Initial position of the boss - off screen + size of the graphic
#define initial_boss_y		(SSD1327_HEIGHT / 2)	// Initial boss position in the middle of the screen
#define initial_boss_update_delay	3	// Initial boss speed

#define frequ_bonus		15			// Frequency of bonuses (1-99)
#define duration_bonus	150			// Duration of bonus

#define initial_lives 	3			// Initial number of lives
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

typedef struct {			// Context of the current game
	T_player 	player;
	T_shot 		shots[num_shots];
	T_shot 		boss_shots[num_boss_shots];
	T_enemy 	enemies[num_enemies];
	T_backgrand background[num_background];
	T_boss		boss;
	T_bonus 	bonuses[num_bonus];
	gamestate	state;
	uint8_t		btn_prev;
}GameCtx;

//#####################################################
//					Function declarations
//#####################################################

extern GameCtx g_singleton;						// Game context

void RunMenu (InputSnapshot* in);							// Menu operation
void RunGame (InputSnapshot* in);							// The main skeleton of the game sequence
void PlayDeadAnim(void);						// Display of animation after death - climate building ;)
void RunDead(InputSnapshot* in);							// End of game support

void GameInit(GameCtx* g);						// Starting the game
void GameTick(GameCtx* g, InputSnapshot* in);						// Handling of "core game" events
void GameDraw(GameCtx* g, InputSnapshot* in);						// "Drawing" the game
void GameLevelUpdate(GameCtx* g);				// Player level update
void GameUpdateBackgrand(GameCtx* g);			// Background handling
void GameAddBackground(GameCtx* g);			// Addition of a background unit
void GameAddBonus(GameCtx* g, int x, int y);	// Adding a bonus on the position of a downed enemy
void GameUpdateBonus(GameCtx* g);				// Bonus handling
void GameShot(GameCtx* g);						// Adding a player shot
void GameShotBoss(GameCtx* g);				// Adding a boss shot
bool Colliding(int x0, int y0, int x1, int y1);	// Checking for collisions between objects
void GameAddEnemy(GameCtx* g);				// Adding opponents
void GameSetState(GameCtx* g, gamestate state);	// Setting the game state
gamestate GameGetState(GameCtx* g);			// Returns the current status of the game
int GameGetPalyerScore(GameCtx* g);			// Returns the player's current score



#endif /* INC_GAME_LOGIC_H_ */
