/*
 * input.h
 *
 *  Created on: Aug 10, 2025
 *      Author: Tomasz
 */

#ifndef INC_INPUT_H_
#define INC_INPUT_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// Structure containing the current status of inputs from sanpa
typedef struct {
	uint32_t joystick_y_value;
	GPIO_PinState btn1_is_pressed;
}InputSnapshot;

InputSnapshot Input_Read (void);

#endif /* INC_INPUT_H_ */
