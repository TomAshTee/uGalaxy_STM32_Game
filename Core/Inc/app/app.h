/**
 * @file    app.h
 * @author  Tomasz
 * @date    Jan 23, 2026
 * @brief   Application layer (top-level control).
 *
 * This module owns the high-level application flow:
 * - initialization of display/input/game,
 * - state dispatch (menu / playing / dead),
 * - logic/render tick handling.
 */

#ifndef INC_APP_APP_H_
#define INC_APP_APP_H_

#include "app/input/input.h"
#include "app/game/game_logic.h"

/** ##########################################################################
 *  @name Structs
 *  @brief Holds pointers and runtime flags used by the application layer.
 *  Tick flags are typically set from a timer ISR and consumed in main loop.
 *  @{
 *  ##########################################################################
 */

typedef struct {
	GameCtx *game;						/**< Game context pointer */

	InputSnapshot input;				/**< Cached input snapshot */

	volatile bool logicTick;			/**< Logic update tick flag */
	volatile bool renderTick;			/**< Render update tick flag */

} AppCtx;

/** @} */

/** ##########################################################################
 *  @name Functions
 *  @brief Public API for App.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Initialize application subsystems.
 *
 * Initializes:
 * - display driver,
 * - input module,
 * - game context.
 *
 * @param app      Pointer to application context
 * @param game     Pointer to game context
 * @param hspi     SPI handle used by OLED
 * @param hadc     ADC handle used for input
 * @param btn1Port GPIO port for button 1
 * @param btn1Pin  GPIO pin for button 1
 * @param btn2Port GPIO port for button 2
 * @param btn2Pin  GPIO pin for button 2
 */
void App_Init(AppCtx *app, GameCtx *game, SPI_HandleTypeDef *hspi,
		ADC_HandleTypeDef *hadc, GPIO_TypeDef *btn1Port, uint16_t btn1Pin,
		GPIO_TypeDef *btn2Port, uint16_t btn2Pin);


/**
 * @brief Execute one iteration of application loop.
 *
 * Call continuously in main while(1).
 * Uses internal tick flags to pace logic and render updates.
 *
 * @param app Pointer to application context
 */
void App_RunOnce(AppCtx *app);


/**
 * @brief Application timing hook (tick).
 *
 * Intended to be called from timer ISR callback (e.g. TIM6).
 * Sets logicTick and renderTick flags.
 *
 * @param app Pointer to application context
 */
void App_OnTimerTick(AppCtx *app);

/** @} */

#endif /* INC_APP_APP_H_ */
