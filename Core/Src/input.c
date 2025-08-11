/*
 * input.c
 *
 *  Created on: Aug 10, 2025
 *      Author: Tomasz
 */

#include "input.h"


static ADC_HandleTypeDef* _hadc;
static GPIO_TypeDef* _btnPort;
static uint16_t _btnPin;

static GPIO_PinState _lastBtnState = GPIO_PIN_SET;
static GPIO_PinState _curentBtnState;


void InputInit(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btnPort, uint16_t btnPin){
	/*
	 * Assignment of input data
	 */
	_hadc = hadc;
	_btnPort = btnPort;
	_btnPin = btnPin;
}

InputSnapshot InputRead (void){
	/*
	 * Reading and returning current states
	 */
	InputSnapshot snap;
	snap.joystickYValue = HAL_ADC_GetValue(_hadc);

	// Removal of button repetition
	_curentBtnState = HAL_GPIO_ReadPin(_btnPort, _btnPin);

	if(_curentBtnState && !_lastBtnState){
		snap.btn1State = GPIO_PIN_SET;
	} else {
		snap.btn1State = GPIO_PIN_RESET;
	}
	_lastBtnState = _curentBtnState;

	return snap;
}


