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
  *
  * @todo
  * - Creation of a BOT for the game instead of the player (some kind of special sequence to run)
  * - Breakdown of overly large functions, e.g. GameTick()
  * - Adding shield support similarly to what happens with explosion support.
  * 	Add shield to Boss (shield == number of lives of Boss)
  * - Add debounce support to the button (input.c), also FSM ?
  * - Add DEBUG mode -> #define DEBUG_MODE 1
  * - Adding bonuses: quick shots, protective shield.
  * - Introduction of tests
  * - Addition of storyline breakers and storyline introduction.
  *
  *
  ******************************************************************************
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <math.h>
#include "graphics_map.h"
#include "game_logic.h"
#include "GFX_ssd1327.h"
#include "ssd1327.h"
#include "input.h"
#include <stdbool.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern void GameInit(GameCtx* g);
extern void GameTick(GameCtx* g, InputSnapshot* in);
extern void GameDraw(GameCtx* g, InputSnapshot* in);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t btn_prev = 0;					//Key operation prevent , repetition
InputSnapshot input_Snap;

volatile bool g_logicTick = false;
volatile bool g_renderTick = false;
InputSnapshot in_cache = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

//  // Konfiguracja ADC z DMA
//  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
//  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)value, 2);

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADC_Start(&hadc1);
  HAL_TIM_Base_Start_IT(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  SSD1327_SpiInit(&hspi1);
  SSD1327_BeginFrame();
  SSD1327_Display();

  //start_game();
  GameInit(&g_singleton);
  //InputInit(&hadc1, USER_BTN_1_GPIO_Port, USER_BTN_1_Pin);
  InputInit(&hadc1, USER_BTN_1_GPIO_Port, USER_BTN_1_Pin, USER_BTN_2_GPIO_Port, USER_BTN_2_Pin);
  //add_enemy();

  while (1)
  {

	  switch(GameGetState(&g_singleton)){

	  case GS_Menu:
		  if(g_logicTick){
			  g_logicTick = false;
			  input_Snap = InputRead();
			  RunMenuTick(&input_Snap);
		  }

		  if(g_renderTick && !SSD1327_IsBusy()){
			  g_renderTick = false;

			  SSD1327_BeginFrame();
			  RunMenu(&input_Snap);
			  SSD1327_Present();
		  }
		  break;

	  case GS_Playing:
		  if(g_logicTick){
			  g_logicTick = false;
			  input_Snap = InputRead();
			  RunGameTick(&input_Snap, &g_singleton);
		  }

		  if(g_renderTick && !SSD1327_IsBusy()){
			  g_renderTick = false;

			  SSD1327_BeginFrame();
			  RunGame(&input_Snap);
			  SSD1327_Present();
		  }

		  break;

	  case GS_Dead:
		  if(g_logicTick){
			  g_logicTick = false;
			  input_Snap = InputRead();
			  RunDeadTick(&input_Snap);
		  }

		  if(g_renderTick && !SSD1327_IsBusy()){
			  g_renderTick = false;

			  SSD1327_BeginFrame();
			  RunDead(&input_Snap);
			  SSD1327_Present();
		  }

		  break;
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
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* SPI1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);
  /* TIM6_DAC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

/* USER CODE BEGIN 4 */
void RunDead(InputSnapshot* in)
{
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 55) dx = -dx;

	GFX_DrawBitMap_P(x,(SCREEN_HEIGHT/2) - 4,Defeated_map,67,16,1);
	GFX_DrawBitMap_P(35,(SCREEN_HEIGHT/2) + 24,Score_map,37,10,1);
	GFX_PutInt(73,(SCREEN_HEIGHT/2) + 27,GameGetPlayerScore(&g_singleton),1,1,0);
}

void RunDeadTick(InputSnapshot* in){
	if(in->btn1State == GPIO_PIN_SET)
	{
		PlayDeadAnim();
		GameSetState(&g_singleton, GS_Menu);
	}
}

void PlayDeadAnim(void)
{
	uint8_t i;

	for (i = 0; i < 10; ++i)
	{
		SSD1327_BeginFrame();
		GFX_FillRect(0,0,128,128,1);
		SSD1327_Display();
		HAL_Delay(10);

		SSD1327_BeginFrame();
		SSD1327_Display();
		HAL_Delay(10);
	}
}

void RunGame (InputSnapshot* in)
{
	GameDraw(&g_singleton, in);
}

void RunGameTick(InputSnapshot* in, GameCtx* g){

	GameLevelUpdate(g);
	GameTick(g, in);
	GameUpdateBackground(g);
	GameUpdateBonus(g);
	GameUpdateExplosion(g);
}

void RunMenu (InputSnapshot* in)
{
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 65) dx = -dx;

	GFX_DrawBitMap_P(x,(SCREEN_HEIGHT/2) - 10,uGalaxy_map,54,16,1);
	GFX_DrawRoundRect(15,(SCREEN_HEIGHT/2) + 34,93,20,8,1);
	GFX_DrawBitMap_P(26, (SCREEN_HEIGHT/2)+ 37, PressToStart_map, 66,10,1);

}
void RunMenuTick(InputSnapshot* in){
	if(in->btn1State == GPIO_PIN_SET)
	{
		GameInit(&g_singleton);
		GameSetState(&g_singleton, GS_Playing);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM6)
    {
        g_logicTick  = true;
        g_renderTick = true;
    }
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
#ifdef USE_FULL_ASSERT
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
