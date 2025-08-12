/*
 * ssd1327.c
 *
 *  Created on: Jul 5, 2022
 *      Author: Tomasz Jaeschke
 */

#include "main.h"
#include "ssd1327.h"
#include <string.h>

// Definition of buffer
SPI_HandleTypeDef *ssd1337_spi;

static uint8_t buffer[BUF_SIZE];


void SSD1327_CMD (uint8_t cmd){

	HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ssd1337_spi, &cmd, 1, 10);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

}

void SSD1327_SetContrast(uint8_t Contrast)
{
	SSD1327_CMD(SSD1327_SETCONTRASTCURRENT);	// Set Contrast Control
	SSD1327_CMD(Contrast);
}

void SSD1327_Reset(void){

	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_SET);
	HAL_Delay(20);

}

void SSD1327_SpiInit(SPI_HandleTypeDef *spi){

	ssd1337_spi = spi;

	SSD1327_Reset();
	SSD1327_Init();

}

void SSD1327_Init (void){

	SSD1327_CMD(0xae);	// Turn off oled panel

	SSD1327_CMD(0x15);  // Set column addresses
	SSD1327_CMD(0x00);  // Start column  0
	SSD1327_CMD(0x7f);  // End column  127

	SSD1327_CMD(0x75);  // Set row addresses
	SSD1327_CMD(0x00);  // Start row  0
	SSD1327_CMD(0x7f);  // End row  127

	SSD1327_CMD(0x81);  // Set contrast control
	SSD1327_CMD(0x80);  // 50% (128/255) def.0x80

	SSD1327_CMD(0xa0);  // Gment remap
	SSD1327_CMD(0x51);  // 51 (To my understanding, this is orientation

	SSD1327_CMD(0xa1);  // Start line
	SSD1327_CMD(0x00);

	SSD1327_CMD(0xa2);  // Display offset
	SSD1327_CMD(0x00);

	SSD1327_CMD(0xa4);  // rmal display
	SSD1327_CMD(0xa8);  // Set multiplex ratio
	SSD1327_CMD(0x7f);

	SSD1327_CMD(0xb1);  // Set phase leghth
	SSD1327_CMD(0xf1);

	SSD1327_CMD(0xb3);  // Set dclk
	SSD1327_CMD(0x00);  // 80Hz:0xc1 90Hz:0xe1  100Hz:0x00  110Hz:0x30 120Hz:0x50  130Hz:0x70   01

	SSD1327_CMD(0xab);  // Enable vReg
	SSD1327_CMD(0x01);

	SSD1327_CMD(0xb6);  // Set phase leghth
	SSD1327_CMD(0x0f);

	SSD1327_CMD(0xbe); 	// Set vcomh voltage
	SSD1327_CMD(0x0f);

	SSD1327_CMD(0xbc); 	// Set pre-charge voltage
	SSD1327_CMD(0x08);

	SSD1327_CMD(0xd5); 	// Second precharge period
	SSD1327_CMD(0x62);

	SSD1327_CMD(0xfd); 	// Unlock commands
	SSD1327_CMD(0x12);

	SSD1327_CMD(0xA4);

	SSD1327_CMD(SSD1327_DISPLAYON);
}

void SSD1327_Display (void){

	SSD1327_CMD(SSD1327_SETCOLUMNADDRESS);
	SSD1327_CMD(0x00);
	SSD1327_CMD(0x7F);

	SSD1327_CMD(SSD1327_SETROWADDRESS);
	SSD1327_CMD(0x00);
	SSD1327_CMD(0x7F);

	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ssd1337_spi, (uint8_t*)&buffer, BUF_SIZE, 100);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

}

void SSD1327_CLR(void){
		memset(buffer, (0 << 4 | 0), BUF_SIZE);
}

void SSD1327_SetPixel( int x , int y , uint8_t bw){
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

