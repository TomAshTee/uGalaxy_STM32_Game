/**
 * @file    ssd1327.h
 * @author  Tomasz Jaeschke
 * @date    2026-01-13
 * @brief   Low-level driver for SSD1327 OLED display.
 * @details
 * This module provides low-level control functions for the SSD1327 OLED display,
 * including initialization, command and data transmission, contrast control,
 * and direct pixel manipulation.
 *
 * It is intended to be used together with higher-level graphics functions from
 * `GFX_ssd1327.h` for rendering shapes, text, and bitmaps.
 *
 * Features:
 * - Initialization over SPI
 * - Command set definitions
 * - Pixel buffer control
 * - Contrast and display settings
 */

#ifndef INC_SSD1327_H_
#define INC_SSD1327_H_

#include "stm32l4xx_hal.h"
#include <stdbool.h>

/** ##########################################################################
 *  @name Using DMA for SIP transmission
 *  @brief Changes the definition of the SSD1327_Display () function
 *  @note Remember to support TXCpltCallback -> Change CS pin
 *  @{
 *  ##########################################################################
 */

#define SSD1327_USE_DMA

/** @} */

/** ##########################################################################
 *  @name Pin configuration
 *  @brief GPIO pins used to control SSD1327.
 *  @{
 *  ##########################################################################
 */

#define RST 		RST_Pin			/**<  RST */
#define DC 			DC_Pin			/**<  D/C */
#define CS 			CS_Pin			/**<  CS */

#define RST_PORT	RST_GPIO_Port
#define DC_PORT		DC_GPIO_Port
#define CS_PORT		CS_GPIO_Port

/** @} */

/** ##########################################################################
 *  @name Display parameters
 *  @brief Resolution and buffer size.
 *  @{
 *  ##########################################################################
 */

#define SSD1327_WIDTH	128
#define SSD1327_HEIGHT 	128

#define BUF_SIZE 		(SSD1327_WIDTH*SSD1327_HEIGHT/2)

#define BLACK 			0
#define WHITE 			15

/** @} */

/** ##########################################################################
 *  @name Command definitions
 *  @brief SSD1327 instruction set.
 *  @{
 *  ##########################################################################
 */

#define SSD1327_SETCOLUMNADDRESS	0x15
#define SSD1327_SETROWADDRESS		0x75
#define SSD1327_SETCONTRASTCURRENT 	0x81
#define SSD1327_NOP					0x84
#define SSD1327_SEGREMAP			0xA0
#define SSD1327_SETDISPLAYSTARTLINE	0xA1
#define SSD1327_SETDISPLAYOFFSET	0xA2
#define SSD1327_DISPLAYALLON_RESUME 0xA4
#define SSD1327_DISPLAYALLON 		0xA5
#define SSD1327_NORMALDISPLAY 		0xA6
#define SSD1327_INVERTDISPLAY 		0xA7
#define SSD1327_SETMULTIPLEX		0xA8
#define SSD1327_FUNCTIONSELECTIONA	0xAB
#define SSD1327_DISPLAYOFF			0xAE
#define SSD1327_DISPLAYON			0xAF
#define SSD1327_SETPHASELENGTH								0xB1
#define SSD1327_SETFRONTCLOCKDIVIDER_OSCILLATORFREQUENCY 	0xB3
#define SSD1327_SETGPIO				0xB5
#define SSD1327_SETSECONDPRECHARGEPERTIOD	0xB6
#define SSD1327_SETGRAYSCALETABLE	0xB8
#define SSD1327_SELECTDEFAULTLINEARGRAYSCALETABLE	0xB9
#define SSD1327_SETPRECHARGEVOLTAGE	0xBC
#define SSD1327_SETSETVCOMVOLTAGE	0xBE
#define SSD1327_FUNCTIONSELECTIONB	0xD5
#define SSD1327_SETCOMMANDLOCK		0xFD

/** @} */

/** ##########################################################################
 *  @name Variables
 *  @brief External variables used by SSD1327 driver.
 *  @{
 *  ##########################################################################
 */

extern const uint8_t font[];
extern int cursor_x, cursor_y;

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE ;

/** @} */

/** ##########################################################################
 *  @name Functions
 *  @brief Public API for SSD1327 control.
 *  @{
 *  ##########################################################################
 */

void SSD1327_SpiInit(SPI_HandleTypeDef *spi);					/**< Initialisation via SPI */
void SSD1327_Display (void);									/**< Sending data from the buffer */
void SSD1327_SetPixel( int x , int y , uint8_t bw); 			/**< Pixel setting */
void SSD1327_BeginFrame(void);   								/**< Cleans back buffer (for drawing) */
bool SSD1327_Present(void);      								/**< If SPI DMA free: changes buffers and starts SPI DMA; returns true when started*/
bool SSD1327_IsBusy(void);       								/**< Status SPI DMA */

/** @} */

#endif /* INC_SSD1327_H_ */
