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
	uint32_t joystickYValue;
	GPIO_PinState btn1State;
}InputSnapshot;

void InputInit(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btnPort, uint16_t btnPin);
InputSnapshot InputRead (void);

#endif /* INC_INPUT_H_ */
