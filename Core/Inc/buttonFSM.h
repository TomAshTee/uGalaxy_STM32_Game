/**
 * @file    buttonFSM.h
 * @author  Tomasz Jaeschke
 * @date    2025-10-09
 * @brief   Finite State Machine (FSM) for button input handling.
 * @details
 * This module provides a non-blocking button handling system with
 * software debouncing. It allows registration of callback functions
 * for press and release events and can be used in main loop polling.
 *
 * Features:
 * - Software debouncing with configurable delay
 * - Non-blocking state machine architecture
 * - Callback support for button press and release
 */

#ifndef INC_BUTTONFSM_H_
#define INC_BUTTONFSM_H_

/** ##########################################################################
 *  @name Enumerations
 *  @brief Button state machine states.
 *  @{
 *  ##########################################################################
 */

typedef enum {
	IDLE = 0,		/**< Button released, waiting for press */
	DEBOUNCE,		/**< Debounce delay in progress */
	PRESSED,		/**< Button is pressed */
	RELEASE			/**< Button released, trigger callback */
}BUTTON_STATE;

/** @} */

/** ##########################################################################
 *  @name Structures
 *  @brief Button configuration and runtime data.
 *  @{
 *  ##########################################################################
 */

typedef struct {
	BUTTON_STATE	State;			/**< Current state of the button FSM */

	GPIO_TypeDef*	GpioPort;		/**< Pointer to GPIO port */
	uint16_t		GpioPin;		/**< GPIO pin number */

	uint32_t		LastTick;		/**< Last tick for debounce timing */
	uint32_t		TimerDebounce;	/**< Debounce delay in milliseconds */

	void(*ButtonPressed)(void);		/**< Callback on button press */
	void(*ButtonRelease)(void);		 /**< Callback on button release */
}TButton;

/** @} */

/** ##########################################################################
 *  @name Public API
 *  @brief Functions for initializing and handling button state.
 *  @{
 *  ##########################################################################
 */

/**
 * @brief Initializes button parameters.
 * @param Key Pointer to TButton structure.
 * @param GpioPort GPIO port pointer.
 * @param GpioPin GPIO pin number.
 * @param TimerDebounce Debounce delay time in milliseconds.
 */
void ButtonInitKey(TButton* Key, GPIO_TypeDef* GpioPort, uint16_t GpioPin, uint32_t TimerDebounce);

/**
 * @brief Registers callback for button press event.
 * @param Key Pointer to TButton structure.
 * @param Callback Function pointer to press event.
 */
void ButtonPressedRegisterCallback(TButton* Key, void *Callback);

/**
 * @brief Registers callback for button release event.
 * @param Key Pointer to TButton structure.
 * @param Callback Function pointer to release event.
 */
void ButtonReleaseRegisterCallback(TButton* Key, void *Callback);

/**
 * @brief Sets the debounce time for the button.
 * @param Key Pointer to TButton structure.
 * @param DebounceTimeMS Debounce time in milliseconds.
 */
void ButtonSetDebounceTime (TButton* Key, uint32_t DebounceTimeMS);

/**
 * @brief Main button task to be called in the loop.
 * @param Key Pointer to TButton structure.
 */
void ButtonTask(TButton* Key);

/** @} */

#endif /* INC_BUTTONFSM_H_ */
