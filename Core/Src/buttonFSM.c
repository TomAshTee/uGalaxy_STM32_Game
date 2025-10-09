/**
 * @file    buttonFSM.c
 * @author  Tomasz Jaeschke
 * @date    2025-10-09
 * @brief   Implementation of button state machine with software debouncing.
 * @details
 * This module implements a non-blocking finite state machine (FSM)
 * to handle button input with debounce logic. It detects press and release
 * events and triggers registered callbacks.
 */

#include "main.h"
#include "buttonFSM.h"

/** ##########################################################################
 *  @name Initialization and Configuration
 *  @brief Basic setup and configuration functions.
 *  ##########################################################################
 */

/**
 * @brief Initializes the button FSM structure.
 */

void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce) {

	Key->State = IDLE;

	Key->GpioPort = GpioPort;
	Key->GpioPin = GpioPin;

	Key->TimerDebounce = TimerDebounce;

}

/**
 * @brief Sets debounce time (ms) for the button.
 */
void ButtonSetDebounceTime (TButton* Key, uint32_t DebounceTimeMS){
	Key->TimerDebounce = DebounceTimeMS;
}


/**
 * @brief Registers callback function for button press.
 */
void ButtonPressedRegisterCallback(TButton* Key, void *Callback){
	Key->ButtonPressed = Callback;
}

/**
 * @brief Registers callback function for button release.
 */
void ButtonReleaseRegisterCallback(TButton* Key, void *Callback){
	Key->ButtonRelease = Callback;
}


/** ##########################################################################
 *  @name Internal State Handlers
 *  @brief FSM routines for each button state.
 *  ##########################################################################
 */

static void ButtonIdleRutine(TButton* Key){
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)){

		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}
}

static void ButtonDebounceRutine(TButton* Key){
	if((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce){
		if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
			Key->State = PRESSED;
			Key->LastTick = HAL_GetTick();
			if(Key->ButtonPressed != NULL){
				Key->ButtonPressed();
			}
		} else {
			Key->State = IDLE;
		}
	}
}

static void ButtonPressedRutine(TButton* Key){
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin)) {
		Key->State = RELEASE;
	}
}


static void ButtonReleaseRutine(TButton* Key){
	if(Key->ButtonRelease != NULL){
		Key->ButtonRelease();
	}
	Key->State = IDLE;
}

/** ##########################################################################
 *  @name Public API
 *  @brief Main FSM function to be called periodically in main loop.
 *  ##########################################################################
 */

/**
 * @brief Executes the button state machine routine.
 * @param Key Pointer to TButton structure.
 */

void ButtonTask(TButton* Key){

	switch(Key->State){
	case IDLE:
		ButtonIdleRutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRutine(Key);
		break;

	case PRESSED:
		ButtonPressedRutine(Key);
		break;

	case RELEASE:
		ButtonReleaseRutine(Key);
		break;
	}
}
