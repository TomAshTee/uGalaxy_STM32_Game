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
	g->player.lives = initial_lives;
	g->player.score = initial_score;
	g->player.x = initial_x;
	g->player.y = initial_y;
	g->player.level = initial_level;
	g->player.game_progres = initial_game_progres;
	g->player.shoot_type = st_normal;

	//Deactivation of player shots
	for (i = 0; i < num_shots; ++i)
		g->shots[i].active = false;

	//Deactivation of boss shots
	for (i = 0; i < num_boss_shots; ++i)
		g->bossShots[i].active = false;

	//Deactivation enemies
	for (i = 0; i < num_enemies; i++) {
		g->enemies[i].active = false;
		g->enemies[i].track_number = 0;
		g->enemies[i].tracked_by_missile = false;
	}

	//Deactivation bonuses
	for (i = 0; i < num_bonus; i++)
		g->bonuses[i].active = false;

	//Deactivation boss
	g->boss.active = false;
	g->boss.x = initial_boss_x;
	g->boss.y = initial_boss_y;
	g->boss.update_delay = initial_boss_update_delay;
}

void GameTick(GameCtx *g, InputSnapshot* in) {
	/*
	 * The logic of the whole game
	 */
	uint8_t i, j;

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
	if (g->player.y > (screen_height - 14))
		g->player.y = (screen_height - 14);

	// Shifting shots forward
	bool shoot_updated = false;

	for (i = 0; i < num_shots; ++i) {
		switch (g->shots[i].type) {
		case st_normal:
			if (g->shots[i].active)
				g->shots[i].x++;
			if (g->shots[i].x > screen_width)
				g->shots[i].active = false;
			break;
		case st_tracker:

			for (int j = 0; j < num_enemies; j++) {
				if (g->shots[i].track_number == g->enemies[j].track_number) {
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
			if (!shoot_updated && g->shots[i].type == st_tracker) {
				g->shots[i].active = false;
				g->shots[i].track_number = 0;
				shoot_updated = false;
			}

			//Remove off-map shots
			if (g->shots[i].x > screen_width) {
				g->shots[i].active = false;
				g->shots[i].track_number = 0;
			}
			break;
		}

	}

	//Remove the markers on enemies whose shots have been used on others
	bool is_there_a_missile;
	for (i = 0; i < num_enemies; i++) {
		is_there_a_missile = false;

		for (j = 0; j < num_shots; j++) {
			if (g->enemies[i].track_number == g->shots[j].track_number)
				is_there_a_missile = true;
		}

		if (!is_there_a_missile)
			g->enemies[i].track_number = 0;
	}

	// Updated enemies
	for (i = 0; i < num_enemies; ++i) {

		if (g->enemies[i].active) {
			g->enemies[i].next_update -= 1;
			if (g->enemies[i].next_update <= 0) {
				if (g->enemies[i].active) {

					g->enemies[i].next_update = g->enemies[i].updateDelay;

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
						g->enemies[i].tracked_by_missile = false;
						g->enemies[i].track_number = 0;
						GFX_DrowBitMap_P(g->enemies[i].x + 2, g->enemies[i].y,
								explosion_map, 10, 10, 1);
						GFX_DrowBitMap_P(g->player.x + 8, g->player.y - 2,
								player_shield_map, 10, 16, 1);
						GFX_DrowBitMap_P(g->player.x, g->player.y, player_map,
								11, 11, 1);

						ssd1327_display();
						if (g->player.lives <= 0) {
							PlayDeadAnim();
							g->state = st_dead;
						}
					}

					// Moving to the left and making special moves
					g->enemies[i].x -= 1;

					switch (g->enemies[i].type) {
					case et_tracker:
						if (g->enemies[i].x < 70) {
							if (g->player.y > g->enemies[i].y)
								g->enemies[i].y += 1;
							if (g->player.y < g->enemies[i].y)
								g->enemies[i].y -= 1;
						}
						break;
					case et_diver:
						break;
					case et_bobber:
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
						g->enemies[i].tracked_by_missile = false;
						g->enemies[i].track_number = 0;
					}
				}
			}
		}
	}

	//------------- Boss service ---------------
	if (g->boss.active) {
		g->boss.next_update -= 1;

		//Boss position
		if (g->boss.next_update <= 0) {
			g->boss.next_update = g->boss.update_delay;

			y += dy;
			if (y < 1 || y > (screen_height - 24))
				dy = -dy;

			g->boss.y = y;

			if (g->boss.y < 10)
				g->boss.y = 10;
			if (g->boss.y > (screen_height - 24))
				g->boss.y = (screen_height - 24);

			g->boss.x -= 1;
			if (g->boss.x < 100)
				g->boss.x = 100;
		}

		//Frequency of boss shots
		if ((rand() % 100) < (g->boss.level * 5))
			GameShotBoss(g);


		for (i = 0; i < num_boss_shots; ++i) {
			if (g->bossShots[i].active)
				g->bossShots[i].x--;
			if (g->bossShots[i].x < -4)
				g->bossShots[i].active = false;
		}

		for (i = 0; i < num_boss_shots; i++) {
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
					GFX_DrowBitMap_P(g->bossShots[i].x + 2, g->bossShots[i].y,
							explosion_map, 10, 10, 1);
					GFX_DrowBitMap_P(g->player.x + 8, g->player.y - 2,
							player_shield_map, 10, 16, 1);
					GFX_DrowBitMap_P(g->player.x, g->player.y, player_map, 11,
							11, 1);

					ssd1327_display();
					if (g->player.lives <= 0) {
						PlayDeadAnim();
						g->state = st_dead;
					}
				}
			}
		}

		// Player's shots to the boss
		for (i = 0; i < num_shots; i++) {
			if (g->shots[i].active) {
				if (Colliding(g->boss.x, g->boss.y, g->shots[i].x,
						g->shots[i].y)
						|| Colliding(g->boss.x, g->boss.y + 6, g->shots[i].x,
								g->shots[i].y)
						|| Colliding(g->boss.x, g->boss.y + 12, g->shots[i].x,
								g->shots[i].y)) {
					g->boss.lives -= 1;
					g->shots[i].active = false;
					g->shots[i].track_number = 0;
					GFX_DrowBitMap_P(g->shots[i].x, g->shots[i].y,
							explosion_map, 10, 10, 1);

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
		for (i = 0; i < num_boss_shots; i++) {
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
	for (i = 0; i < num_shots; ++i) {
		for (j = 0; j < num_enemies; ++j) {
			if (g->shots[i].active && g->enemies[j].active) {
				if (Colliding(g->enemies[j].x, g->enemies[j].y, g->shots[i].x,
						g->shots[i].y)) {
					g->enemies[j].active = false;
					g->enemies[j].tracked_by_missile = false;
					g->enemies[j].track_number = 0;
					g->shots[i].active = false;
					g->player.score += 1;
					GFX_DrowBitMap_P(g->enemies[j].x, g->enemies[j].y,
							explosion_map, 10, 10, 1);

					//Dodanie bonusa w miejscu zestrzelenia
					if ((rand() % 100) < frequ_bonus)
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
	for (i = 0; i < num_shots; i++) {
		if (g->shots[i].active) {
			GFX_DrowBitMap_P(g->shots[i].x, g->shots[i].y, player_shot_map, 4, 1, 1);
		}
	}

	//Drawing graphics of a boss shot
	if (g->boss.active) {
		for (i = 0; i < num_boss_shots; i++) {
			if (g->bossShots[i].active) {
				GFX_DrowBitMap_P(g->bossShots[i].x, g->bossShots[i].y,
						player_shot_map, 4, 1, 1);
			}
		}
	}

	//Drawing graphics of enemies
	for (i = 0; i < num_enemies; i++) {
		if (g->enemies[i].active) {
			if (g->enemies[i].type == et_tracker)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bit_map,
						5, 5, 1);
			if (g->enemies[i].type == et_diver)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bit_map,
						3, 7, 1);
			if (g->enemies[i].type == et_bobber)
				GFX_DrowBitMap_P(g->enemies[i].x, g->enemies[i].y, g->enemies[i].bit_map,
						5, 5, 1);
		}
	}

	//Drawing the player's graphics
	GFX_DrowBitMap_P(g->player.x, g->player.y, player_map, 11, 11, 1);

	//Drawing a background
	for (i = 0; i < num_background; i++) {
		if (g->background[i].active) {
			ssd1327_setPixel(g->background[i].x, g->background[i].y, (rand() % 15));
		}
	}

	//Drawing Boss
	if (g->boss.active && g->player.game_progres == 30)
		GFX_DrowBitMap_P(g->boss.x, g->boss.y, boss_map_1, 10, 18, 1);
	if (g->boss.active && g->player.game_progres == 60)
		GFX_DrowBitMap_P(g->boss.x, g->boss.y, boss_map_2, 10, 18, 1);

	//Drawing bonuses
	for (i = 0; i < num_bonus; i++) {
		if (g->bonuses[i].active) {
			switch (g->bonuses[i].type) {
			case bt_live:
				GFX_DrowBitMap_P(g->bonuses[i].x, g->bonuses[i].y, bonus_live_map, 7,
						7, 1);
				break;
			case bt_tracker_shoot:
				GFX_DrowBitMap_P(g->bonuses[i].x, g->bonuses[i].y,
						bonus_tracker_shoot_map, 7, 7, 1);
				break;
			}
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
			g->player.game_progres += 1;
			i = 0;
		}

		//If the boss is not active count the progress
		if(!g->boss.active) i++;

		//---- Moments of boss appearance -----
		if(g->player.game_progres == 29)
		{
			g->boss.active = true;
			g->boss.lives = 3;
			g->boss.level = 3;
			g->boss.update_delay = 4;
			g->player.game_progres += 1;

		}
		if(g->player.game_progres == 59)
		{
			g->boss.active = true;
			g->boss.lives = 6;
			g->boss.level = 6;
			g->boss.update_delay = 2;
			g->player.game_progres += 1;
		}
		//--------------------------------------

		//Next levels
		if(g->player.game_progres > 9 && g->player.game_progres < 10 )
			g->player.level = 1;
		if(g->player.game_progres > 10 && g->player.game_progres < 19)
			g->player.level = 2;
		if(g->player.game_progres > 20 && g->player.game_progres < 29)
			g->player.level = 3;
		if(g->player.game_progres > 30 && g->player.game_progres < 39)
			g->player.level = 4;
		if(g->player.game_progres > 40 && g->player.game_progres < 49)
			g->player.level = 5;
		if(g->player.game_progres > 50 && g->player.game_progres < 59)
			g->player.level = 6;
		if(g->player.game_progres > 60 && g->player.game_progres < 69)
			g->player.level = 7;
		if(g->player.game_progres > 70 && g->player.game_progres < 79)
			g->player.level = 8;
		if(g->player.game_progres > 80 && g->player.game_progres < 89)
			g->player.level = 9;
		if(g->player.game_progres > 90)
			g->player.level = 10;

}

void GameUpdateBackgrand(GameCtx* g) {

	/*
	 * Refreshes the background effect, stars.
	 * Moves elements and randomises when a new element have to be added.
	 */
	uint8_t i;

	for(i = 0; i < num_background; i++)
	{
		if(g->background[i].active)
		{
			g->background[i].next_update -= 1;
			if(g->background[i].next_update <= 0)
			{
				g->background[i].next_update = g->background[i].update_delay;
				if(g->background[i].active)
				{
					g->background[i].x -= 1;

					if(g->background[i].x <= -2)
						g->background[i].active = false;
				}
			}
		}
	}

	if ((rand()%100) < num_background_freq) 		//Frequency of background additions
		GameAddBackground(g);//add_background();

}

void GameAddBackground(GameCtx* g){
	/*
	 * Adds a single unit (structure) to the entire array.
	 * In addition, it sets the random parameters for its
	 * refreshment and a random position on the Y axis.
	 */
	uint8_t i;

	for(i = 0; i < num_background; i++)
	{
		if(!g->background[i].active)
		{
			g->background[i].active 		= true;
			g->background[i].x 				= screen_width;
			g->background[i].y				= (rand()%(screen_height-10)) +10;
			g->background[i].update_delay 	= (rand()%6)+2; // def. (rand()%4)+2;

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

	for (i = 0; i < num_bonus; i++)
	{
		if (!g->bonuses[i].active)
		{
			g->bonuses[i].active = true;
			g->bonuses[i].x = x;
			g->bonuses[i].y = y;
			g->bonuses[i].update_delay = 3;

			bonus_type = rand()%100;

			if(bonus_type > 30)
			{
				g->bonuses[i].bit_map = bonus_live_map;
				g->bonuses[i].type = st_normal;
			}
			if(bonus_type < 30)
			{
				g->bonuses[i].bit_map = bonus_tracker_shoot_map;
				g->bonuses[i].type = st_tracker;
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

		if(g->player.bonus_duration > 0)
			g->player.bonus_duration -= 1;
		if(g->player.bonus_duration == 0 && g->player.shoot_type == st_tracker)
			g->player.shoot_type = st_normal;

		// Checking whether a player has hovered over a bonus
		for (i = 0; i < num_bonus; i++)
		{
			if(g->bonuses[i].active)
			{
				g->bonuses[i].next_update -= 1;
				if(g->bonuses[i].next_update <= 0)
				{
					if(g->bonuses[i].active)
					{
						g->bonuses[i].next_update = g->bonuses[i].update_delay;

						if (Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x, g->player.y) 	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x, g->player.y+5) 	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x+7, g->player.y)	||
							Colliding(g->bonuses[i].x,g->bonuses[i].y, g->player.x+7, g->player.y+5)
							)
						{
							switch(g->bonuses[i].type)
							{
							case bt_live:
								g->player.lives += 1;
								g->bonuses[i].active = false;
								break;
							case bt_tracker_shoot:
								g->player.shoot_type = st_tracker;
								g->player.bonus_duration = duration_bonus + (g->player.level * 50);
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

		for (i = 0; i < num_shots; ++i)
		{
			if (!g->shots[i].active)
			{
				g->shots[i].active = true;
				g->shots[i].x = 11;
				g->shots[i].y = g->player.y + 5;

				//Setting the type of shot
				switch(g->player.shoot_type)
				{
				case st_normal:
					g->shots[i].type = st_normal;
					break;
				case st_tracker:

					/*
					 * Is there at least one active opponent who is untargeted.
					 * If so start tracking him.
					 * */
					for(int j = 0; j < num_enemies; j++)
					{
						if(g->enemies[j].active && !g->enemies[j].tracked_by_missile)
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
						g->enemies[closest_enemy_number].track_number = random_tracking_number;
						g->enemies[closest_enemy_number].tracked_by_missile = true;
						g->shots[i].type = st_tracker;
						g->shots[i].track_number = random_tracking_number;
					} else {
						// If you haven't found a target act like a normal shot
						g->shots[i].type = st_normal;
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

		for (i = 0; i < num_boss_shots; ++i)
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
				g->enemies[i].x = screen_width + 12;
				g->enemies[i].y = ((rand()%(screen_height - 10))+10);
				g->enemies[i].tracked_by_missile = false;
				g->enemies[i].track_number = 0;

				enemy_type = (rand()%100);

				if((enemy_type > 50))
				{

					g->enemies[i].type = et_diver;
					g->enemies[i].updateDelay = (rand()%3);	//Speed setting (less = faster)
					g->enemies[i].bit_map = driver_map;
				}
				if((enemy_type > 20 && enemy_type < 50) && (g->player.level > 4))
				{
					g->enemies[i].type = et_tracker;
					g->enemies[i].updateDelay = ((rand()%3)+1);
					g->enemies[i].bit_map = tracker_map;
				}
				if((enemy_type < 20) && (g->player.level > 7))
				{
					g->enemies[i].type = et_bobber;
					g->enemies[i].updateDelay = ((rand()%3)+2);
					g->enemies[i].bit_map = bobber_map;
				}
				break;
			}
		}
}

void GameSetState(GameCtx* g, gamestate state){
	/*
	 * Sets the game state in the passed context
	 * */

	g->state = state;
}

gamestate GameGetState(GameCtx* g){
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


