/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "graphics_map.h"
#include "ssd1327.h"
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//--------------------------------------

//------------- WYLICZENIA --------------

typedef enum
{
	et_diver,
	et_tracker,
	et_bobber,
}enemy_type;

typedef enum
{
	st_menu,
	st_playing,
	st_dead,
}gamestate;

//---------------------------------------
//------------- STRUKT�RY --------------

typedef struct		//strukt�ra gracza
{
	int x, y;
	int score, high_score;
	int lives;
	int level;
	int game_progres; //stan gry , 1 oznacza pe�ny przebieg ekranu 128 punkt�w
} T_player;

typedef struct		//struktura pocisk�w , strza��w
{
	bool active;
	int x, y;
}T_shot;

typedef struct		//s. odpowiadajaca za t�o w grze
{
	bool active;
	int x;
	int y;
	int next_update;
	int update_delay;
}T_backgrand;

typedef struct
{
	bool active;
	int x;
	int y;
	enemy_type type;
	uint8_t* bit_map;
	int next_update;
	int update_delay;
}T_enemy;

typedef struct
{
	bool active;
	int x,y;
	int next_update;
	int update_delay;
	int lives;
	int level;
}T_boss;
//--------------------------------------

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//------------- STA�E, ZMIENNE --------------

#define num_shots  		8			//maksymalna ilo�c strza��w
#define num_boss_shots 	5			// ilosc strza��w bosa !!!! NIE WIADOMO CZEMU GRA SIE KRZACZY PRZY INNEJ ILOSCI !!!
#define num_enemies  	10			//maksymalna ilosc przeciwnik�w
#define num_backgrand	40			//max. obiekt�w w tle( gwiazd) (du�a konsumpacja pamieci RAM)

#define initial_lives 	20			//początkowa ilość żyć
#define initial_score 	0			//początkowy wynik gry
#define initial_x		2			//początkowa pozycja X gracza
#define initial_y		64			//początkowa pozycja Y gracza
#define initial_level	1			//początkowy poziom gry
#define initial_game_progres 0		//początkowy postę gry

#define BLACK 			0
#define WHITE 			15
#define screen_width 	SSD1327_WIDTH
#define screen_height 	SSD1327_HEIGHT

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

//------- TABLICE I TABLICE STRUKT�R ---------
T_player player; 						//tworzenie gracza
T_shot shots[num_shots]; 				//tworzenie stra��w
T_shot boss_shots [num_boss_shots];		//tworzenie strza��w od bosa
T_enemy enemies[num_enemies];			//tworzenie przeciwnik�w
T_backgrand backgrand[num_backgrand];	//tworzenie t�a
T_boss boss;							//boss w grze

gamestate state = st_menu;		//poczatkowy stan gry

uint8_t score_1, score_0;		//przechwycenie najelepszych wynik�w
uint8_t btn_prev = 0;			//obs�uga klawiszy zapobiega , repetycji

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

void start_game(void); 							//rozpocz�cie gry
void add_enemy(void);							//dodawniae przeciwnik�w
void run_menu (void);							//obs�ga menu
void run_game (void);							//g��wny szkielet sekfencji gry
void drow_game(void);							//"rysowanie" gry
void update_scene(void);						//Obs�uga wydarze� w grze "g��wny rdze� gry"
void shoot(void);								//Dodanie strza�u gracza
void boss_shoot(void);							//dodanie strza�u bossa
void play_dead_anim(void);						//Wyswietlenie animacji smierci  , taki tam przerywnik
void run_dead(void);							//Obs�uga konca gry
void update_lvl(void);							//aktualizacja levelu gracza
void update_backgrand(void);					//obs�uga t�a
void add_backgrand(void);						//Dodanie jednostki t�a, jedenj
bool colliding(int x0, int y0, int x1, int y1);	//sprawdzanie kolizji obiekt�w
void update_enemy (T_enemy* enemy);				//dodanie jednego przeciwnika
uint8_t button_pressed (void); 					//czy klawisz wcisniety

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

//  // Konfiguracja ADC z DMA
//  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
//  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)value, 2);

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADC_Start(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  ssd1327_SpiInit(&hspi1);
  ssd1327_CLR();
  ssd1327_display();

  start_game();
  add_enemy();

  while (1)
  {
		switch (state)
		{
		case st_menu:
			run_menu(); break;
		case st_playing:
			run_game(); break;
		case st_dead:
			run_dead();	break;
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|CS_Pin|DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BTN_1_Pin */
  GPIO_InitStruct.Pin = USER_BTN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(USER_BTN_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin CS_Pin DC_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|CS_Pin|DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void add_backgrand(void)
{
	/*
	 * Dodaje pojedyncz� jednostke (strukture) do calej tablicy.
	 * Dodatkowo ustawia losowe parametry odswiezania jej oraz po�o�enie,
	 * jedynie po�o�enie osi y.
	 *
	 */
	uint8_t i;

	for(i = 0; i < num_backgrand; i++)
	{
		if(!backgrand[i].active)
		{
			backgrand[i].active = true;
			backgrand[i].x 		= 128;
			backgrand[i].y		= (rand()%(screen_height-10)) +10;
			backgrand[i].update_delay = (rand()%4)+2;

			break;
		}
	}
}

void update_backgrand(void)
{
	/*
	 * Odswierza efekt t�a, gwiazd.
	 * Przesuwa elementy , oraz losuje kiedy ma
	 * zosta� dodany nowy element
	 */
	uint8_t i;

	for(i = 0; i < num_backgrand; i++)
	{
		if(backgrand[i].active)
		{
			backgrand[i].next_update -= 1;
			if(backgrand[i].next_update <= 0)
			{
				backgrand[i].next_update = backgrand[i].update_delay;
				if(backgrand[i].active)
				{
					backgrand[i].x -= 1;

					if(backgrand[i].x <= -2)
						backgrand[i].active = false;
				}
			}
		}
	}

	if ((rand()%100) < 20) //czesto�c dodawnaia przeciwnik�w
		add_backgrand();

}

void update_lvl(void)
{
	/*
	 * Odpowiada za odpowienie pojawianie sie bos�w,
	 * ich poczatkowych parametr�w oraz dostosowuje poziom gry
	 * do jej postepu.
	 */
	static uint8_t i = 0;

	//Obliczanie postepu gry
	if(i > 70) //poprawne dzia�anie 128 (ca�y ekran)
	{
		player.game_progres += 1;
		i = 0;
	}

	//Jesli boss nie aktywny naliczaj postep
	if(!boss.active) i++;

	//---- Momenty pojawienia sie bosa -----
	//Oraz jego parametry
	//boss.update_delay = x; powienien on posiadac pa�yst� liczb� poniewa�
	//w innym wypadku z niewiadomych przyczyn wysupje sie jego animacja.
	if(player.game_progres == 29)
	{
		boss.active = true;
		boss.lives = 15;
		boss.update_delay = 4;
		player.game_progres += 1; // zeby nie wejsc w tego ifa drugi raz

	}
	if(player.game_progres == 59)
	{
		boss.active = true;
		boss.lives = 20;
		boss.update_delay = 2;
		player.game_progres += 1;
	}
	//--------------------------------------

	//Kolejne levele
	if(player.game_progres > 9 && player.game_progres < 10 )
		player.level = 1;
	if(player.game_progres > 10 && player.game_progres < 19)
		player.level = 2;
	if(player.game_progres > 20 && player.game_progres < 29)
		player.level = 3;
	if(player.game_progres > 30 && player.game_progres < 39)
		player.level = 4;
	if(player.game_progres > 40 && player.game_progres < 49)
		player.level = 5;
	if(player.game_progres > 50 && player.game_progres < 59)
		player.level = 6;
	if(player.game_progres > 60 && player.game_progres < 69)
		player.level = 7;
	if(player.game_progres > 70 && player.game_progres < 79)
		player.level = 8;
	if(player.game_progres > 80 && player.game_progres < 89)
		player.level = 9;
	if(player.game_progres > 90)
		player.level = 10;

}

void run_dead(void)
{

	/*
	 * Ekran po smierci gracza. Wyswietlanie wybiku oraz
	 * animowanego napisu.
	 */

	static int x = 0, dx = 1;

	/*
	if(player.score > player.high_score)
	{
		player.high_score = player.score;
		score_0 = (uint8_t)(player.score & 0x00FF);
		score_1 = (uint8_t)((player.score & 0xFF00) >> 8 );

		eeprom_write_byte(eeprom_score_addr_0,score_0);
		eeprom_write_byte(eeprom_score_addr_1,score_1);
		eeprom_write_byte(eeprom_magic_addr_0, eeprom_magic_number_0);
		eeprom_write_byte(eeprom_magic_addr_1, eeprom_magic_number_1);
	}
	*/
	//pokazanie wyniku i najwyzszego wyniku

	x += dx;
	if (x < 1 || x > 55) dx = -dx;
	ssd1327_CLR();
	GFX_DrowBitMap_P(x,4,Defeated_map,67,16,1);
	GFX_DrowBitMap_P(35,24,Score_map,37,10,1);
	GFX_PutInt(73,27,player.score,1,1,0);
	//GFX_DrowBitMap_P(36,40,Best_map,27,10,1);
	//GFX_PutInt(73,43,player.high_score,1,1,0);
	ssd1327_display();

	if(button_pressed())
	{
		play_dead_anim();
		state = st_menu;
	}
}

void play_dead_anim(void)
{
	/*
	 * Animacja miedzy poczczegolnymi eranami. Daje z�udzenie
	 * takiej starej gry.
	 * Po sobie sobie jest ekran zapalany i gaszony dajac ciekawy efekt.
	 */
	uint8_t i;

	for (i = 0; i < 10; ++i)
	{
		ssd1327_CLR();
		GFX_FillRect(0,0,128,64,1);
		ssd1327_display();

		ssd1327_CLR();
		ssd1327_display();

	}
}

void shoot(void)
{
	/*
	 * Dodanie struktury strza�u do tablicy strza��w
	 * oraz ustalenie wstepnych jego parametr�w.
	 *
	 * Obs�uga strza��w gracza.
	 */
	uint8_t i;

	for (i = 0; i < num_shots; ++i)
	{
		if (!shots[i].active)
		{
			shots[i].active = true;
			shots[i].x = 11;
			shots[i].y = player.y + 5;
			return;
		}
	}
}

void boss_shoot(void)
{
	/*
	 * To samo co shoot() tylko �e odpowiada za strza�y
	 * boss�w.
	 */
	uint8_t i;

	for (i = 0; i < num_shots; ++i)
	{
		if (!boss_shots[i].active)
		{
			boss_shots[i].active = true;
			boss_shots[i].x = boss.x;
			boss_shots[i].y = boss.y + 5;
			return;
		}
	}
}

bool colliding(int x0, int y0, int x1, int y1)
{
	/*
	 * Sprawdzanie czy dane obiekty ze sob� koliduj�.
	 * Tzw. hitbox.
	 */
	int dx = abs(x0 - x1);
	int dy = abs(y0 - y1);
	return dx < 5 && dy < 8; // orginlanie  dx < 4 && dy < 6
}

void update_scene(void)
{
	/*
	 * Tu dziej� si� g��wne �eczy zwi�zane z gr� , jest tego
	 * za du�o zeby w tym miejscu wszystko opisa� :P
	 */
	uint8_t i,j;

	static uint8_t y = 0, dy = 1;
	  // Read analog stick (but we're just using it as binary input). Thresholds
	// experimentally determined.
	int stick = HAL_ADC_GetValue(&hadc1);

	if (stick < 1000)
		player.y -= 1;
	else if (stick > 3500)
		player.y += 1;

	// Keep the player in bounds.
	if (player.y < 10) player.y = 10;
	if (player.y > (screen_height - 14)) player.y = (screen_height - 14);

	// Move shots forward.
	for (i = 0; i < num_shots; ++i)
	{
		if (shots[i].active)
			++shots[i].x;
		if (shots[i].x > 128)
			shots[i].active = false;
	}

	// Update enemies.
	for (i = 0; i < num_enemies; ++i)
	{

		if (enemies[i].active)
		{
			enemies[i].next_update -= 1;
				if (enemies[i].next_update <= 0)
				{
					if (enemies[i].active)
					{

						enemies[i].next_update = enemies[i].update_delay;

						// Check for hits on player.
						if (colliding(enemies[i].x,enemies[i].y, player.x, player.y) 	||
							colliding(enemies[i].x,enemies[i].y, player.x, player.y+5) 	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y)	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y+5)
							)
						{
							player.lives -= 1;;
							enemies[i].active = false;
							GFX_DrowBitMap_P(enemies[i].x+2, enemies[i].y, explosion_map,10,10,1);
							GFX_DrowBitMap_P(player.x + 8, player.y-2, player_shield_map,10 ,16,1);
							GFX_DrowBitMap_P(player.x, player.y, player_map, 11, 11, 1);

							ssd1327_display();
							if (player.lives <= 0)
							{
								play_dead_anim();
								state = st_dead;
							}
						}

						// Move left and do any type-specific behavior.
						enemies[i].x -= 1;

						switch (enemies[i].type)
						{
						case et_tracker:
							if (enemies[i].x < 70)
							{
								if (player.y > enemies[i].y) enemies[i].y += 1;
								if (player.y < enemies[i].y) enemies[i].y -= 1;
							}
							break;
						case et_diver:
							break;
						case et_bobber:
							if((enemies[i].x%4 == 0) && (enemies[i].x%8 == 0))
								enemies[i].y += 4;
							if((enemies[i].x%4 == 0) && !(enemies[i].x%8 == 0))
								enemies[i].y -= 4;
							if (enemies[i].x < 70)
							{
								if (player.y > enemies[i].y) enemies[i].y += 1;
								if (player.y < enemies[i].y) enemies[i].y -= 1;
							}
							break;
						}

						// If we've moved off the left edge, die.
						if (enemies[i].x < -4) enemies[i].active = false;
					}
				}
		}
	}

	//------------- Obs�uga Bossa ---------------
	if(boss.active)
	{
		boss.next_update -= 1;

		//Pozycja bosa
		if(boss.next_update <= 0)
		{
			boss.next_update = boss.update_delay;

			y += dy;
			if (y < 1 || y > (screen_height - 24)) dy = -dy;

			boss.y = y;

			if(boss.y < 10) boss.y = 10;
			if(boss.y > (screen_height - 24)) boss.y = (screen_height - 24);

			boss.x -= 1;
			if(boss.x < 100) boss.x = 100;
		}

		//Cz�stosc sztrza��w bossa
		if((rand()%100) < (boss.level * 2 ))
			boss_shoot();

		for (i = 0; i < num_boss_shots; ++i)
		{
			if (boss_shots[i].active)
				--boss_shots[i].x;
			if (boss_shots[i].x < -4)
				boss_shots[i].active = false;
		}

		for(i = 0; i < num_boss_shots; i++)
		{
			if(boss_shots[i].active)
			{
				if (colliding(boss_shots[i].x,boss_shots[i].y, player.x, player.y) 	||
					colliding(boss_shots[i].x,boss_shots[i].y, player.x, player.y+5) 	||
					colliding(boss_shots[i].x,boss_shots[i].y, player.x+7, player.y)	||
					colliding(boss_shots[i].x,boss_shots[i].y, player.x+7, player.y+5))
				{
					player.lives -= 1;;
					boss_shots[i].active = false;
					GFX_DrowBitMap_P(boss_shots[i].x+2, boss_shots[i].y, explosion_map,10,10,1);
					GFX_DrowBitMap_P(player.x + 8, player.y-2, player_shield_map,10 ,16,1);
					GFX_DrowBitMap_P(player.x, player.y, player_map, 11, 11, 1);

					ssd1327_display();
					if (player.lives <= 0)
					{
						play_dead_anim();
						state = st_dead;
					}
				}
			}
		}

		// Strza�y gracza do bosa
		for(i = 0; i < num_shots; i++)
		{
			if(shots[i].active)
			{
				if(colliding(boss.x, boss.y, shots[i].x, shots[i].y) ||
				   colliding(boss.x, boss.y+6, shots[i].x, shots[i].y) ||
				   colliding(boss.x, boss.y+12, shots[i].x, shots[i].y))
				{
					boss.lives -= 1;
					shots[i].active = false;
					GFX_DrowBitMap_P(shots[i].x, shots[i].y, explosion_map, 10,10,1);

					if(boss.lives <= 0)
					{
						boss.active = false;
						//boss.level += 1;
						boss.lives = 0;
						break;
					}
				}
			}
		}
	}
	// zamalowanie i dezaktywacja strza��w pozosta�ych po bosie
	if(!boss.active)
	{
		for(i = 0; i < num_boss_shots; i++)
		{
			if(boss_shots[i].active)
			{
				boss_shots[i].active = false;
				GFX_DrowBitMap_P(boss_shots[i].x, boss_shots[i].y, player_shot_map,4,1,0);
			}
		}
	}
	//-------------------------------------------

	if (button_pressed())
		shoot();

	// Check for enemy/shot collisions.
	for (i = 0; i < num_shots; ++i)
	{
		for (j = 0; j < num_enemies; ++j)
		{
			if (shots[i].active && enemies[j].active)
			{
				if (colliding(enemies[j].x, enemies[j].y, shots[i].x, shots[i].y))
				{
					enemies[j].active = false;
					shots[i].active = false;
					player.score += 1;
					GFX_DrowBitMap_P(enemies[j].x, enemies[j].y, explosion_map,10,10,1);
				}
			}
		}
	}
	if ((rand()%100) < (player.level * 2) && !(boss.active)) //czesto�c dodawnaia przeciwnik�w
			add_enemy();
}

void drow_game(void)
{
	/*
	 * Rysowanie ca�ej grafiki podczas gry.
	 */
	uint8_t i;

	//rysownanie informacji o grze
	GFX_PutInt(5,0,player.score,1,1,0);
	//GFX_PutInt(5,0,boss.level,1,1,0);
	GFX_DrowBitMap_P(102,0,lives_map,8,6,1);
	GFX_PutInt(122,0,player.lives,1,1,0);
	GFX_DrowBitMap_P(50,0,Level_map,20,7,1);
	GFX_PutInt(80,0,player.level,1,1,0);

	//rysowniae grafiki strza�u gracza
	for(i = 0; i < num_shots; i++)
	{
		if(shots[i].active)
		{
			GFX_DrowBitMap_P(shots[i].x, shots[i].y, player_shot_map,4,1,1);
		}
	}

	//rysowniae grafiki strza�u bosa
	if(boss.active)
	{
		for(i = 0; i < num_boss_shots; i++)
		{
			if(boss_shots[i].active)
			{
				GFX_DrowBitMap_P(boss_shots[i].x, boss_shots[i].y, player_shot_map,4,1,1);
			}
		}
	}
	//rysownaie grafiki przeciwnik�w
	for(i = 0; i < num_enemies; i++)
	{
		if(enemies[i].active)
		{
			if(enemies[i].type == et_tracker)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,5,5,1);
			if(enemies[i].type == et_diver)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,3,7,1);
			if(enemies[i].type == et_bobber)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,4,4,1);
		}
	}
	//rysowanie grafiki gracza
	GFX_DrowBitMap_P(player.x,player.y,player_map,11,11,1);

	//rysowanie t�a
	for(i = 0; i < num_backgrand; i++)
	{
		if(backgrand[i].active)
		{
			ssd1327_setPixel(backgrand[i].x, backgrand[i].y,1);
		}
	}

	//rysowanie bos�w
	if(boss.active && player.game_progres == 30)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_1,10,18,1);
	if(boss.active && player.game_progres == 60)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_2, 10, 18, 1);

}

void run_game (void)
{
	/*
	 * G��wna p�tla gry, czyli wykonywanie po kolei odpowiednich sekwencji.
	 */
	drow_game();
	ssd1327_display();
	ssd1327_CLR();

	update_lvl();
	update_scene();
	update_backgrand();

}

void run_menu (void)
{
	/*
	 * Startowy ekran, podstawowe info. dla gracza na pocz�tku
	 */
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 65) dx = -dx;
	ssd1327_CLR();

	GFX_DrowBitMap_P(x,10,uGalaxy_map,54,16,1);
	GFX_DrowRoundRect(15,34,93,20,8,1);;
	GFX_DrowBitMap_P(26, 37, PressToStart_map, 66,10,1);

	if(button_pressed())
	{
		start_game();
		state = st_playing;
	}
	ssd1327_display();

}

void start_game(void)
{
	/*
	 * Funkcja wywo�ywana tylko raz na poczatku gry.
	 * Ma ona za zadanie ustawienie dopowiednich parametr�w
	 * startowych dla gracza jak i ustawnie pierwszego bosa.
	 * Co wi�cej "resetuje" ona tablice pocisk�w i przeciwnik�w
	 */
	uint8_t i;
	//ustawiniania pocz�tkowe gracza
	player.lives = initial_lives;
	player.score = initial_score;
	player.x = initial_x;
	player.y = initial_y;
	player.level = initial_level;
	player.game_progres = initial_game_progres;

	//aktywacja pocisk�w gracza(dezaktywacja, zerownaie)
	for (i = 0; i < num_shots; ++i)
		shots[i].active = false;

	//aktywacja pocisk�w bosa
	for (i = 0; i < num_boss_shots; ++i)
		boss_shots[i].active = false;

	//aktywacja pzreciwnik�w
	for (i = 0; i < num_enemies; i++)
		enemies[i].active = false;

	//aktwyacja boss�w
	boss.active = false;
	boss.lives = 6;
	boss.level = 1;
	boss.x = 140;
	boss.y = 32;
	boss.update_delay = 3;

}

void add_enemy(void)
{
	/*
	 * Dodanie przeciwnika odpowiedniego do aktualengo post�pu gry.
	 * Ustwienie jego parametr�w pocz�tkowych tj. pozycja, odswiezanie,
	 * grafika, typ.
	 */
	uint8_t i;
	uint8_t enemy_type;

	for(i = 0; i < player.level; i++)
	{
		if(!enemies[i].active)
		{
			enemies[i].active = true;
			enemies[i].x = 140;			//pozycja startowa przeciwnika
			enemies[i].y = ((rand()%(screen_height - 10))+10);

			enemy_type = (rand()%100);

			if((enemy_type > 50))				//rodzaj przeciwnika
			{

				enemies[i].type = et_diver;
				enemies[i].update_delay = (rand()%3);	//ustawinie predkosci (mniej = szybciej)
				enemies[i].bit_map = driver_map;		//grafika przeciwnika
			}
			if((enemy_type > 20 && enemy_type < 50) && (player.level > 4))
			{
				enemies[i].type = et_tracker;
				enemies[i].update_delay = ((rand()%3)+1);
				enemies[i].bit_map = tracker_map;
			}
			if((enemy_type < 20) && (player.level > 7))
			{
				enemies[i].type = et_bobber;
				enemies[i].update_delay = ((rand()%3)+2);
				enemies[i].bit_map = bobber_map;
			}
			break;
		}
	}
}

uint8_t button_pressed (void)
{
	/*
	 * Sprawdzanie czy klawisz zosta� wcisniety.
	 * Odpowiednia konstrukcja tej wunkcji zapewnia
	 * stan aktywny na zbocze "narastajace" i uniemozliwia
	 * repetycje gdy klawisz jest dlaej wcisniety.
	 * Jest to szczeg�lnie przydatane przy strzelaniu bo zapobiega
	 * oddawaniu kilku strza��w naraz.
	 */

	if((HAL_GPIO_ReadPin(USER_BTN_1_GPIO_Port, USER_BTN_1_Pin) == GPIO_PIN_SET) && btn_prev == 0)
	{
		btn_prev = 1;
		return 1;
	}

	if(HAL_GPIO_ReadPin(USER_BTN_1_GPIO_Port, USER_BTN_1_Pin) == GPIO_PIN_RESET) //oczekiwania na zwolnienie kalwisza
	{
		btn_prev = 0;
	}
	return 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
