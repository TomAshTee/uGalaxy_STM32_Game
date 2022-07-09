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
 * - Wprowadzenie #define jako cześtotliwości dodawania tła
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
#include "ssd1327.h"
#include <stdbool.h>

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
 * 					Struktóry i tablice struktór
 */
T_player player; 						//Tworzenie gracza
T_shot shoots[num_shots]; 				//Tworzenie strzałów gracza
T_shot boss_shots [num_boss_shots];		//Tworzenie strzałów od bosa
T_enemy enemies[num_enemies];			//Tworzenie przeciwników
T_backgrand backgrand[num_backgrand];	//Tworzenie tła
T_boss boss;							//Boss w grze
T_bonus bonuses[num_bonus];				//Bonusy w grze

gamestate state = st_menu;		//Poczatkowy stan gry

uint8_t score_1, score_0;		//Przechwycenie najelepszych wyników
uint8_t btn_prev = 0;			//Obsługa klawiszy zapobiega , repetycji

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

void add_backgrand(void)
{
	/*
	 * Dodaje pojedynczą jednostke (strukture) do calej tablicy.
	 * Dodatkowo ustawia losowe parametry odswiezania jej oraz
	 * losowe położenie na osi Y.
	 */
	uint8_t i;

	for(i = 0; i < num_backgrand; i++)
	{
		if(!backgrand[i].active)
		{
			backgrand[i].active = true;
			backgrand[i].x 		= 128;
			backgrand[i].y		= (rand()%(screen_height-10)) +10;
			backgrand[i].update_delay = (rand()%6)+2; // def. (rand()%4)+2;

			break;
		}
	}
}

void update_backgrand(void)
{
	/*
	 * Odswierza efekt tła, gwiazd.
	 * Przesuwa elementy , oraz losuje kiedy ma
	 * zostać dodany nowy element.
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

	if ((rand()%100) < num_backgrand_freq) 		//Częstotliwość dodawania tła
		add_backgrand();

}

void update_lvl(void)
{
	/*
	 * Odpowiada za odpowienie pojawianie sie bosów,
	 * ich poczatkowych parametrów oraz dostosowuje poziom gry
	 * do jej postepu.
	 */
	static uint8_t i = 0;

	//Obliczanie postepu gry
	if(i > 70) //poprawne dzia�anie 128 (cały ekran)
	{
		player.game_progres += 1;
		i = 0;
	}

	//Jesli boss nie aktywny naliczaj postep
	if(!boss.active) i++;

	//---- Momenty pojawienia sie bosa -----
	//Oraz jego parametry
	//boss.update_delay = x; powienien on posiadac pażystą liczbę ponieważ
	//w innym wypadku z niewiadomych przyczyn wysupje sie jego animacja.
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
	 * Ekran po smierci gracza. Wyswietlanie wybiektu oraz
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
	//Pokazanie wyniku i najwyższego wyniku

	x += dx;
	if (x < 1 || x > 55) dx = -dx;
	ssd1327_CLR();
	GFX_DrowBitMap_P(x,(screen_height/2) - 4,Defeated_map,67,16,1);
	GFX_DrowBitMap_P(35,(screen_height/2) + 24,Score_map,37,10,1);
	GFX_PutInt(73,(screen_height/2) + 27,player.score,1,1,0);
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
	 * Animacja miedzy poczczegolnymi ekranami. Daje złudzenie starej gry.
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

void shoot(void)
{
	/*
	 * Dodanie struktury strzału do tablicy strzałów
	 * oraz ustalenie wstepnych jego parametrów.
	 *
	 * Obs�uga strza��w gracza.
	 */
	uint8_t i;

	bool is_any_enemies_active;

	int closest_enemy_number;
	int temp_x;
	int temp_y;
	double temp_distance;
	double smolest_distance = 500;
	int random_tracking_number;

	for (i = 0; i < num_shots; ++i)
	{
		if (!shoots[i].active)
		{
			shoots[i].active = true;
			shoots[i].x = 11;
			shoots[i].y = player.y + 5;

			//Ustawienie rodzaju strzału
			switch(player.shoot_type)
			{
			case st_normal:
				shoots[i].type = st_normal;
				break;
			case st_tracker:

				/*
				 * Czy jest chociaż jeden aktywny przeciwnik który jest nienamieżany.
				 * Jeśli tak zacznij go namieżać.
				 * */
				for(int j = 0; j < num_enemies; j++)
				{
					if(enemies[j].active && !enemies[j].tracked_by_missile)
					{
						is_any_enemies_active = true;
						temp_x = enemies[j].x;
						temp_y = enemies[j].y;

						temp_distance = sqrt(pow(enemies[j].x - player.x, 2) + pow(enemies[j].y - player.y, 2));

						if (temp_distance < smolest_distance)
						{
							smolest_distance = temp_distance;
							closest_enemy_number = j;
						}
						break;
					}
				}
				if(is_any_enemies_active)
				{
					random_tracking_number = rand();
					enemies[closest_enemy_number].truck_number = random_tracking_number;
					enemies[closest_enemy_number].tracked_by_missile = true;
					shoots[i].type = st_tracker;
					shoots[i].truck_number = random_tracking_number;
				} else {
					// Jeśli nie znalazłeś żadnego celu zachowój się jak normlany strzał
					shoots[i].type = st_normal;
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
	 * Tu jest zawarta cała logika gry
	 */
	uint8_t i,j;

	static uint8_t y = 0, dy = 1;
	// Read analog stick
	int stick = joystick_value_y();

	if (stick < 1000)
		player.y -= 1;
	else if (stick > 3500)
		player.y += 1;

	// Trzymanie gracza w ramach ekranu
	if (player.y < 10) player.y = 10;
	if (player.y > (screen_height - 14)) player.y = (screen_height - 14);

	// Przesunięcie strałów do przodu

	bool shoot_updated = false;

	for (i = 0; i < num_shots; ++i)
	{
		switch(shoots[i].type)
		{
		case st_normal:
			if (shoots[i].active)
				shoots[i].x++;
			if (shoots[i].x > 128)
				shoots[i].active = false;
			break;
		case st_tracker:

			for (int j = 0; j < num_enemies; j++)
			{
				if (shoots[i].truck_number == enemies[j].truck_number)
				{
					//shoots[i].x += 1;
					if(shoots[i].x > enemies[j].x) shoots[i].x -= 2;
					if(shoots[i].x < enemies[j].x) shoots[i].x += 2;
					if(shoots[i].y > enemies[j].y) shoots[i].y -= 2;
					if(shoots[i].y < enemies[j].y) shoots[i].y += 2;
					shoot_updated = true;
					break;
				}
			}
			//Usuń śledzące pociski które nie mają celu
			if(!shoot_updated && shoots[i].type == st_tracker)
			{
				shoots[i].active = false;
				shoots[i].truck_number = 0;
				shoot_updated = false;
			}

			if(shoots[i].x > 128)
			{
				shoots[i].active = false;
				shoots[i].truck_number = 0;
			}
			break;
		}

	}

	// Aktualizacja przeciwników
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

						//Sprawdzanie kolizji przeciwników z graczem
						if (colliding(enemies[i].x,enemies[i].y, player.x, player.y) 	||
							colliding(enemies[i].x,enemies[i].y, player.x, player.y+5) 	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y)	||
							colliding(enemies[i].x,enemies[i].y, player.x+7, player.y+5)
							)
						{
							player.lives -= 1;;
							enemies[i].active = false;
							enemies[i].tracked_by_missile = false;
							enemies[i].truck_number = 0;
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

						// Przesunięcie w lewo i wykonanie specjalnych ruchów
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

						// Jeśli poza ekranem, dezaktywacja
						if (enemies[i].x < -4)
						{
							enemies[i].active = false;
							enemies[i].tracked_by_missile = false;
							enemies[i].truck_number = 0;
						}
					}
				}
		}
	}

	//------------- Obsługa Bossa ---------------
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

		//Częstotliwość strzałów bosa
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

		// Strzały gracza do bosa
		for(i = 0; i < num_shots; i++)
		{
			if(shoots[i].active)
			{
				if(colliding(boss.x, boss.y, shoots[i].x, shoots[i].y) ||
				   colliding(boss.x, boss.y+6, shoots[i].x, shoots[i].y) ||
				   colliding(boss.x, boss.y+12, shoots[i].x, shoots[i].y))
				{
					boss.lives -= 1;
					shoots[i].active = false;
					shoots[i].truck_number = 0;
					GFX_DrowBitMap_P(shoots[i].x, shoots[i].y, explosion_map, 10,10,1);

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

	//Zamalowanie i dezaktywacja strzałów pozostałych po bosie
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

	// Sprawdzenie kolicji strzałów gracza z przeciwnikami. Dodawania Bonusów
	for (i = 0; i < num_shots; ++i)
	{
		for (j = 0; j < num_enemies; ++j)
		{
			if (shoots[i].active && enemies[j].active)
			{
				if (colliding(enemies[j].x, enemies[j].y, shoots[i].x, shoots[i].y))
//				if(	colliding(enemies[j].x,enemies[j].y, shoots[i].x, shoots[i].y) 	||
//					colliding(enemies[j].x + 2,enemies[j].y, shoots[i].x, shoots[i].y) 	||
//					colliding(enemies[j].x,enemies[j].y + 2, shoots[i].x, shoots[i].y)	||
//					colliding(enemies[j].x + 2,enemies[j].y + 2, shoots[i].x, shoots[i].y))
				{
					enemies[j].active = false;
					enemies[j].tracked_by_missile = false;
					enemies[j].truck_number = 0;
					shoots[i].active = false;
					player.score += 1;
					GFX_DrowBitMap_P(enemies[j].x, enemies[j].y, explosion_map,10,10,1);

					//Dodanie bonusa w miejscu zestrzelenia
					if((rand()%100) < frequ_bonus)
						add_bonus(enemies[j].x, enemies[j].y);
				}
			}
		}
	}
	if ((rand()%100) < (player.level * 2) && !(boss.active)) //Częstotliwość dodawania przeciwników w zależności od poziomu
			add_enemy();
}

void drow_game(void)
{
	/*
	 * Rysowanie ca�ej grafiki podczas gry.
	 */
	uint8_t i;

	//Rsownanie informacji o grze
	GFX_PutInt(5,0,player.score,1,1,0);
	GFX_DrowBitMap_P(102,0,lives_map,8,6,1);
	GFX_PutInt(122,0,player.lives,1,1,0);
	GFX_DrowBitMap_P(50,0,Level_map,20,7,1);
	GFX_PutInt(80,0,player.level,1,1,0);

	//Rysowniae grafiki strzału gracza
	for(i = 0; i < num_shots; i++)
	{
		if(shoots[i].active)
		{
			GFX_DrowBitMap_P(shoots[i].x, shoots[i].y, player_shot_map,4,1,1);
		}
	}

	//Rysowniae grafiki strzału bosa
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
	//Rysownaie grafiki przeciwników
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
	//Rysowanie grafiki gracza
	GFX_DrowBitMap_P(player.x,player.y,player_map,11,11,1);

	//Rysowanie tła
	for(i = 0; i < num_backgrand; i++)
	{
		if(backgrand[i].active)
		{
			ssd1327_setPixel(backgrand[i].x, backgrand[i].y,(rand()%15));
		}
	}

	//Rysowanie bosów
	if(boss.active && player.game_progres == 30)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_1,10,18,1);
	if(boss.active && player.game_progres == 60)
		GFX_DrowBitMap_P(boss.x, boss.y, boss_map_2, 10, 18, 1);

	//Rysowanie bonusów
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

	// DEBUG VALUE
	GFX_PutInt(0, 100, debug_value, 1, 1, 0);

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
	update_bonus();

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
	 * Funkcja wywoływana tylko raz na poczatku gry.
	 * Ma ona za zadanie ustawienie dopowiednich parametrów
	 * startowych dla gracza jak i ustawnie pierwszego bosa.
	 * Co więcej "resetuje" ona tablice pocisków, przeciwników i bonosów
	 */
	uint8_t i;
	//Ustawiniania początkowe gracza
	player.lives = initial_lives;
	player.score = initial_score;
	player.x = initial_x;
	player.y = initial_y;
	player.level = initial_level;
	player.game_progres = initial_game_progres;
	player.shoot_type = st_normal;

	//Dezaktywacja pocisków gracza
	for (i = 0; i < num_shots; ++i)
		shoots[i].active = false;

	//Dezaktywacja pocisków bosa
	for (i = 0; i < num_boss_shots; ++i)
		boss_shots[i].active = false;

	//Dezaktywacja przeciwników
	for (i = 0; i < num_enemies; i++)
	{
		enemies[i].active = false;
		enemies[i].truck_number = 0;
		enemies[i].tracked_by_missile = false;
	}

	//Dezaktywacja bonusów
	for (i = 0; i < num_bonus; i++)
		bonuses[i].active = false;

	//Dezaktywacja bossa
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
			enemies[i].tracked_by_missile = false;
			enemies[i].truck_number = 0;

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

void add_bonus(int x, int y)
{
	/*
	 * Dodanie bonusa w mijscu po zestrzelonym przeciwniku.
	 * Sprawdzenie czy jest to możliwe.
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
	 * Sprawdzanie czy gracz najechał na bonus.
	 * Przesuwanie bonusa na mapie
	 * */
	int i = 0;

	//Sprawdzenie czasu trwania st_tracekr jeśli jest aktywny

	if(player.bonus_duration > 0)
		player.bonus_duration -= 1;
	if(player.bonus_duration == 0 && player.shoot_type == st_tracker)
		player.shoot_type = st_normal;

	debug_value = player.bonus_duration;

	// Sprawdzanie czy gracz najechał na bonus
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

					//Sprawdzanie kolizji z graczem
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
	/*k
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
