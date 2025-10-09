/**
 * @file    GFX_ssd1327.h
 * @author  Tomasz Jaeschke
 * @date    2022-07-05
 * @brief   Graphics primitives for SSD1327 OLED display.
 * @details
 * This module provides basic drawing functions for the SSD1327 OLED display,
 * including bitmaps, shapes, lines, circles, rectangles, and text rendering.
 * It works with monochrome 4-bit grayscale mode supported by SSD1327.
 *
 * Functions here operate on the display buffer and require a separate update
 * call to send changes to the screen.
 */

#ifndef INC_GFX_SSD1327_H_
#define INC_GFX_SSD1327_H_


/** ##########################################################################
 *  @name Display parameters
 *  @brief SSD1327 resolution in pixels.
 *  @{
 *  ##########################################################################
 */

#define SSD1327_WIDTH			128
#define SSD1327_HEIGHT 			128

/** @} */

/** ##########################################################################
 *  @name Font data
 *  @brief External font table used by GFX text rendering.
 *  @{
 *  ##########################################################################
 */

extern const uint8_t  font[];

/** @} */

/** ##########################################################################
 *  @name Drawing functions
 *  @brief Graphics functions for bitmaps, shapes, and text.
 *  @{
 *  ##########################################################################
 */

void GFX_DrawBitMap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);	/**< Drawing a bitmap */

void GFX_DrawCircle(int CX, int CY ,int R,int I_O);													/**< Drawing a circle  */
void GFX_FillCircle(int x0, int y0, int R, int I_O);												/**< Drawing a circle with a given radius  */
void GFX_FillCircleHelper(int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color ); 	/**< Draws part of a circle and fills it in */
void GFX_DrawCircleHelper(int x0, int y0, int r, uint8_t cornername, uint8_t color );				/**< Draws part of a circle */

void GFX_DrawLine(int X1, int Y1,int X2,int Y2,int I_O);											/**< Line drawing */
void GFX_DrawFastVLine(int x, int y, int h, uint8_t color);											/**< Quick vertical line drawing */
void GFX_DrawFastHLine(int x, int y, int h, uint8_t color);											/**< Quick drawing of a horizontal line */

void GFX_FillRect (int x, int y, int w, int h, uint8_t color ); 									/**< Drawing a rectangle with filling */
void GFX_FillRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);						/**< Drawing a rounded rectangle with filling */
void GFX_DrawRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);						/**< Drawing a rounded rectangle */

void GFX_DrawChar(int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size); 					/**< Drawing a Char */
void GFX_PutString(int x, int y, char* str, uint8_t txt_size, uint8_t color, uint8_t bg); 			/**< Drawing a String */
void GFX_PutInt (int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg); 				/**< Drawing a Int */

/** @} */

#endif /* INC_GFX_SSD1327_H_ */
