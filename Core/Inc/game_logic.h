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

/*
 * ----------------------------------------------------------------------
 * 								Stałe
 */

#define num_shots  			35		//Maksymalna ilość strzałów def.8
#define num_boss_shots 		5		//Ilość strzaów bosa !!!! NIE WIADOMO CZEMU GRA SIE KRZACZY PRZY INNEJ ILOSCI !!! def.5
#define num_enemies  		10		//Maksymalna ilość przeciwników
#define num_background		250		//Max. obiektów w tle( gwiazd) (du�a konsumpacja pamieci RAM) def.40
#define num_background_freq 	12		//Częstotliwość dodwawnia tła (0-100) def.20
#define num_bonus			3		//Max. ilość bonusów na mapie

#define frequ_bonus		15			// Częstotliowść pojawiania się bonusów (1-99)
#define duration_bonus	150			// Czas trwania bonusu

#define initial_lives 	10			//Początkowa ilość żyć
#define initial_score 	0			//Początkowy wynik gry
#define initial_x		2			//Początkowa pozycja X gracza
#define initial_y		64			//Początkowa pozycja Y gracza
#define initial_level	1			//Początkowy poziom gry
#define initial_game_progres 0		//Początkowy postę gry

#define BLACK 			0
#define WHITE 			15
#define screen_width 	SSD1327_WIDTH
#define screen_height 	SSD1327_HEIGHT


/*
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * 								Wyliczenia
 */

typedef enum			//Rodzaje przeciwników
{
	et_diver,
	et_tracker,
	et_bobber,
}enemy_type;

typedef enum			//Stany gry
{
	st_menu,
	st_playing,
	st_dead,
}gamestate;

typedef enum 			//Rodzaj bonusa
{
	bt_live,
	bt_tracker_shoot,
}bonus_type;

typedef enum
{
	st_normal,
	st_tracker,
}shot_type;

/*
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * 								Struktóry
 */

typedef struct				//Gracz i jego parametry
{
	int x, y;
	int score, high_score;
	int lives;
	int level;
	int game_progres;
	shot_type shoot_type;
	int bonus_duration;
} T_player;

typedef struct				//Strzały gracza, strzały bosów
{
	bool active;
	int x, y;
	shot_type type;
	int truck_number;
}T_shot;

typedef struct				//Tło - "gwiazdy"
{
	bool active;
	int x;
	int y;
	int next_update;
	int update_delay;
}T_backgrand;

typedef struct				//Przeciwnicy
{
	bool active;
	int x;
	int y;
	enemy_type type;
	uint8_t* bit_map;
	int next_update;
	int update_delay;
	bool tracked_by_missile;
	int truck_number;
}T_enemy;

typedef struct 				// Bosowie
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
	uint8_t* bit_map;
	int next_update;
	int update_delay;
}T_bonus;

/*
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

/*
 * ----------------------------------------------------------------------
 * 							Deklaracje Funkcji
 */

void start_game(void); 							//Rozpoczęcie gry
void add_enemy(void);							//Dodawanie przeciwników
void run_menu (void);							//Obsługa menu
void run_game (void);							//Główny szkielet sekwencji gry
void drow_game(void);							//"Rysowanie" gry
void update_scene(void);						//Obsługa wydarzeń w grze "główny rdzeń gry"
void shot(void);								//Dodanie strzału gracza
void boss_shoot(void);							//Dodanie strzału bosa
void play_dead_anim(void);						//Wyswietlenie animacji po śmierci - budowanie klimatu
void run_dead(void);							//Obsługa końca gry
void update_lvl(void);							//Aktualizacja poziomu gracza
void update_backgrand(void);					//Obsługa tła
void add_backgrand(void);						//Dodanie jednostki tła
bool colliding(int x0, int y0, int x1, int y1);	//Sprawdzanie kolizji obiektów
void update_enemy (T_enemy* enemy);				//Dodanie przeciwnika
void add_bonus(int x, int y);					//Dodanie bonusa na pozycji zestrzelonego wroga
void update_bonus(void);						//Obsługa bonusów
uint8_t button_pressed (void); 					//Sprawdzenie czy przycisk nr.1  jest wciśnięty
int joystick_value_y (void);					//Podanie wartości osi Y Joysticka

/*
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

#endif /* INC_GAME_LOGIC_H_ */
