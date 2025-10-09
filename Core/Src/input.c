/**
 * @file    input.c
 * @author  Tomasz
 * @date    2025-08-10
 * @brief   Handles joystick and button inputs for the game.
 * @details
 * Reads user input from:
 * - Joystick (Y-axis) via ADC
 * - Button 1 using a debounced FSM
 *
 * Returns an `InputSnapshot` containing:
 * - Raw joystick ADC value
 * - One-shot button press event
 */

#include "input.h"
#include "buttonFSM.h"

/** ##########################################################################
 *  @name Private Variables
 *  @brief Internal module state.
 *  @{
 *  ##########################################################################
 */

static ADC_HandleTypeDef* _hadc;		/** ADC handle used to read the joystick analog value. */
static bool _btn1PressedEdge = false;	/** Flag set on a rising-edge press event of BTN1 (via FSM callback). */
static TButton _btn1;					/** FSM instance for Button 1. */

/** @} */

/** ##########################################################################
 *  @name Private Helpers
 *  @brief Static callbacks and utilities.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Callback invoked by the button FSM on press event.
 * @details
 * Sets a one-shot edge flag consumed in @ref InputRead().
 */
static void onBtn1Pressed(void) {
	_btn1PressedEdge = true;
}

/** @} */

/** ##########################################################################
 *  @name Public API
 *  @brief Initialization and snapshot readout.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Initialize input subsystem and button FSM.
 * @param hadc      Pointer to the ADC handle for joystick input (Y-axis).
 * @param btn1Port  GPIO port of Button 1.
 * @param btn1Pin   GPIO pin number of Button 1.
 * @details
 * - Stores hardware handles for later readout.
 * - Initializes button FSM with a 20 ms debounce time.
 * - Registers @ref onBtn1Pressed() as press-event callback.
 */
void InputInit(ADC_HandleTypeDef* hadc, GPIO_TypeDef* btn1Port, uint16_t btn1Pin){

	_hadc = hadc;

	ButtonInitKey(&_btn1, btn1Port, btn1Pin, 20);
	ButtonPressedRegisterCallback(&_btn1, onBtn1Pressed);
}

/**
 * @brief Read current input states and return a snapshot.
 * @return `InputSnapshot` with:
 * - `joystickYValue` – raw ADC reading of joystick Y-axis,
 * - `btn1State`      – GPIO_PIN_SET only on press edge (one-shot), otherwise RESET.
 * @details
 * - Invokes ButtonTask() to advance FSM and process debouncing.
 * - Converts the press callback into a one-shot flag consumed here, which
 *   prevents key repetition in tick-based loops.
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

/** @} */
