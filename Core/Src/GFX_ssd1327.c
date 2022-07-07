/*
 * GFX_ssd1327.c
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "itoa.h"
#include "GFX_ssd1327.h"

int cursor_x, cursor_y;

void GFX_DrowCircle(int CX, int CY ,int R,int I_O) {
	   int X, Y, Xchange, YChange, RadiusError;

	   X = R;
	   Y = 0;
	   Xchange = 1 - 2*R;
	   YChange = 1;
	   RadiusError = 0;
	   while( X >= Y ) {
		   ssd1327_setPixel(CX+X, CY+Y,I_O);
		   ssd1327_setPixel(CX-X, CY+Y,I_O);
		   ssd1327_setPixel(CX-X, CY-Y,I_O);
		   ssd1327_setPixel(CX+X, CY-Y,I_O);
		   ssd1327_setPixel(CX+Y, CY+X,I_O);
		   ssd1327_setPixel(CX-Y, CY+X,I_O);
		   ssd1327_setPixel(CX-Y, CY-X,I_O);
		   ssd1327_setPixel(CX+Y, CY-X,I_O);

	        Y++;
	        RadiusError += YChange;
	        YChange+=2;
	        if ( 2*RadiusError + Xchange > 0 ) {
	              X--;
	              RadiusError+=Xchange;
	              Xchange+=2;
	        }
	   }
}

//Bresenham's algorithm - Wikipedia
void GFX_DrowLine(int X1, int Y1,int X2,int Y2,int I_O) {
	int CurrentX, CurrentY, Xinc, Yinc,
		Dx, Dy, TwoDx, TwoDy,
		TwoDxAccumulatedError, TwoDyAccumulatedError;

	Dx = (X2-X1);
	Dy = (Y2-Y1);

	TwoDx = Dx + Dx;
	TwoDy = Dy + Dy;

	CurrentX = X1;
	CurrentY = Y1;

	Xinc = 1;
	Yinc = 1;

	if(Dx < 0) {

		Xinc = -1;
		Dx = -Dx;
		TwoDx = -TwoDx;
	}

	if (Dy < 0) {
		Yinc = -1;
		Dy = -Dy;
		TwoDy = -TwoDy;
	}

	ssd1327_setPixel(X1,Y1,I_O);

	if ((Dx != 0) || (Dy != 0)) {

		if (Dy <= Dx) {
			TwoDxAccumulatedError = 0;
			do {
			    CurrentX += Xinc;
			    TwoDxAccumulatedError += TwoDy;
			    if(TwoDxAccumulatedError > Dx) {
			        CurrentY += Yinc;
			        TwoDxAccumulatedError -= TwoDx;
			    }
			    ssd1327_setPixel(CurrentX,CurrentY,I_O);
			  } while (CurrentX != X2);
		  } else {
			  TwoDyAccumulatedError = 0;
			  do {
			      CurrentY += Yinc;
			      TwoDyAccumulatedError += TwoDx;
			      if(TwoDyAccumulatedError>Dy) {
				      CurrentX += Xinc;
				      TwoDyAccumulatedError -= TwoDy;
			      }
			      ssd1327_setPixel(CurrentX,CurrentY,I_O);
			  } while (CurrentY != Y2);
		  }
	}
}

void GFX_FillCircleHelper(int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color )
{
	//delata- odlegoc miedzy ciwartami po��wki

	int f 		= 1 - r;
	int ddF_x 	= 1;
	int ddF_y	= -2 * r;
	int x		= 0;
	int y		= r;

	while(x<y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if(cornername & 0x1)
		{
			GFX_DrowFastVLine(x0+x, y0-y, 2*y+1+delta, color);
			GFX_DrowFastVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if(cornername & 0x2)
		{
			GFX_DrowFastVLine(x0-x, y0-y, 2*y+1+delta, color);
			GFX_DrowFastVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

void GFX_DrowCircleHelper(int x0, int y0, int r, uint8_t cornername, uint8_t color )
{
	int f 		= 1 - r;
	int ddF_x 	= 1;
	int ddF_y	= -2 * r;
	int x		= 0;
	int y		= r;

	while(x<y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if(cornername & 0x4)
		{
			ssd1327_setPixel(x0 + x, y0 + y, color);
			ssd1327_setPixel(x0 + y, y0 + x, color);
		}
		if(cornername & 0x2)
		{
			ssd1327_setPixel(x0 + x, y0 - y, color);
			ssd1327_setPixel(x0 + y, y0 - x, color);
		}
		if(cornername & 0x8)
		{
			ssd1327_setPixel(x0 - x, y0 + y, color);
			ssd1327_setPixel(x0 - y, y0 + x, color);
		}
		if(cornername & 0x1)
		{
			ssd1327_setPixel(x0 - x, y0 - y, color);
			ssd1327_setPixel(x0 - y, y0 - x, color);
		}
	}
}

void GFX_FillCircle(int x0, int y0, int R, int I_O)
{

int f = 1 - R;
int ddF_x = 1;
int ddF_y = -2 * R;
int x = 0;
int y = R;

	/*
	 * Fill in the center between the two halves
	 */
	GFX_DrowLine(x0, y0-R, x0, y0+R,I_O);

	while(x < y)
	{
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		/*
		 * Now draw vertical lines between the points on the circle rather than
		 * draw the points of the circle. This draws lines between the
		 * perimeter points on the upper and lower quadrants of the 2 halves of the circle.
		 */

		GFX_DrowLine(x0+x, y0+y, x0+x, y0-y,I_O);
		GFX_DrowLine(x0-x, y0+y, x0-x, y0-y,I_O);
		GFX_DrowLine(x0+y, y0+x, y+x0, y0-x,I_O);
		GFX_DrowLine(x0-y, y0+x, x0-y, y0-x,I_O);
  	}
}

void GFX_DrowBitMap_P (int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{
//	int i, j, byteWidth = (w + 7) / 8 ;
//
//	for(j=0; j<h;j++)
//	{
//		for(i=0; i<w;i++)
//		{
//			if(*(bitmap + (j * byteWidth) + (i /8)) &(128 >> (i & 7))){
//				ssd1327_setPixel(x+i, y+j, color);
//			}
//		}
//	}




//	uint8_t i, j;
//	uint8_t SelectedCell;
//
//	for(j = 0; j < h; j++)
//	{
//		for(i = 0; i < w; i++)
//		{
//			SelectedCell = bitmap[i/2 + j*(w/2)];
//
//			 if(i % 2)
//			 {
//				 SelectedCell &= 0x0F;
//			 }
//			 else
//			 {
//				 SelectedCell >>= 4;
//			 }
//
//			ssd1327_setPixel(x+i, y+j, );
//		}
//	}

	uint8_t i, j, byteWidth = (w+7)/8;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			if(*(bitmap + j *byteWidth + i /8) & (128 >> (i&7)) )
				ssd1327_setPixel(x+i, y+j, color);
		}
	}

}

void GFX_DrowChar(int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size)
{

	  uint8_t line;
	  int8_t i,j;
	  if((x >= SSD1327_WIDTH)            || // Clip right
	     (y >= SSD1327_HEIGHT)           || // Clip bottom
	     ((x + 6 * size - 1) < 0) || // Clip left
	     ((y + 8 * size - 1) < 0))   // Clip top
	    return;

	  for  (i=0; i<6; i++ ) {
	    if (i == 5)
	      line = 0x0;
	    else
	      //line = pgm_read_byte(font+(c*5)+i);
	    	line = (font[(c*5)+i]);
	    for (j = 0; j<8; j++) {
	      if (line & 0x1) {
	        if (size == 1) // default size
	        	ssd1327_setPixel(x+i, y+j, color);
	        else {  // big size
	        	GFX_FillRect(x+(i*size), y+(j*size), size, size, color);
	        }
	      } else if (bg != color) {
	        if (size == 1) // default size
	        	ssd1327_setPixel(x+i, y+j, bg);
	        else {  // big size
	        	GFX_FillRect(x+i*size, y+j*size, size, size, bg);
	        }
	      }
	      line >>= 1;
	    }
	  }
}

void GFX_FillRect (int x, int y, int w, int h, uint8_t color )
{
	int16_t i;
	for(i=x; i <x+w; i++)
	{
		GFX_DrowFastVLine(i, y, h, color);
	}
}

void GFX_DrowFastVLine(int x, int y, int h, uint8_t color)
{
	GFX_DrowLine(x, y, x, y+h-1, color);
}

void GFX_DrowFastHLine(int x, int y, int w, uint8_t color)
{
	GFX_DrowLine(x, y, x+w-1 ,y , color);
}

void GFX_PutString(int x, int y, char* str, uint8_t txt_size, uint8_t color, uint8_t bg)
{
	cursor_x = x;
	cursor_y = y;

	while(*str)		//do kiedy jest cos w stringu
	{
		GFX_DrowChar(cursor_x,cursor_y, *str++, color, bg, txt_size);
		cursor_x += txt_size*6; // tylko pod dany font trzeba potem to korygowa�
	}
}

void GFX_PutInt (int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg)
{
	char buf[16];
	GFX_PutString(x, y, itoa(data, buf,10), txt_size, color, bg);
	//GFX_PutString(x, y, dtoa((double)data,1,1,1,1,buf), txt_size, color, bg);
	//itoa(data,buf);
	//GFX_PutString(x, y, buf , txt_size, color, bg);
}

void GFX_DrowRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color)
{
	// r - to jest wielkosc zaokraglonego naro�nika
	GFX_DrowFastHLine(x+r	,y		,w-2*r	,color );
	GFX_DrowFastHLine(x+r	,y+h-1	,w-2*r	,color );
	GFX_DrowFastVLine(x		,y+r	,h-2*r	,color );
	GFX_DrowFastVLine(x+w-1	,y+r	,h-2*r	,color );

	GFX_DrowCircleHelper(x+r	,y+r	,r	,1	,color);
	GFX_DrowCircleHelper(x+w-r-1,y+r	,r	,2	,color);
	GFX_DrowCircleHelper(x+w-r-1,y+h-r-1,r	,4	,color);
	GFX_DrowCircleHelper(x+r	,y+h-r-1,r	,8	,color);
}

void GFX_FillRoundRect(int x, int y, int w, int h, uint8_t r, uint8_t color)
{
	GFX_FillRect(x+r, y, w-2*r, h, color);

	GFX_FillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	GFX_FillCircleHelper(x+r, y+r, r, 2, h-2*r-1, color);
}

const uint8_t  font[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x22, 0x54, 0x54, 0x78, 0x42, // a-umlaut
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0x7D, 0x12, 0x11, 0x12, 0x7D, // A-umlaut
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x3A, 0x44, 0x44, 0x44, 0x3A, // o-umlaut
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x3D, 0x42, 0x42, 0x42, 0x3D, // O-umlaut
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0xAA, 0x00, 0x55, 0x00, 0xAA,
	0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0xFC, 0x4A, 0x4A, 0x4A, 0x34, // sharp-s or beta
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00
};
