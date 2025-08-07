/*
 * ssd1327.c
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#include "main.h"
#include "ssd1327.h"
#include <string.h>

// Definicja buforu
SPI_HandleTypeDef *ssd1337_spi;

static uint8_t buffer[BUF_SIZE];


void ssd1327_cmd (uint8_t cmd){

	HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ssd1337_spi, &cmd, 1, 10);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

}

void ssd1327_SetContrast(uint8_t Contrast)
{
	ssd1327_cmd(SSD1327_SETCONTRASTCURRENT);	// Set Contrast Control
	ssd1327_cmd(Contrast);
}

void ssd1327_Reset(void){

	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_SET);
	HAL_Delay(20);

}

void ssd1327_SpiInit(SPI_HandleTypeDef *spi){

	ssd1337_spi = spi;

	ssd1327_Reset();
	ssd1327_Init();

}

void ssd1327_Init (void){

//	ssd1327_cmd(SSD1327_DISPLAYOFF);  // Display Off
//
//	ssd1327_cmd(SSD1327_SETMULTIPLEX);
//	ssd1327_cmd(0x5F);
//
//	ssd1327_cmd(SSD1327_SETDISPLAYSTARTLINE);
//	ssd1327_cmd(0x00);
//
//	ssd1327_cmd(SSD1327_SETDISPLAYOFFSET);
//	ssd1327_cmd(0x20);
//
//	ssd1327_cmd(SSD1327_SEGREMAP);
//	ssd1327_cmd(0x51);
//
//	ssd1327_SetContrast(0x7F);
//
//	ssd1327_cmd(SSD1327_SETPHASELENGTH);
//	ssd1327_cmd(0x22);
//
//	ssd1327_cmd(SSD1327_SETFRONTCLOCKDIVIDER_OSCILLATORFREQUENCY);
//	ssd1327_cmd(0x50);
//
//	ssd1327_cmd(SSD1327_SELECTDEFAULTLINEARGRAYSCALETABLE);
//
//	ssd1327_cmd(SSD1327_SETPRECHARGEVOLTAGE);
//	ssd1327_cmd(0x10);
//
//	ssd1327_cmd(SSD1327_SETSETVCOMVOLTAGE);
//	ssd1327_cmd(0x05);
//
//	ssd1327_cmd(SSD1327_SETSECONDPRECHARGEPERTIOD);
//	ssd1327_cmd(0x0a);
//
//	ssd1327_cmd(SSD1327_FUNCTIONSELECTIONB);
//	ssd1327_cmd(0x62);
//
//	ssd1327_cmd(SSD1327_SETCOLUMNADDRESS);
//	ssd1327_cmd(0x00);
//	//ssd1327_cmd(0x3F);
//	ssd1327_cmd(0x7F);
//
//	ssd1327_cmd(SSD1327_SETROWADDRESS);
//	ssd1327_cmd(0x00);
//	//ssd1327_cmd(0x5F);
//	ssd1327_cmd(0x7F);
//
//	ssd1327_cmd(SSD1327_NORMALDISPLAY);  // Set Normal Display
//
//	ssd1327_cmd(SSD1327_DISPLAYALLON_RESUME);  // Entire Display ON

//--------------------------------------------------------------------------

	ssd1327_cmd(0xae);//--turn off oled panel

	ssd1327_cmd(0x15);  //set column addresses
	ssd1327_cmd(0x00);  //start column  0
	ssd1327_cmd(0x7f);  //end column  127

	ssd1327_cmd(0x75);  //set row addresses
	ssd1327_cmd(0x00);  //start row  0
	ssd1327_cmd(0x7f);  //end row  127

	ssd1327_cmd(0x81);  //set contrast control
	ssd1327_cmd(0x80);  //50% (128/255) def.0x80

	ssd1327_cmd(0xa0);   //gment remap
	ssd1327_cmd(0x51);  //51 (To my understanding, this is orientation

	ssd1327_cmd(0xa1);  //start line
	ssd1327_cmd(0x00);

	ssd1327_cmd(0xa2);  //display offset
	ssd1327_cmd(0x00);

	ssd1327_cmd(0xa4);  //rmal display
	ssd1327_cmd(0xa8);  //set multiplex ratio
	ssd1327_cmd(0x7f);

	ssd1327_cmd(0xb1);  //set phase leghth
	ssd1327_cmd(0xf1);

	ssd1327_cmd(0xb3);  //set dclk
	ssd1327_cmd(0x00);  //80Hz:0xc1 90Hz:0xe1  100Hz:0x00  110Hz:0x30 120Hz:0x50  130Hz:0x70   01

	ssd1327_cmd(0xab);  //Enable vReg
	ssd1327_cmd(0x01);

	ssd1327_cmd(0xb6);  //set phase leghth
	ssd1327_cmd(0x0f);

	ssd1327_cmd(0xbe); //Set vcomh voltage
	ssd1327_cmd(0x0f);

	ssd1327_cmd(0xbc); //set pre-charge voltage
	ssd1327_cmd(0x08);

	ssd1327_cmd(0xd5); //second precharge period
	ssd1327_cmd(0x62);

	ssd1327_cmd(0xfd); //Unlock commands
	ssd1327_cmd(0x12);

	ssd1327_cmd(0xA4);

	ssd1327_cmd(SSD1327_DISPLAYON);
}

void ssd1327_display (void){

	ssd1327_cmd(SSD1327_SETCOLUMNADDRESS);
	ssd1327_cmd(0x00);
	ssd1327_cmd(0x7F);

	ssd1327_cmd(SSD1327_SETROWADDRESS);
	ssd1327_cmd(0x00);
	ssd1327_cmd(0x7F);

	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ssd1337_spi, (uint8_t*)&buffer, BUF_SIZE, 100);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

}

void ssd1327_CLR(void){
		memset(buffer, (0 << 4 | 0), BUF_SIZE);
}

void ssd1327_setPixel( int x , int y , uint8_t bw){
	if ((x < 0) || (x >= SSD1327_WIDTH) || (y < 0) || (y >= SSD1327_HEIGHT))
		return;

	uint8_t SelectedCell = buffer[x/2 + y*(SSD1327_WIDTH/2)];

	if(x % 2)
	{
		SelectedCell &= ~(0x0F);
		SelectedCell |= (0x0F & bw);
	}
	else
	{
		SelectedCell &= ~(0xF0);
		SelectedCell |= (0xF0 & (bw<<4));
	}

	buffer[x/2 + y*(SSD1327_WIDTH/2)] = SelectedCell;



//	if ((x < 0) || (x >= SSD1327_WIDTH) || (y < 0) || (y >= SSD1327_HEIGHT))
//		return;
//	if (bw) buffer[x + (y/4)*SSD1327_WIDTH] |= (1<<(y%4));
//	else buffer[x + (y/4)*SSD1327_WIDTH] &= ~(1<<(y%4));
}

