/*
 * game_logic.c
 *
 *  Created on: Aug 8, 2025
 *      Author: Tomasz
 */

#include "game_logic.h"
#include "graphics_map.h"
#include "GFX_ssd1327.h"
#include <stdlib.h>
#include <math.h>

GameCtx g_singleton;

void GameInit(GameCtx *g) {
	/*
	 * This function is only called once at the start of the game.
	 * It has the task of setting the starting parameters for
	 * the player and the first boss. Furthermore, it "resets"
	 * the arrays of shot, opponents and bonuses.
	 */
	uint8_t i;
	//Player initial settings
	g->player.lives = INITIAL_LIVES;
	g->player.score = INITIAL_SCORE;
	g->player.x = INITIAL_X;
	g->player.y = INITIAL_Y;
	g->player.level = INITIAL_LEVEL;
	g->player.gameProgres = INITIAL_GAME_PROGRES;
	g->player.shootType = ST_Normal;

	//Deactivation of player shots
	for (i = 0; i < NUMBER_SHOTS; ++i)
		g->shots[i].active = false;

	//Deactivation of boss shots
	for (i = 0; i < NUMBER_BOSS_SHOTS; ++i)
		g->bossShots[i].active = false;

	//Deactivation enemies
	for (i = 0; i < NUMBER_ENEMIES; i++) {
		g->enemies[i].active = false;
		g->enemies[i].trackNumber = 0;
		g->enemies[i].trackedByMissile = false;
	}

	//Deactivation bonuses
	for (i = 0; i < NUMBER_BONUS; i++)
		g->bonuses[i].active = false;

	//Deactivation boss
	g->boss.active = false;
	g->boss.x = INITIAL_BOSS_X;
	g->boss.y = INITIAL_BOSS_Y;
	g->boss.updateDelay = INITIAL_BOSS_UPDATE_DELAY;

	//Deactivation of explosion
	for(i = 0; i < NUMBER_EXPLOSION; i++)
		g->explosion[i].active = false;

}

void GameTick(GameCtx *g, InputSnapshot* in) {
	/*
	 * The logic of the whole game
	 */
	uint8_t i, j, k;

	static uint8_t y = 0, dy = 1;
	// Read analog stick
	int stick = in->joystickYValue;

	if (stick < 1000)
		g->player.y -= 1;
	else if (stick > 3500)
		g->player.y += 1;

	// Keeping the player within the screen
	if (g->player.y < 10)
		g->player.y = 10;
	if (g->player.y > (SCREEN_HEIGHT - 14))
		g->player.y = (SCREEN_HEIGHT - 14);

	// Shifting shots forward
	bool shoot_updated = false;

	for (i = 0; i < NUMBER_SHOTS; ++i) {
		switch (g->shots[i].type) {
		case ST_Normal:
			if (g->shots[i].active)
				g->shots[i].x++;
			if (g->shots[i].x > SCREEN_WIDTH)
				g->shots[i].active = false;
			break;
		case ST_Tracker:

			for (int j = 0; j < NUMBER_ENEMIES; j++) {
				if (g->shots[i].trackNumber == g->enemies[j].trackNumber) {
					if (g->shots[i].x > g->enemies[j].x)
						g->shots[i].x -= 2;
					if (g->shots[i].x < g->enemies[j].x)
						g->shots[i].x += 2;
					if (g->shots[i].y > g->enemies[j].y)
						g->shots[i].y -= 2;
					if (g->shots[i].y < g->enemies[j].y)
						g->shots[i].y += 2;
					shoot_updated = true;
					break;
				}
			}
			//Remove tracking missiles that have no target
			if (!shoot_updated && g->shots[i].type == ST_Tracker) {
				g->shots[i].active = false;
				g->shots[i].trackNumber = 0;
				shoot_updated = false;
			}

			//Remove off-map shots
			if (g->shots[i].x > SCREEN_WIDTH) {
				g->shots[i].active = false;
				g->shots[i].trackNumber = 0;
			}
			break;
		}

	}

	//Remove the markers on enemies whose shots have been used on others
	bool is_there_a_missile;
	for (i = 0; i < NUMBER_ENEMIES; i++) {
		is_there_a_missile = false;

		for (j = 0; j < NUMBER_SHOTS; j++) {
			if (g->enemies[i].trackNumber == g->shots[j].trackNumber)
				is_there_a_missile = true;
		}

		if (!is_there_a_missile)
			g->enemies[i].trackNumber = 0;
	}

	// Updated enemies
	for (i = 0; i < NUMBER_ENEMIES; ++i) {

		if (g->enemies[i].active) {
			g->enemies[i].nextUpdate -= 1;
			if (g->enemies[i].nextUpdate <= 0) {
				if (g->enemies[i].active) {

					g->enemies[i].nextUpdate = g->enemies[i].updateDelay;

					//Checking for collisions between opponents and the player
					if (Colliding(g->enemies[i].x, g->enemies[i].y, g->player.x,
							g->player.y)
							|| Colliding(g->enemies[i].x, g->enemies[i].y,
									g->player.x, g->player.y + 5)
							|| Colliding(g->enemies[i].x, g->enemies[i].y,
									g->player.x + 7, g->player.y)
							|| Colliding(g->enemies[i].x, g->enemies[i].y,
									g->player.x + 7, g->player.y + 5)) {
						g->player.lives -= 1;
						;
						g->enemies[i].active = false;
						g->enemies[i].trackedByMissile = false;
						g->enemies[i].trackNumber = 0;

//						GFX_DrowBitMap_P(g->enemies[i].x + 2, g->enemies[i].y,
//								explosion_map, 10, 10, 1);
						for (j = 0; j < NUMBER_EXPLOSION; j++){
							if (!g->explosion[j].active){
								g->explosion[j].active = true;
								g->explosion[j].x = g->enemies[i].x + 2;
								g->explosion[j].y = g->enemies[i].y;
								g->explosion[j].explosionTimer = EXPLOSION_TIMER;

								break;
							}
						}
						GFX_DrowBitMap_P(g->player.x + 8, g->player.y - 2,
								player_shield_map, 10, 16, 1);
						GFX_DrowBitMap_P(g->player.x, g->player.y, player_map,
								11, 11, 1);

						//SSD1327_Display();
						if (g->player.lives <= 0) {
							PlayDeadAnim();
							g->state = GS_Dead;
						}
					}

					// Moving to the left and making special moves
					g->enemies[i].x -= 1;

					switch (g->enemies[i].type) {
					case ET_Tracker:
						if (g->enemies[i].x < 70) {
							if (g->player.y > g->enemies[i].y)
								g->enemies[i].y += 1;
							if (g->player.y < g->enemies[i].y)
								g->enemies[i].y -= 1;
						}
						break;
					case ET_Diver:
						break;
					case ET_Bobber:
						if ((g->enemies[i].x % 4 == 0)
								&& (g->enemies[i].x % 8 == 0))
							g->enemies[i].y += 4;
						if ((g->enemies[i].x % 4 == 0)
								&& !(g->enemies[i].x % 8 == 0))
							g->enemies[i].y -= 4;
						if (g->enemies[i].x < 70) {
							if (g->player.y > g->enemies[i].y)
								g->enemies[i].y += 1;
							if (g->player.y < g->enemies[i].y)
								g->enemies[i].y -= 1;
						}
						break;
					}

					// If off-screen, deactivation
					if (g->enemies[i].x < -4) {
						g->enemies[i].active = false;
						g->enemies[i].trackedByMissile = false;
						g->enemies[i].trackNumber = 0;
					}
				}
			}
		}
	}

	//------------- Boss service ---------------
	if (g->boss.active) {
		g->boss.nextUpdate -= 1;

		//Boss position
		if (g->boss.nextUpdate <= 0) {
			g->boss.nextUpdate = g->boss.updateDelay;

			y += dy;
			if (y < 1 || y > (SCREEN_HEIGHT - 24))
				dy = -dy;

			g->boss.y = y;

			if (g->boss.y < 10)
				g->boss.y = 10;
			if (g->boss.y > (SCREEN_HEIGHT - 24))
				g->boss.y = (SCREEN_HEIGHT - 24);

			g->boss.x -= 1;
			if (g->boss.x < 100)
				g->boss.x = 100;
		}

		//Frequency of boss shots
		if ((rand() % 100) < (g->boss.level * 5))
			GameShotBoss(g);


		for (i = 0; i < NUMBER_BOSS_SHOTS; ++i) {
			if (g->bossShots[i].active)
				g->bossShots[i].x--;
			if (g->bossShots[i].x < -4)
				g->bossShots[i].active = false;
		}

		for (i = 0; i < NUMBER_BOSS_SHOTS; i++) {
			if (g->bossShots[i].active) {
				if (Colliding(g->bossShots[i].x, g->bossShots[i].y,
						g->player.x, g->player.y)
						|| Colliding(g->bossShots[i].x, g->bossShots[i].y,
								g->player.x, g->player.y + 5)
						|| Colliding(g->bossShots[i].x, g->bossShots[i].y,
								g->player.x + 7, g->player.y)
						|| Colliding(g->bossShots[i].x, g->bossShots[i].y,
								g->player.x + 7, g->player.y + 5)) {
					g->player.lives -= 1;
					;
					g->bossShots[i].active = false;
//					GFX_DrowBitMap_P(g->bossShots[i].x + 2, g->bossShots[i].y,
//							explosion_map, 10, 10, 1);

					for (j = 0; j < NUMBER_EXPLOSION; j++) {
						if (!g->explosion[j].active) {
							g->explosion[j].active = true;
							g->explosion[j].x = g->bossShots[i].x + 2;
							g->explosion[j].y = g->bossShots[i].y;
							g->explosion[j].explosionTimer = EXPLOSION_TIMER;

							break;
						}
					}
					GFX_DrowBitMap_P(g->player.x + 8, g->player.y - 2,
							player_shield_map, 10, 16, 1);
					GFX_DrowBitMap_P(g->player.x, g->player.y, player_map, 11,
							11, 1);

					//SSD1327_Display();
					if (g->player.lives <= 0) {
						PlayDeadAnim();
						g->state = GS_Dead;
					}
				}
			}
		}

		// Player's shots to the boss
		for (i = 0; i < NUMBER_SHOTS; i++) {
			if (g->shots[i].active) {
				if (Colliding(g->boss.x, g->boss.y, g->shots[i].x,
						g->shots[i].y)
						|| Colliding(g->boss.x, g->boss.y + 6, g->shots[i].x,
								g->shots[i].y)
						|| Colliding(g->boss.x, g->boss.y + 12, g->shots[i].x,
								g->shots[i].y)) {
					g->boss.lives -= 1;
					g->shots[i].active = false;
					g->shots[i].trackNumber = 0;
//					GFX_DrowBitMap_P(g->shots[i].x, g->shots[i].y,
//							explosion_map, 10, 10, 1);
					for (j = 0; j < NUMBER_EXPLOSION; j++) {
						if (!g->explosion[j].active) {
							g->explosion[j].active = true;
							g->explosion[j].x = g->shots[i].x + 2;
							g->explosion[j].y = g->shots[i].y;
							g->explosion[j].explosionTimer = EXPLOSION_TIMER;

							break;
						}
					}

					if (g->boss.lives <= 0) {
						g->boss.active = false;
						g->boss.lives = 0;
						break;
					}
				}
			}
		}
	}

	//Painting over and deactivating shots left over from the boss
	if (!g->boss.active) {
		for (i = 0; i < NUMBER_BOSS_SHOTS; i++) {
			if (g->bossShots[i].active) {
				g->bossShots[i].active = false;
				GFX_DrowBitMap_P(g->bossShots[i].x, g->bossShots[i].y,
						player_shot_map, 4, 1, 0);
			}
		}
	}
	//-------------------------------------------

	if (in->btn1State == GPIO_PIN_SET)
		GameShot(g);//shot();

	// Checking the collision of a player's shots with opponents. Adding Bonuses
	for (i = 0; i < NUMBER_SHOTS; ++i) {
		for (j = 0; j < NUMBER_ENEMIES; ++j) {
			if (g->shots[i].active && g->enemies[j].active) {
				if (Colliding(g->enemies[j].x, g->enemies[j].y, g->shots[i].x,
						g->shots[i].y)) {
					g->enemies[j].active = false;
					g->enemies[j].trackedByMissile = false;
					g->enemies[j].trackNumber = 0;
					g->shots[i].active = false;
					g->player.score += 1;
//					GFX_DrowBitMap_P(g->enemies[j].x, g->enemies[j].y,
//							explosion_map, 10, 10, 1);
					for (k = 0; k < NUMBER_EXPLOSION; k++) {
						if (!g->explosion[k].active) {
							g->explosion[k].active = true;
							g->explosion[k].x = g->enemies[j].x + 2;
							g->explosion[k].y = g->enemies[j].y;
							g->explosion[k].explosionTimer = EXPLOSION_TIMER;

							break;
						}
					}

					//Dodanie bonusa w miejscu zestrzelenia
					if ((rand() % 100) < BONUS_FREQUENCY)
						GameAddBonus(g, g->enemies[j].x, g->enemies[j].y);//add_bonus(g->enemies[j].x, g->enemies[j].y);
				}
			}
		}
	}
	if ((rand() % 100) < (g->player.level * 2) && !(g->boss.active)) //Frequency of adding opponents according to level
		GameAddEnemy(g);//add_enemy();
}

void GameDraw(GameCtx *g, InputSnapshot* in) {
	/*
	 * Drawing all game graphics
	 */
	uint8_t i;

	//Drowing the game information
	GFX_PutInt(5, 0, g->player.score, 1, 1, 0);
	GFX_DrowBitMap_P(102, 0, lives_map, 8, 6, 1);
	GFX_PutInt(114, 0, g->player.lives, 1, 1, 0);
	GFX_DrowBitMap_P(50, 0, Level_map, 20, 7, 1);
	GFX_PutInt(80, 0, g->player.level, 1, 1, 0);

	//Drawing graphics of a player's shot
	for (i = 0; i < NUMBER_SHOTS; i++) {
		if (g->shots[i].active) {
			GFX_DrowBitMap_P(g->shots[i].x, g->shots[i].y, player_shot_map, 4, 1, 1);
		}
	}

	//Drawing graphics of a boss shot
	if (g->boss.active) {
		for (i = 0; i < NUMBER_BOSS_SHOTS; i++) {
			if (g->bossShots[i].active) {
				GFX_DrowBitMap_P(g->bossShots[i].x, g->bossShots[i].y,
						player_shot_map, 4, 1, 1);
			}
		}
	}

	//Drawing graphics of enemies
	for (i = 0; i < NUMBER_ENEMIES; i++) {
		if (g->enemies[i].active) {
			if (g->enemies[i].type == ET_Tracker)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bitMap,
						5, 5, 1);
			if (g->enemies[i].type == ET_Diver)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bitMap,
						3, 7, 1);
			if (g->enemies[i].type == ET_Bobber)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bitMap,
						5, 5, 1);
		}
	}

	//Drawing the player's graphics
	GFX_DrowBitMap_P(g->player.x, g->player.y, player_map, 11, 11, 1);

	//Drawing a background
	for (i = 0; i < NUMBER_BACKGROUND; i++) {
		if (g->background[i].active) {
			SSD1327_SetPixel(g->background[i].x, g->background[i].y, (rand() % 15));
		}
	}

	//Drawing Boss
	if (g->boss.active && g->player.gameProgres == 30)
		GFX_DrowBitMap_P(g->boss.x, g->boss.y, boss_map_1, 10, 18, 1);
	if (g->boss.active && g->player.gameProgres == 60)
		GFX_DrowBitMap_P(g->boss.x, g->boss.y, boss_map_2, 10, 18, 1);

	//Drawing bonuses
	for (i = 0; i < NUMBER_BONUS; i++) {
		if (g->bonuses[i].active) {
			switch (g->bonuses[i].type) {
			case BT_Live:
				GFX_DrowBitMap_P(g->bonuses[i].x, g->bonuses[i].y, bonus_live_map, 7,
						7, 1);
				break;
			case BT_TrackerShot:
				GFX_DrowBitMap_P(g->bonuses[i].x, g->bonuses[i].y,
						bonus_tracker_shoot_map, 7, 7, 1);
				break;
			}
		}
	}

	//Drawing explosion
	for (i = 0; i < NUMBER_EXPLOSION; i++){
		if (g->explosion[i].active){
			GFX_DrowBitMap_P(g->explosion[i].x, g->explosion[i].y, explosion_map, 10, 10, 1);
		}
	}
}

void GameLevelUpdate(GameCtx* g) {
	/*
		 * It is responsible for the appropriate appearance of bosses,
		 * their initial parameters and adjusts the level of play to its progress.
		 */
		static uint8_t i = 0;

		//Calculation of game progress
		if(i > 70)
		{
			g->player.gameProgres += 1;
			i = 0;
		}

		//If the boss is not active count the progress
		if(!g->boss.active) i++;

		//---- Moments of boss appearance -----
		if(g->player.gameProgres == 29)
		{
			g->boss.active = true;
			g->boss.lives = 3;
			g->boss.level = 3;
			g->boss.updateDelay = 4;
			g->player.gameProgres += 1;

		}
		if(g->player.gameProgres == 59)
		{
			g->boss.active = true;
			g->boss.lives = 6;
			g->boss.level = 6;
			g->boss.updateDelay = 2;
			g->player.gameProgres += 1;
		}
		//--------------------------------------

		//Next levels
		if(g->player.gameProgres > 9 && g->player.gameProgres < 10 )
			g->player.level = 1;
		if(g->player.gameProgres > 10 && g->player.gameProgres < 19)
			g->player.level = 2;
		if(g->player.gameProgres > 20 && g->player.gameProgres < 29)
			g->player.level = 3;
		if(g->player.gameProgres > 30 && g->player.gameProgres < 39)
			g->player.level = 4;
		if(g->player.gameProgres > 40 && g->player.gameProgres < 49)
			g->player.level = 5;
		if(g->player.gameProgres > 50 && g->player.gameProgres < 59)
			g->player.level = 6;
		if(g->player.gameProgres > 60 && g->player.gameProgres < 69)
			g->player.level = 7;
		if(g->player.gameProgres > 70 && g->player.gameProgres < 79)
			g->player.level = 8;
		if(g->player.gameProgres > 80 && g->player.gameProgres < 89)
			g->player.level = 9;
		if(g->player.gameProgres > 90)
			g->player.level = 10;

}

void GameUpdateBackgrand(GameCtx* g) {

	/*
	 * Refreshes the background effect, stars.
	 * Moves elements and randomises when a new element have to be added.
	 */
	uint8_t i;

	for(i = 0; i < NUMBER_BACKGROUND; i++)
	{
		if(g->background[i].active)
		{
			g->background[i].nextUpdate -= 1;
			if(g->background[i].nextUpdate <= 0)
			{
				g->background[i].nextUpdate = g->background[i].updateDelay;
				if(g->background[i].active)
				{
					g->background[i].x -= 1;

					if(g->background[i].x <= -2)
						g->background[i].active = false;
				}
			}
		}
	}

	if ((rand()%100) < NUMBER_BACKGROUND_FREQ) 		//Frequency of background additions
		GameAddBackground(g);//add_background();

}

void GameAddBackground(GameCtx* g){
	/*
	 * Adds a single unit (structure) to the entire array.
	 * In addition, it sets the random parameters for its
	 * refreshment and a random position on the Y axis.
	 */
	uint8_t i;

	for(i = 0; i < NUMBER_BACKGROUND; i++)
	{
		if(!g->background[i].active)
		{
			g->background[i].active 		= true;
			g->background[i].x 				= SCREEN_WIDTH;
			g->background[i].y				= (rand()%(SCREEN_HEIGHT-10)) +10;
			g->background[i].updateDelay 	= (rand()%6)+2; // def. (rand()%4)+2;

			break;
		}
	}
}

void GameAddBonus(GameCtx* g, int x, int y){

	/*
	 * Adding a bonus in the place after an opponent has been shot down.
	 * Checking whether this is possible.
	 */

	uint8_t i;
	int bonus_type;

	for(i = 0; i < NUMBER_BONUS; i++){
		if(!g->bonuses[i].active){
			g->bonuses[i].active = true;
			g->bonuses[i].x = x;
			g->bonuses[i].y = y;
			g->bonuses[i].updateDelay = 3;

			bonus_type = rand()%100;

			if(bonus_type > 30){
				g->bonuses[i].bitMap = bonus_live_map;
				g->bonuses[i].type = BT_Live;
			}
			if(bonus_type < 30){
				g->bonuses[i].bitMap = bonus_tracker_shoot_map;
				g->bonuses[i].type = BT_TrackerShot;
			}
			return;
		}
	}
}

void GameUpdateBonus(GameCtx* g){
	/*
		 * Checking whether a player has hovered over a bonus.
		 * Moving a bonus on the map
		 * */
		int i = 0;

		//Check duration of st_tracekr if active

		if(g->player.bonusDuration > 0)
			g->player.bonusDuration -= 1;
		if(g->player.bonusDuration == 0 && g->player.shootType == ST_Tracker)
			g->player.shootType = ST_Normal;

		// Checking whether a player has hovered over a bonus
		for (i = 0; i < NUMBER_BONUS; i++)
		{
			if(g->bonuses[i].active)
			{
				g->bonuses[i].nextUpdate -= 1;
				if(g->bonuses[i].nextUpdate <= 0)
				{
					if(g->bonuses[i].active)
					{
						g->bonuses[i].nextUpdate = g->bonuses[i].updateDelay;

						if (Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x, g->player.y) 	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x, g->player.y+5) 	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x+7, g->player.y)	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x+7, g->player.y+5)
							)
						{
							switch(g->bonuses[i].type)
							{
							case BT_Live:
								g->player.lives += 1;
								g->bonuses[i].active = false;
								break;
							case BT_TrackerShot:
								g->player.shootType = ST_Tracker;
								g->player.bonusDuration = BONUS_DURATION + (g->player.level * 50);
								g->bonuses[i].active = false;
								break;
							}

						}

						g->bonuses[i].x -= 1;
						if(g->bonuses[i].x <= -2){
							g->bonuses[i].active = false;
						}
					}
				}
			}
		}
}

void GameShot(GameCtx* g){
	/*
		 * Activate the shot in the player's shot table and set the initial parameters.
		 */
		uint8_t i;

		bool is_any_enemies_active = false;

		int closest_enemy_number;
		double temp_distance;
		double smolest_distance = 500;
		int random_tracking_number;

		for (i = 0; i < NUMBER_SHOTS; ++i)
		{
			if (!g->shots[i].active)
			{
				g->shots[i].active = true;
				g->shots[i].x = 11;
				g->shots[i].y = g->player.y + 5;

				//Setting the type of shot
				switch(g->player.shootType)
				{
				case ST_Normal:
					g->shots[i].type = ST_Normal;
					break;
				case ST_Tracker:

					/*
					 * Is there at least one active opponent who is untargeted.
					 * If so start tracking him.
					 * */
					for(int j = 0; j < NUMBER_ENEMIES; j++)
					{
						if(g->enemies[j].active && !g->enemies[j].trackedByMissile)
						{
							is_any_enemies_active = true;

							temp_distance = sqrt(pow(g->enemies[j].x - g->player.x, 2) + pow(g->enemies[j].y - g->player.y, 2));

							if (temp_distance < smolest_distance)
							{
								smolest_distance = temp_distance;
								closest_enemy_number = j;
							}
						}
					}
					if(is_any_enemies_active)
					{
						random_tracking_number = rand();
						g->enemies[closest_enemy_number].trackNumber = random_tracking_number;
						g->enemies[closest_enemy_number].trackedByMissile = true;
						g->shots[i].type = ST_Tracker;
						g->shots[i].trackNumber = random_tracking_number;
					} else {
						// If you haven't found a target act like a normal shot
						g->shots[i].type = ST_Normal;
					}
					break;
				}
				return;
			}
		}
}

void GameShotBoss(GameCtx* g){
	/*
		 * Handling boss shots.
		 */
		uint8_t i;

		for (i = 0; i < NUMBER_BOSS_SHOTS; ++i)
		{
			if (!g->bossShots[i].active)
			{
				g->bossShots[i].active = true;
				g->bossShots[i].x = g->boss.x;
				g->bossShots[i].y = g->boss.y + 5;
				return;
			}
		}
}

bool Colliding(int x0, int y0, int x1, int y1){
	/*
		 * Checking whether objects collide with each other.
		 */
		int dx = abs(x0 - x1);
		int dy = abs(y0 - y1);
		return dx < 6 && dy < 9;
}

void GameAddEnemy(GameCtx* g){
	/*
		 * Adding an opponent appropriate to the current progress of the game.
		 * Setting its initial parameters, i.e. position, refreshment, graphics, type.
		 */
		uint8_t i;
		uint8_t enemy_type;

		for(i = 0; i < g->player.level; i++)
		{
			if(!g->enemies[i].active)
			{
				g->enemies[i].active = true;
				g->enemies[i].x = SCREEN_WIDTH + 12;
				g->enemies[i].y = ((rand()%(SCREEN_HEIGHT - 10))+10);
				g->enemies[i].trackedByMissile = false;
				g->enemies[i].trackNumber = 0;

				enemy_type = (rand()%100);

				if((enemy_type > 50))
				{

					g->enemies[i].type = ET_Diver;
					g->enemies[i].updateDelay = (rand()%3);	//Speed setting (less = faster)
					g->enemies[i].bitMap = driver_map;
				}
				if((enemy_type > 20 && enemy_type < 50) && (g->player.level > 4))
				{
					g->enemies[i].type = ET_Tracker;
					g->enemies[i].updateDelay = ((rand()%3)+1);
					g->enemies[i].bitMap = tracker_map;
				}
				if((enemy_type < 20) && (g->player.level > 7))
				{
					g->enemies[i].type = ET_Bobber;
					g->enemies[i].updateDelay = ((rand()%3)+2);
					g->enemies[i].bitMap = bobber_map;
				}
				break;
			}
		}
}

void GameUpdateExplosion(GameCtx* g){
	/*
	 * Updates explosions, deactivates when timer expires
	 * */
	uint8_t i;

	for(i = 0; i < NUMBER_EXPLOSION; i++){
		if(g->explosion[i].active){
			g->explosion[i].explosionTimer -= 1;
			if(g->explosion[i].explosionTimer <= 0){
				g->explosion[i].active = false;
			}
		}
	}
}

void GameSetState(GameCtx* g, GameState state){
	/*
	 * Sets the game state in the passed context
	 * */

	g->state = state;
}

GameState GameGetState(GameCtx* g){
	/*
	 * Returns the current play status of the passed context.
	 * */

	return g->state;
}

int GameGetPalyerScore(GameCtx* g){
	/*
	 * Returns the player's current score from the passed context.
	 * */

	return g->player.score;
}



