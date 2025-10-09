/*
 * input.c
 *
 *  Created on: Aug 10, 2025
 *      Author: Tomasz
 */

#include "input.h"
#include "buttonFSM.h"


static ADC_HandleTypeDef* _hadc;

static bool _btn1PressedEdge = false;

static TButton _btn1;

static void onBtn1Pressed(void) {
	_btn1PressedEdge = true;
}

/*
 * @brief Assignment of input data
 *
 */
void InputInit(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btn1Port, uint16_t btn1Pin){

	_hadc = hadc;

	ButtonInitKey(&_btn1, btn1Port, btn1Pin, 20);
	ButtonPressedRegisterCallback(&_btn1, onBtn1Pressed);
}

/*
 * @brief Reading and returning current states
 */
InputSnapshot InputRead (void){

	InputSnapshot snap;
	snap.joystickYValue = HAL_ADC_GetValue(_hadc);

	ButtonTask(&_btn1);

	// Removal of repetition
	if(_btn1PressedEdge){
		snap.btn1State = GPIO_PIN_SET;
		_btn1PressedEdge = false;
	} else {
		snap.btn1State = GPIO_PIN_RESET;
	}

	return snap;
}


