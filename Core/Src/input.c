/*
 * input.c
 *
 *  Created on: Aug 10, 2025
 *      Author: Tomasz
 */

#include "input.h"


static ADC_HandleTypeDef* s_hadc;
static GPIO_TypeDef* s_btn_Port;
static uint16_t s_btn_Pin;

static GPIO_PinState last_Btn_State = GPIO_PIN_SET;
static GPIO_PinState curent_Btn_State;


void Input_Init(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btn_Port, uint16_t btn_Pin){
	/*
	 * Assignment of input data
	 */
	s_hadc = hadc;
	s_btn_Port = btn_Port;
	s_btn_Pin = btn_Pin;
}

InputSnapshot Input_Read (void){
	/*
	 * Reading and returning current states
	 */
	InputSnapshot snap;
	snap.joystick_y_value = HAL_ADC_GetValue(s_hadc);

	// Removal of button repetition
	curent_Btn_State = HAL_GPIO_ReadPin(s_btn_Port, s_btn_Pin);

	if(curent_Btn_State && !last_Btn_State){
		snap.btn1_is_pressed = GPIO_PIN_SET;
	} else {
		snap.btn1_is_pressed = GPIO_PIN_RESET;
	}
	last_Btn_State = curent_Btn_State;

	return snap;
}


