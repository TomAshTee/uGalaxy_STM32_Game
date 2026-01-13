/**
 * @file    ssd1327.c
 * @author  Tomasz Jaeschke
 * @date    2026-01-13
 * @brief   Low-level driver implementation for SSD1327 OLED display.
 * @details
 * This file contains all low-level functions required to initialize,
 * control, and update the SSD1327 OLED display over SPI (optionally with DMA).
 * It provides buffer management, command transmission, and pixel operations.
 */

#include "main.h"
#include "ssd1327.h"
#include <string.h>
#include <stdbool.h>

/** ##########################################################################
 *  @name Private Variables and Functions
 *  @brief Internal buffers and SPI handler.
 *  ##########################################################################
 */

static SPI_HandleTypeDef *ssd1327_spi;					/**< Pointer to active SPI handle. */

static uint8_t _bufferBack[BUF_SIZE]; 			/**< Back buffer – used for drawing. */
static uint8_t _bufferFront[BUF_SIZE];			/**< Front buffer – used for transmission (DMA). */
static uint8_t* _bufferDraw = _bufferBack;		/**< Pointer to current draw buffer. */
static uint8_t* _bufferTx = _bufferFront;		/**< Pointer to current TX buffer. */

static volatile bool _ssd1327DMA_Busy = false;	/**< Flag indicating DMA transfer in progress. */


static void SSD1327_Init (void); 									/**< Initialising the display */
static void SSD1327_Reset(void);									/**< Screen reset */
static void SSD1327_CMD (uint8_t cmd); 								/**< Sending commands */
//static void SSD1327_SetContrast(uint8_t Contrast);					/**< Contrast setting */
static void SSD1327_StartDMATransfer(uint8_t* data, size_t len);	/**< Starts DMA transfer of display buffer.*/


/** ##########################################################################
 *  @name Command and Initialization
 *  @brief Functions for control and setup.
 *  ##########################################################################
 */

/**
 * @brief Sends a single command byte to the SSD1327 controller.
 * @param cmd Command byte to send.
 */
static void SSD1327_CMD (uint8_t cmd){

	HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(ssd1327_spi, &cmd, 1, 10);
	HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);

}

/**
 * @brief Sets display contrast.
 * @param Contrast Contrast value (0–255).
 */
//static void SSD1327_SetContrast(uint8_t Contrast)
//{
//	SSD1327_CMD(SSD1327_SETCONTRASTCURRENT);	// Set Contrast Control
//	SSD1327_CMD(Contrast);
//}

/**
 * @brief Performs hardware reset sequence for SSD1327.
 */
static void SSD1327_Reset(void){

	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(RST_PORT, RST, GPIO_PIN_SET);
	HAL_Delay(20);

}

/**
 * @brief Initializes SPI interface and display.
 * @param spi Pointer to SPI handle.
 */
void SSD1327_SpiInit(SPI_HandleTypeDef *spi){

	ssd1327_spi = spi;

	SSD1327_Reset();
	SSD1327_Init();

}

/**
 * @brief Sends full initialization command sequence to SSD1327.
 * @details Configures addressing mode, contrast, remapping, voltages, etc.
 */
static void SSD1327_Init (void){

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


/** ##########################################################################
 *  @name Framebuffer and Rendering
 *  @brief Functions for drawing, clearing, and displaying data.
 *  ##########################################################################
 */

/**
 * @brief Transfers frame buffer to display.
 * @details If DMA is enabled, starts asynchronous transfer using @ref SSD1327_Present().
 */
void SSD1327_Display (void){

#ifdef SSD1327_USE_DMA
    (void)SSD1327_Present();  /// Skip frame if DMA busy
#else
    SSD1327_CMD(SSD1327_SETCOLUMNADDRESS); SSD1327_CMD(0x00); SSD1327_CMD(0x7F);
    SSD1327_CMD(SSD1327_SETROWADDRESS);    SSD1327_CMD(0x00); SSD1327_CMD(0x7F);

    HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_SET);
    HAL_SPI_Transmit(ssd1327_spi, (uint8_t*)_bufferDraw, BUF_SIZE, 100);
    HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);
#endif


}

/**
 * @brief Begins a new frame by clearing the back buffer.
 */
void SSD1327_BeginFrame(void){
    memset(_bufferDraw, 0x00, BUF_SIZE);
}

/**
 * @brief Checks if SPI DMA transfer is currently in progress.
 * @return `true` if busy, otherwise `false`.
 */
bool SSD1327_IsBusy(void){
    return _ssd1327DMA_Busy;;
}


/** ##########################################################################
 *  @name DMA Transfer Control
 *  @brief Internal routines for asynchronous frame sending.
 *  ##########################################################################
 */

/**
 * @brief Starts DMA transfer of display buffer.
 * @param data Pointer to buffer data.
 * @param len  Length of buffer in bytes.
 */
static void SSD1327_StartDMATransfer(uint8_t* data, size_t len){
    // set window addresses (commands, blocking - it's only a few bytes)
    SSD1327_CMD(SSD1327_SETCOLUMNADDRESS); SSD1327_CMD(0x00); SSD1327_CMD(0x7F);
    SSD1327_CMD(SSD1327_SETROWADDRESS);    SSD1327_CMD(0x00); SSD1327_CMD(0x7F);

    // DATA: DC=1, CS=0 i DMA
    HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DC_PORT, DC, GPIO_PIN_SET);

    _ssd1327DMA_Busy = true;
    HAL_SPI_Transmit_DMA(ssd1327_spi, data, len);
}

/**
 * @brief Presents current frame by swapping buffers and starting DMA transfer.
 * @return `true` if transfer started, `false` if DMA was busy.
 */
bool SSD1327_Present(void){
    if (_ssd1327DMA_Busy) return false;   // Still transmitting previous frame

    // swapping buffer roles
    uint8_t* tmp = _bufferTx;
    _bufferTx   = _bufferDraw;
    _bufferDraw = tmp;

    // DMA start of the next frame
    SSD1327_StartDMATransfer(_bufferTx, BUF_SIZE);
    return true;
}


/** ##########################################################################
 *  @name Pixel Operations
 *  @brief Manipulation of single pixels in buffer.
 *  ##########################################################################
 */

/**
 * @brief Sets grayscale pixel at given coordinates.
 * @param x  X position (0–127).
 * @param y  Y position (0–127).
 * @param bw 4-bit grayscale value (0–15).
 */
void SSD1327_SetPixel( int x , int y , uint8_t bw){
	if ((x < 0) || (x >= SSD1327_WIDTH) || (y < 0) || (y >= SSD1327_HEIGHT))
		return;

	uint8_t SelectedCell = _bufferDraw[x/2 + y*(SSD1327_WIDTH/2)];

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

	_bufferDraw[x/2 + y*(SSD1327_WIDTH/2)] = SelectedCell;

//	if ((x < 0) || (x >= SSD1327_WIDTH) || (y < 0) || (y >= SSD1327_HEIGHT))
//		return;
//	if (bw) buffer[x + (y/4)*SSD1327_WIDTH] |= (1<<(y%4));
//	else buffer[x + (y/4)*SSD1327_WIDTH] &= ~(1<<(y%4));
}

/** ##########################################################################
 *  @name HAL Callbacks
 *  @brief SPI DMA completion handler.
 *  ##########################################################################
 */

/**
 * @brief HAL callback executed when SPI DMA transmission completes.
 * @param hspi Pointer to SPI handle that triggered the callback.
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {

	if (hspi == ssd1327_spi){
	        HAL_GPIO_WritePin(CS_PORT, CS, GPIO_PIN_SET);   // koniec ramki
	        _ssd1327DMA_Busy = false;
	    }
}

