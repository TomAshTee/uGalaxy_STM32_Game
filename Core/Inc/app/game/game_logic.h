/**
 * @file    game_logic.h
 * @author  Tomasz Jaeschke
 * @date    2022-07-07
 * @brief   Core game logic API for uGalaxy STM32 Game.
 * @details
 * This module contains all core definitions, constants, structures,
 * enumerations, and public API functions that control the gameplay loop,
 * player behavior, enemy spawning, boss fights, bonuses, and background updates.
 *
 * The functions declared here are implemented in `game_logic.c` and interact
 * with input handling, graphics rendering, and hardware drivers to create
 * a complete game experience on the SSD1327 OLED display.
 *
 * Key features:
 * - Player, enemy, boss, and bonus management
 * - Collision detection
 * - Level progression system
 * - Modular separation of game logic from hardware
 */

#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "stdbool.h"
#include "stdint.h"
#include "app/input/input.h"


/** ##########################################################################
 *  @name Constants
 *  @brief Gameplay constants and limits.
 *  @{
 *  ##########################################################################
 */

#define NUMBER_SHOTS  			25					/**<  Maximum number of shots def.8 */
#define NUMBER_ENEMIES  		10					/**<  Maximum number of opponents */
#define NUMBER_BACKGROUND		40					/**<  Max. background objects (stars) (high RAM consumption) def.40 */
#define NUMBER_BACKGROUND_FREQ 	12					/**<  Background adding frequency (0-100) def.20 */
#define NUMBER_BONUS			3					/**<  Max. number of bonuses on the map */

#define NUMBER_EXPLOSION		NUMBER_ENEMIES + 2	/**< Max. number of explosion on the map - every enemy + boss + player*/
#define EXPLOSION_TIMER			20					/**< Duration of explosion */

#define NUMBER_BOSS_SHOTS 		5					/**<  Number of boss shots !!!! NOT SURE WHY THE GAME CRASHES WITH A DIFFERENT AMOUNT !!! def.5 */
#define INITIAL_BOSS_X			(SSD1327_WIDTH + 12)/**<  Initial position of the boss - off screen + size of the graphic */
#define INITIAL_BOSS_Y			(SSD1327_HEIGHT / 2)/**<  Initial boss position in the middle of the screen */
#define INITIAL_BOSS_UPDATE_DELAY	3				/**<  Initial boss speed */

#define BONUS_FREQUENCY			15					/**<  Frequency of bonuses (1-99)  */
#define BONUS_DURATION			150					/**<  Duration of bonus  */

#define INITIAL_LIVES 			5					/**<  Initial number of lives  */
#define INITIAL_SCORE 			0					/**<  Initial result of the game */
#define INITIAL_X				2					/**<  Initial position X of player */
#define INITIAL_Y				64					/**<  Initial position Y of player */
#define INITIAL_LEVEL			1					/**<  Initial level of play */
#define INITIAL_GAME_PROGRES 	0					/**<  Initial game progress */

#define BLACK 					0
#define WHITE 					15
#define SCREEN_WIDTH 			SSD1327_WIDTH
#define SCREEN_HEIGHT 			SSD1327_HEIGHT

#define JOYSTICK_LOW_THRESH     1000				/** Lower joystick threshold for downward movement */
#define JOYSTICK_HIGH_THRESH    3500				/** Upper joystick threshold for upward movement */

#define PLAYER_Y_MIN            10					/** Minimum vertical position of the player */
#define PLAYER_Y_MAX            (SCREEN_HEIGHT - 14)/** Maximum vertical position of the player */

#define BOSS_Y_MIN              1					/** Minimum vertical position of the boss sprite */
#define BOSS_Y_MAX              (SCREEN_HEIGHT - 24)/** Maximum vertical position of the boss sprite */

#define ENEMY_OFFSCREEN_X       (-4)				/** X position at which enemies are considered off-screen */

#define SHOT_INIT_X             11					/** Initial X position of the player's shot */
#define SHOT_INIT_Y_OFFSET      5					/** Vertical offset for spawning player's shot */

#define STAR_Y_MARGIN           10					/** Vertical margin for starfield random placement */
#define ENEMIES_Y_MARGIN        10					/** Vertical margin for enemies random placement */


/** @} */

/** ##########################################################################
 *  @name Enums
 *  @brief Enumerations for game states and types.
 *  @{
 *  ##########################################################################
 */

typedef enum			/**<  Types of opponents */
{
	ET_Diver,
	ET_Tracker,
	ET_Bobber,
}EnemyType;

typedef enum			/**<  States of game */
{
	GS_Menu,
	GS_Playing,
	GS_Dead,
}GameState;

typedef enum 			/**<  Type of bonus */
{
	BT_Live,
	BT_TrackerShot,
}BonusType;

typedef enum
{
	ST_Normal,
	ST_Tracker,
}ShotType;

/** @} */

/** ##########################################################################
 *  @name Structs
 *  @brief Game struct and main game context.
 *  @{
 *  ##########################################################################
 */

typedef struct				/**<  Player and its parameters */
{
	int x, y;
	int score, highScore;
	int lives;
	int level;
	int gameProgres;
	ShotType shootType;
	int bonusDuration;
	int shieldDuration;
} T_player;

typedef struct				/**<  Player shots, boss shots */
{
	bool active;
	int x, y;
	ShotType type;
	int trackNumber;
}T_shot;

typedef struct				/**<  Background - "stars" */
{
	bool active;
	int x;
	int y;
	int nextUpdate;
	int updateDelay;
}T_background;

typedef struct				/**<  Opponents structures */
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

typedef struct 				/**<  Bosses structures */
{
	bool active;
	int x,y;
	int nextUpdate;
	int updateDelay;
	int lives;
	int level;
}T_boss;

typedef struct				/**<  Bonus structures */
{
	bool active;
	int x;
	int y;
	BonusType type;
	const uint8_t* bitMap;
	int nextUpdate;
	int updateDelay;
}T_bonus;

typedef struct				/**<  Explosion structures */
{
	bool active;
	int explosionTimer;
	int x, y;

}T_explosion;

typedef struct {			/**<  Context of the current game */
	T_player 	player;
	T_shot 		shots[NUMBER_SHOTS];
	T_shot 		bossShots[NUMBER_BOSS_SHOTS];
	T_enemy 	enemies[NUMBER_ENEMIES];
	T_background background[NUMBER_BACKGROUND];
	T_boss		boss;
	T_bonus 	bonuses[NUMBER_BONUS];
	T_explosion explosion[NUMBER_EXPLOSION];
	GameState	state;
	uint8_t		btnPrevious;
}GameCtx;

/** @} */

/** ##########################################################################
 *  @name Function Declarations
 *  @brief Public API for controlling game flow.
 *  @{
 *  ##########################################################################
 */

extern GameCtx g_singleton;						/**<  Game context */

void RunMenu (InputSnapshot* in);				/**<  Drawing Menu  */
void RunGame (InputSnapshot* in);				/**<  Drawing Game */
void RunDead(InputSnapshot* in);				/**<  Drawing end game animation */
void RunMenuTick(InputSnapshot* in);			/**<  Execution of logic-> Menu*/
void RunGameTick(InputSnapshot* in, GameCtx* g);/**<  Execution of logic-> Game*/
void RunDeadTick(InputSnapshot* in);			/**<  Execution of logic-> Dead*/
void PlayDeadAnim(void);						/**<  Display of animation after death - climate building ;) */


void GameInit(GameCtx* g);						/**<  Starting the game */
void GameTick(GameCtx* g, InputSnapshot* in);	/**<  Handling of "core game" events */
void GameDraw(GameCtx* g, InputSnapshot* in);	/**<  "Drawing" the game */
void GameLevelUpdate(GameCtx* g);				/**<  Player level update */
void GameUpdateBackground(GameCtx* g);			/**<  Background handling */
void GameAddBackground(GameCtx* g);				/**<  Addition of a background unit */
void GameAddBonus(GameCtx* g, int x, int y);	/**<  Adding a bonus on the position of a downed enemy */
void GameUpdateBonus(GameCtx* g);				/**<  Bonus handling */
void GameShot(GameCtx* g);						/**<  Adding a player shot */
void GameShotBoss(GameCtx* g);					/**<  Adding a boss shot */
bool Colliding(int x0, int y0, int x1, int y1);	/**<  Checking for collisions between objects */
void GameAddEnemy(GameCtx* g);					/**<  Adding opponents */
void GameSetState(GameCtx* g, GameState state);	/**<  Setting the game state */
GameState GameGetState(GameCtx* g);				/**<  Returns the current status of the game */
int GameGetPlayerScore(GameCtx* g);				/**<  Returns the player's current score */
void GameUpdateExplosion(GameCtx* g);			/**<  Updates explosions, deactivates when timer expires */

/** @} */

#endif /* INC_GAME_LOGIC_H_ */
