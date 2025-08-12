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

#define NUMBER_SHOTS  			25		// Maximum number of shots def.8
#define NUMBER_ENEMIES  		10		// Maximum number of opponents
#define NUMBER_BACKGROUND		80		// Max. background objects (stars) (high RAM consumption) def.40
#define NUMBER_BACKGROUND_FREQ 	12		// Background adding frequency (0-100) def.20
#define NUMBER_BONUS			3		// Max. number of bonuses on the map

#define NUMBER_BOSS_SHOTS 		5		// Number of boss shots !!!! NOT SURE WHY THE GAME CRASHES WITH A DIFFERENT AMOUNT !!! def.5
#define INITIAL_BOSS_X			(SSD1327_WIDTH + 12)	// Initial position of the boss - off screen + size of the graphic
#define INITIAL_BOSS_Y			(SSD1327_HEIGHT / 2)	// Initial boss position in the middle of the screen
#define INITIAL_BOSS_UPDATE_DELAY	3	// Initial boss speed

#define BONUS_FREQUENCY			15		// Frequency of bonuses (1-99)
#define BONUS_DURATION			150		// Duration of bonus

#define INITIAL_LIVES 			3		// Initial number of lives
#define INITIAL_SCORE 			0		// Initial result of the game
#define INITIAL_X				2		// Initial position X of player
#define INITIAL_Y				64		// Initial position Y of player
#define INITIAL_LEVEL			1		// Initial level of play
#define INITIAL_GAME_PROGRES 	0		// Initial game progress

#define BLACK 					0
#define WHITE 					15
#define SCREEN_WIDTH 			SSD1327_WIDTH
#define SCREEN_HEIGHT 			SSD1327_HEIGHT


//#####################################################
//						Enums
//#####################################################

typedef enum			// Types of opponents
{
	ET_Diver,
	ET_Tracker,
	ET_Bobber,
}EnemyType;

typedef enum			// States of game
{
	GS_Menu,
	GS_Playing,
	GS_Dead,
}GameState;

typedef enum 			// Type of bonus
{
	BT_Live,
	BT_TrackerShot,
}BonusType;

typedef enum
{
	ST_Normal,
	ST_Tracker,
}ShotType;

//#####################################################
//						Structs
//#####################################################

typedef struct				// Player and its parameters
{
	int x, y;
	int score, highScore;
	int lives;
	int level;
	int gameProgres;
	ShotType shootType;
	int bonusDuration;
} T_player;

typedef struct				// Player shots, boss shots
{
	bool active;
	int x, y;
	ShotType type;
	int trackNumber;
}T_shot;

typedef struct				// Background - "stars"
{
	bool active;
	int x;
	int y;
	int nextUpdate;
	int updateDelay;
}T_backgrand;

typedef struct				// Opponents
{
	bool active;
	int x;
	int y;
	EnemyType type;
	const uint8_t* bitMap;
	int nextUpdate;
	int updateDelay;
	bool trackedByMissile;
	int trackNumber;
}T_enemy;

typedef struct 				// Bosses
{
	bool active;
	int x,y;
	int nextUpdate;
	int updateDelay;
	int lives;
	int level;
}T_boss;

typedef struct
{
	bool active;
	int x;
	int y;
	BonusType type;
	const uint8_t* bitMap;
	int nextUpdate;
	int updateDelay;
}T_bonus;

typedef struct {			// Context of the current game
	T_player 	player;
	T_shot 		shots[NUMBER_SHOTS];
	T_shot 		bossShots[NUMBER_BOSS_SHOTS];
	T_enemy 	enemies[NUMBER_ENEMIES];
	T_backgrand background[NUMBER_BACKGROUND];
	T_boss		boss;
	T_bonus 	bonuses[NUMBER_BONUS];
	GameState	state;
	uint8_t		btnPrevious;
}GameCtx;

//#####################################################
//					Function declarations
//#####################################################

extern GameCtx g_singleton;						// Game context

void RunMenu (InputSnapshot* in);				// Menu operation
void RunGame (InputSnapshot* in);				// The main skeleton of the game sequence
void PlayDeadAnim(void);						// Display of animation after death - climate building ;)
void RunDead(InputSnapshot* in);				// End of game support

void GameInit(GameCtx* g);						// Starting the game
void GameTick(GameCtx* g, InputSnapshot* in);	// Handling of "core game" events
void GameDraw(GameCtx* g, InputSnapshot* in);	// "Drawing" the game
void GameLevelUpdate(GameCtx* g);				// Player level update
void GameUpdateBackgrand(GameCtx* g);			// Background handling
void GameAddBackground(GameCtx* g);				// Addition of a background unit
void GameAddBonus(GameCtx* g, int x, int y);	// Adding a bonus on the position of a downed enemy
void GameUpdateBonus(GameCtx* g);				// Bonus handling
void GameShot(GameCtx* g);						// Adding a player shot
void GameShotBoss(GameCtx* g);					// Adding a boss shot
bool Colliding(int x0, int y0, int x1, int y1);	// Checking for collisions between objects
void GameAddEnemy(GameCtx* g);					// Adding opponents
void GameSetState(GameCtx* g, GameState state);	// Setting the game state
GameState GameGetState(GameCtx* g);				// Returns the current status of the game
int GameGetPalyerScore(GameCtx* g);				// Returns the player's current score



#endif /* INC_GAME_LOGIC_H_ */
