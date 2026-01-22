/**
 * @file    input.h
 * @author  Tomasz Jaeschke
 * @date    2025-08-10
 * @brief   Input handling interface for uGalaxy STM32 Game.
 * @details
 * This module provides an abstraction layer for reading user input
 * from joystick and button(s) on the STM32 hardware.
 *
 * Features:
 * - Joystick ADC reading
 * - Button GPIO reading
 * - Debouncing and input snapshot structure
 *
 * The API returns an `InputSnapshot` struct containing the current
 * state of all inputs, which is used by the game logic.
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/** ##########################################################################
 *  @name Data Structures
 *  @brief Input state containers.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Snapshot of current input states.
 *
 * Holds the latest read values from joystick and buttons.
 */
typedef struct {
	uint32_t joystickYValue;	/**< Raw Y-axis ADC value from joystick. */
	GPIO_PinState btn1State;	/**< State of button 1 (pressed/released). */
	GPIO_PinState btn2State;	/**< State of button 2 (pressed/released). */
}InputSnapshot;

/** @} */

/** ##########################################################################
 *  @name Functions
 *  @brief Public API for input initialization and reading.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Initialize input system.
 * @param hadc     Pointer to ADC handle for joystick input.
 * @param btnPort  GPIO port for the button.
 * @param btnPin   GPIO pin number for the button.
 */
void InputInit(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btn1Port, uint16_t btn1Pin, GPIO_TypeDef* btn2Port, uint16_t btn2Pin);

/**
 * @brief Read current input states.
 * @return An `InputSnapshot` struct with current joystick and button values.
 */
InputSnapshot InputRead (void);

/** @} */

#endif /* INC_INPUT_H_ */
