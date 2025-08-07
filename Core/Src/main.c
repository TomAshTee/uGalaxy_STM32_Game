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

/*
 * ToDo:
 * - Dodanie może wprowadzenia fabularnego ?
 * - Dodanie przrywników fabularnych ?
 * - Poprawa czasów trwania poziomów ?
 *
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <math.h>
#include "graphics_map.h"
#include "game_logic.h"
#include "GFX_ssd1327.h"
#include "ssd1327.h"
#include <stdbool.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/*
 * ----------------------------------------------------------------------
 * 					Structures and structure arrays
 */
T_player player; 						//Player
T_shot shots[num_shots]; 				//Player shots
T_shot boss_shots [num_boss_shots];		//Boss shots
T_enemy enemies[num_enemies];			//Enemies
T_backgrand background[num_background];	//Back
T_boss boss;							//Boss in game
T_bonus bonuses[num_bonus];				//Bonuses in game

gamestate state = st_menu;				//Initial game state

uint8_t btn_prev = 0;					//Key operation prevent , repetition

int debug_value;


/*
 * ----------------------------------------------------------------------
 * ----------------------------------------------------------------------
 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

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

void add_background(void)
{
	/*
	 * Adds a single unit (structure) to the entire array.
	 * In addition, it sets the random parameters for its
	 * refreshment and a random position on the Y axis.
	 */
	uint8_t i;

	for(i = 0; i < num_background; i++)
	{
		if(!background[i].active)
		{
			background[i].active 	= true;
			background[i].x 		= 128;
			background[i].y			= (rand()%(screen_height-10)) +10;
			background[i].update_delay = (rand()%6)+2; // def. (rand()%4)+2;

			break;
		}
	}
}

void update_backgrand(void)
{
	/*
	 * Refreshes the background effect, stars.
	 * Moves elements and randomises when a new element have to be added.
	 */
	uint8_t i;

	for(i = 0; i < num_background; i++)
	{
		if(background[i].active)
		{
			background[i].next_update -= 1;
			if(background[i].next_update <= 0)
			{
				background[i].next_update = background[i].update_delay;
				if(background[i].active)
				{
					background[i].x -= 1;

					if(background[i].x <= -2)
						background[i].active = false;
				}
			}
		}
	}

	if ((rand()%100) < num_background_freq) 		//Frequency of background additions
		add_background();

}

void update_lvl(void)
{
	/*
	 * It is responsible for the appropriate appearance of bosses,
	 * their initial parameters and adjusts the level of play to its progress.
	 */
	static uint8_t i = 0;

	//Calculation of game progress
	if(i > 70)
	{
		player.game_progres += 1;
		i = 0;
	}

	//If the boss is not active count the progress
	if(!boss.active) i++;

	//---- Moments of boss appearance -----
	if(player.game_progres == 29)
	{
		boss.active = true;
		boss.lives = 3;
		boss.update_delay = 4;
		player.game_progres += 1;

	}
	if(player.game_progres == 59)
	{
		boss.active = true;
		boss.lives = 6;
		boss.update_delay = 2;
		player.game_progres += 1;
	}
	//--------------------------------------

	//Next levels
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
	 * Screen after player death. Display of score and animated text.
	 */

	static int x = 0, dx = 1;


	x += dx;
	if (x < 1 || x > 55) dx = -dx;
	ssd1327_CLR();
	GFX_DrowBitMap_P(x,(screen_height/2) - 4,Defeated_map,67,16,1);
	GFX_DrowBitMap_P(35,(screen_height/2) + 24,Score_map,37,10,1);
	GFX_PutInt(73,(screen_height/2) + 27,player.score,1,1,0);
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
	 * Animation between separate screens. Gives the illusion of an old game.
	 */
	uint8_t i;

	for (i = 0; i < 10; ++i)
	{
		ssd1327_CLR();
		GFX_FillRect(0,0,128,128,1);
		ssd1327_display();

		ssd1327_CLR();
		ssd1327_display();

	}
}

void shot(void)
{
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
		if (!shots[i].active)
		{
			shots[i].active = true;
			shots[i].x = 11;
			shots[i].y = player.y + 5;

			//Setting the type of shot
			switch(player.shoot_type)
			{
			case st_normal:
				shots[i].type = st_normal;
				break;
			case st_tracker:

				/*
				 * Is there at least one active opponent who is untargeted.
				 * If so start tracking him.
				 * */
				for(int j = 0; j < num_enemies; j++)
				{
					if(enemies[j].active && !enemies[j].tracked_by_missile)
					{
						is_any_enemies_active = true;

						temp_distance = sqrt(pow(enemies[j].x - player.x, 2) + pow(enemies[j].y - player.y, 2));

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
					enemies[closest_enemy_number].track_number = random_tracking_number;
					enemies[closest_enemy_number].tracked_by_missile = true;
					shots[i].type = st_tracker;
					shots[i].track_number = random_tracking_number;
				} else {
					// If you haven't found a target act like a normal shot
					shots[i].type = st_normal;
				}
				break;
			}
			return;
		}
	}
}

void boss_shoot(void)
{
	/*
	 * Handling boss shots.
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
	 * Checking whether objects collide with each other.
	 */
	int dx = abs(x0 - x1);
	int dy = abs(y0 - y1);
	return dx < 6 && dy < 9;
}

void update_scene(void)
{
	/*
	 * The logic of the whole game
	 */
	uint8_t i,j;

	static uint8_t y = 0, dy = 1;
	// Read analog stick
	int stick = joystick_value_y();

	if (stick < 1000)
		player.y -= 1;
	else if (stick > 3500)
		player.y += 1;

	// Keeping the player within the screen
	if (player.y < 10) player.y = 10;
	if (player.y > (screen_height - 14)) player.y = (screen_height - 14);

	// Shifting shots forward
	bool shoot_updated = false;

	for (i = 0; i < num_shots; ++i)
	{
		switch(shots[i].type)
		{
		case st_normal:
			if (shots[i].active)
				shots[i].x++;
			if (shots[i].x > 128)
				shots[i].active = false;
			break;
		case st_tracker:

			for (int j = 0; j < num_enemies; j++)
			{
				if (shots[i].track_number == enemies[j].track_number)
				{
					if(shots[i].x > enemies[j].x) shots[i].x -= 2;
					if(shots[i].x < enemies[j].x) shots[i].x += 2;
					if(shots[i].y > enemies[j].y) shots[i].y -= 2;
					if(shots[i].y < enemies[j].y) shots[i].y += 2;
					shoot_updated = true;
					break;
				}
			}
			//Remove tracking missiles that have no target
			if(!shoot_updated && shots[i].type == st_tracker)
			{
				shots[i].active = false;
				shots[i].track_number = 0;
				shoot_updated = false;
			}

			//Remove off-map shots
			if(shots[i].x > 128)
			{
				shots[i].active = false;
				shots[i].track_number = 0;
			}
			break;
		}

	}

	//Remove the markers on enemies whose shots have been used on others
	bool is_there_a_missile;
	for(i = 0; i < num_enemies; i++)
	{
		is_there_a_missile = false;

		for(j = 0; j < num_shots; j++)
		{
			if(enemies[i].track_number == shots[j].track_number)
				is_there_a_missile = true;
		}

		if(!is_there_a_missile)
			enemies[i].track_number = 0;
	}

	// Updated enemies
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

						//Checking for collisions between opponents and the player
						if (colliding(enemies[i].x,enemies[i].y, player.x, player.y) 	||
							colliding(enemies[i].x,enemies[i].y, player.x, player.y+5) 	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y)	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y+5)
							)
						{
							player.lives -= 1;;
							enemies[i].active = false;
							enemies[i].tracked_by_missile = false;
							enemies[i].track_number = 0;
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

						// Moving to the left and making special moves
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

						// If off-screen, deactivation
						if (enemies[i].x < -4)
						{
							enemies[i].active = false;
							enemies[i].tracked_by_missile = false;
							enemies[i].track_number = 0;
						}
					}
				}
		}
	}

	//------------- Boss service ---------------
	if(boss.active)
	{
		boss.next_update -= 1;

		//Boss position
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

		//Frequency of boss shots
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

		// Player's shots to the boss
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
					shots[i].track_number = 0;
					GFX_DrowBitMap_P(shots[i].x, shots[i].y, explosion_map, 10,10,1);

					if(boss.lives <= 0)
					{
						boss.active = false;
						boss.lives = 0;
						break;
					}
				}
			}
		}
	}

	//Painting over and deactivating shots left over from the boss
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
		shot();

	// Checking the collision of a player's shots with opponents. Adding Bonuses
	for (i = 0; i < num_shots; ++i)
	{
		for (j = 0; j < num_enemies; ++j)
		{
			if (shots[i].active && enemies[j].active)
			{
				if (colliding(enemies[j].x, enemies[j].y, shots[i].x, shots[i].y))
				{
					enemies[j].active = false;
					enemies[j].tracked_by_missile = false;
					enemies[j].track_number = 0;
					shots[i].active = false;
					player.score += 1;
					GFX_DrowBitMap_P(enemies[j].x, enemies[j].y, explosion_map,10,10,1);

					//Dodanie bonusa w miejscu zestrzelenia
					if((rand()%100) < frequ_bonus)
						add_bonus(enemies[j].x, enemies[j].y);
				}
			}
		}
	}
	if ((rand()%100) < (player.level * 2) && !(boss.active)) //Frequency of adding opponents according to level
			add_enemy();
}

void drow_game(void)
{
	/*
	 * Drawing all game graphics
	 */
	uint8_t i;

	//Rsownanie informacji o grze
	GFX_PutInt(5,0,player.score,1,1,0);
	GFX_DrowBitMap_P(102,0,lives_map,8,6,1);
	GFX_PutInt(114,0,player.lives,1,1,0);
	GFX_DrowBitMap_P(50,0,Level_map,20,7,1);
	GFX_PutInt(80,0,player.level,1,1,0);

	//Drawing graphics of a player's shot
	for(i = 0; i < num_shots; i++)
	{
		if(shots[i].active)
		{
			GFX_DrowBitMap_P(shots[i].x, shots[i].y, player_shot_map,4,1,1);
		}
	}

	//Drawing graphics of a boss shot
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

	//Drawing graphics of enemies
	for(i = 0; i < num_enemies; i++)
	{
		if(enemies[i].active)
		{
			if(enemies[i].type == et_tracker)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,5,5,1);
			if(enemies[i].type == et_diver)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,3,7,1);
			if(enemies[i].type == et_bobber)
				GFX_DrowBitMap_P(enemies[i].x, enemies[i].y, enemies[i].bit_map,5,5,1);
		}
	}

	//Drawing the player's graphics
	GFX_DrowBitMap_P(player.x,player.y,player_map,11,11,1);

	//Drawing a background
	for(i = 0; i < num_background; i++)
	{
		if(background[i].active)
		{
			ssd1327_setPixel(background[i].x, background[i].y,(rand()%15));
		}
	}

	//Drawing Boss
	if(boss.active && player.game_progres == 30)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_1,10,18,1);
	if(boss.active && player.game_progres == 60)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_2, 10, 18, 1);

	//Drawing bonuses
	for(i = 0; i < num_bonus; i++)
	{
		if(bonuses[i].active)
		{
			switch(bonuses[i].type)
			{
			case bt_live:
				GFX_DrowBitMap_P(bonuses[i].x, bonuses[i].y, bonus_live_map, 7, 7, 1);
				break;
			case bt_tracker_shoot:
				GFX_DrowBitMap_P(bonuses[i].x, bonuses[i].y, bonus_tracker_shoot_map, 7, 7, 1);
				break;
			}
		}
	}
}

void run_game (void)
{
	/*
	 * The main loop of the game, executing the relevant functions one by one
	 */
	drow_game();
	ssd1327_display();
	ssd1327_CLR();

	update_lvl();
	update_scene();
	update_backgrand();
	update_bonus();

}

void run_menu (void)
{
	/*
	 * Start screen, basic information for the player at the beginning
	 */
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 65) dx = -dx;
	ssd1327_CLR();

	GFX_DrowBitMap_P(x,(screen_height/2) - 10,uGalaxy_map,54,16,1);
	GFX_DrowRoundRect(15,(screen_height/2) + 34,93,20,8,1);
	GFX_DrowBitMap_P(26, (screen_height/2)+ 37, PressToStart_map, 66,10,1);

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
	 * This function is only called once at the start of the game.
	 * It has the task of setting the starting parameters for
	 * the player and the first boss. Furthermore, it "resets"
	 * the arrays of shot, opponents and bonuses.
	 */
	uint8_t i;
	//Player initial settings
	player.lives = initial_lives;
	player.score = initial_score;
	player.x = initial_x;
	player.y = initial_y;
	player.level = initial_level;
	player.game_progres = initial_game_progres;
	player.shoot_type = st_normal;

	//Deactivation of player shots
	for (i = 0; i < num_shots; ++i)
		shots[i].active = false;

	//Deactivation of boss shots
	for (i = 0; i < num_boss_shots; ++i)
		boss_shots[i].active = false;

	//Deactivation enemies
	for (i = 0; i < num_enemies; i++)
	{
		enemies[i].active = false;
		enemies[i].track_number = 0;
		enemies[i].tracked_by_missile = false;
	}

	//Deactivation bonuses
	for (i = 0; i < num_bonus; i++)
		bonuses[i].active = false;

	//Deactivation boss
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
	 * Adding an opponent appropriate to the current progress of the game.
	 * Setting its initial parameters, i.e. position, refreshment, graphics, type.
	 */
	uint8_t i;
	uint8_t enemy_type;

	for(i = 0; i < player.level; i++)
	{
		if(!enemies[i].active)
		{
			enemies[i].active = true;
			enemies[i].x = 140;
			enemies[i].y = ((rand()%(screen_height - 10))+10);
			enemies[i].tracked_by_missile = false;
			enemies[i].track_number = 0;

			enemy_type = (rand()%100);

			if((enemy_type > 50))
			{

				enemies[i].type = et_diver;
				enemies[i].update_delay = (rand()%3);	//Speed setting (less = faster)
				enemies[i].bit_map = driver_map;
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

void add_bonus(int x, int y)
{
	/*
	 * Adding a bonus in the place after an opponent has been shot down.
	 * Checking whether this is possible.
	 */

	uint8_t i;
	int bonus_type;

	for (i = 0; i < num_bonus; i++)
	{
		if (!bonuses[i].active)
		{
			bonuses[i].active = true;
			bonuses[i].x = x;
			bonuses[i].y = y;
			bonuses[i].update_delay = 3;

			bonus_type = rand()%100;

			if(bonus_type > 30)
			{
				bonuses[i].bit_map = bonus_live_map;
				bonuses[i].type = st_normal;
			}
			if(bonus_type < 30)
			{
				bonuses[i].bit_map = bonus_tracker_shoot_map;
				bonuses[i].type = st_tracker;
			}
			return;
		}
	}
}

void update_bonus(void)
{
	/*
	 * Checking whether a player has hovered over a bonus.
	 * Moving a bonus on the map
	 * */
	int i = 0;

	//Check duration of st_tracekr if active

	if(player.bonus_duration > 0)
		player.bonus_duration -= 1;
	if(player.bonus_duration == 0 && player.shoot_type == st_tracker)
		player.shoot_type = st_normal;

	// Checking whether a player has hovered over a bonus
	for (i = 0; i < num_bonus; i++)
	{
		if(bonuses[i].active)
		{
			bonuses[i].next_update -= 1;
			if(bonuses[i].next_update <= 0)
			{
				if(bonuses[i].active)
				{
					bonuses[i].next_update = bonuses[i].update_delay;

					if (colliding(bonuses[i].x,bonuses[i].y, player.x, player.y) 	||
						colliding(bonuses[i].x,bonuses[i].y, player.x, player.y+5) 	||
						colliding(bonuses[i].x,bonuses[i].y, player.x+7, player.y)	||
						colliding(bonuses[i].x,bonuses[i].y, player.x+7, player.y+5)
						)
					{
						switch(bonuses[i].type)
						{
						case bt_live:
							player.lives += 1;
							bonuses[i].active = false;
							break;
						case bt_tracker_shoot:
							player.shoot_type = st_tracker;
							player.bonus_duration = duration_bonus + (player.level * 50);
							bonuses[i].active = false;
							break;
						}

					}

					bonuses[i].x -= 1;
					if(bonuses[i].x <= -2){
						bonuses[i].active = false;
					}
				}
			}
		}
	}
}

uint8_t button_pressed (void)
{
	/*
	 * Checking whether a button has been pressed. Prevention of repetition.
	 */

	if((HAL_GPIO_ReadPin(USER_BTN_1_GPIO_Port, USER_BTN_1_Pin) == GPIO_PIN_SET) && btn_prev == 0)
	{
		btn_prev = 1;
		return 1;
	}

	if(HAL_GPIO_ReadPin(USER_BTN_1_GPIO_Port, USER_BTN_1_Pin) == GPIO_PIN_RESET)
	{
		btn_prev = 0;
	}
	return 0;
}

int joystick_value_y (void){
	return HAL_ADC_GetValue(&hadc1);
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
