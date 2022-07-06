/*
 * GFX_ssd1327.h
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#ifndef INC_GFX_SSD1327_H_
#define INC_GFX_SSD1327_H_


//#####################################################
//				Parametry wyświetlacza
//#####################################################

#define SSD1327_WIDTH			128
#define SSD1327_HEIGHT 			128


const uint8_t  font[];

void GFX_DrowBitMap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);//rysoanie bitmapy

void GFX_DrowCircle(int CX, int CY ,int R,int I_O);			//rysowanie okr�gu
void GFX_FillCircle(int x0, int y0, int R, int I_O);		//rysowanie ko�o o zadanym promieniu
void GFX_FillCircleHelper(int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color ); //p�kole ?
void GFX_DrowCircleHelper(int x0, int y0, int r, uint8_t cornername, uint8_t color );	//rysuje �wiartke okregu

void GFX_DrowLine(int X1, int Y1,int X2,int Y2,int I_O);	//rysowanie lini
void GFX_DrowFastVLine(int x, int y, int h, uint8_t color);		//rysowanie szybkiej poziomej lini
void GFX_DrowFastHLine(int x, int y, int h, uint8_t color);		//rysowanie szybkiej pionowej lini

void GFX_FillRect (int x, int y, int w, int h, uint8_t color ); //rysowanie kwadratu z wype�nieniem
void GFX_FillRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);
void GFX_DrowRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color);

void GFX_DrowChar(int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size); //rysowanie znaku
void GFX_PutString(int x, int y, char* str, uint8_t txt_size, uint8_t color, uint8_t bg); // rysuje ca�y string
void GFX_PutInt (int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg); //rysuje dan liczbe

#endif /* INC_GFX_SSD1327_H_ */
