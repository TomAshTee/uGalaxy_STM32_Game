/*
 * ssd1327.h
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#ifndef INC_SSD1327_H_
#define INC_SSD1327_H_

#include "stm32l4xx_hal.h"


//#####################################################
//				Pin configuration
//#####################################################


#define RST 	RST_Pin			// RST
#define DC 		DC_Pin			// D/C
#define CS 		CS_Pin			// CS

#define RST_PORT	RST_GPIO_Port
#define DC_PORT		DC_GPIO_Port
#define CS_PORT		CS_GPIO_Port

//#####################################################
//				Display parameters
//#####################################################

#define SSD1327_WIDTH			128
#define SSD1327_HEIGHT 			128

#define BUF_SIZE (SSD1327_WIDTH*SSD1327_HEIGHT/2)

#define BLACK 0
// Grays between
#define WHITE 15

//#####################################################
//				Command definitions
//#####################################################


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

//#####################################################
//				Variable declarations
//#####################################################

//uint8_t ssd1327_buf[BUF_SIZE] ;
extern const uint8_t font[];
extern int cursor_x, cursor_y;

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE ;

//#####################################################
//				Function declarations
//#####################################################

void ssd1327_Init (void); 										//Initialising the display
void ssd1327_SpiInit(SPI_HandleTypeDef *spi);					//Initialisation via SPI
void ssd1327_Reset(void);										//Screen reset
void ssd1327_cmd (uint8_t cmd); 								//Sending commands
void ssd1327_SetContrast(uint8_t Contrast);						//Contrast setting
void ssd1327_data ( uint8_t dat );								//Sending data
void ssd1327_display (void);									//Sending data from the buffer
void ssd1327_setPixel( int x , int y , uint8_t bw); 			//Pixel setting
void ssd1327_CLR(void);											//Cleaning the screen




#endif /* INC_SSD1327_H_ */
