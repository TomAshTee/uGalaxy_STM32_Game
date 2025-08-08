/*
 * GFX_ssd1327.h
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#ifndef INC_GFX_SSD1327_H_
#define INC_GFX_SSD1327_H_


//#####################################################
//				Display parameters
//#####################################################

#define SSD1327_WIDTH			128
#define SSD1327_HEIGHT 			128

//#####################################################
//				Board with font
//#####################################################

extern const uint8_t  font[];

//#####################################################
//				Function declarations
//#####################################################

void GFX_DrowBitMap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);	// Drawing a bitmap

void GFX_DrowCircle(int CX, int CY ,int R,int I_O);													// Drawing a circle
void GFX_FillCircle(int x0, int y0, int R, int I_O);												// Drawing a circle with a given radius
void GFX_FillCircleHelper(int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color ); 	// Draws part of a circle and fills it in
void GFX_DrowCircleHelper(int x0, int y0, int r, uint8_t cornername, uint8_t color );				// Draws part of a circle

void GFX_DrowLine(int X1, int Y1,int X2,int Y2,int I_O);											// Line drawing
void GFX_DrowFastVLine(int x, int y, int h, uint8_t color);											// Quick vertical line drawing
void GFX_DrowFastHLine(int x, int y, int h, uint8_t color);											// Quick drawing of a horizontal line

void GFX_FillRect (int x, int y, int w, int h, uint8_t color ); 									// Drawing a rectangle with filling
void GFX_FillRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);						// Drawing a rounded rectangle with filling
void GFX_DrowRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);						// Drawing a rounded rectangle

void GFX_DrowChar(int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size); 					// Drawing a Char
void GFX_PutString(int x, int y, char* str, uint8_t txt_size, uint8_t color, uint8_t bg); 			// Drawing a String
void GFX_PutInt (int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg); 				// Drawing a Int

#endif /* INC_GFX_SSD1327_H_ */
