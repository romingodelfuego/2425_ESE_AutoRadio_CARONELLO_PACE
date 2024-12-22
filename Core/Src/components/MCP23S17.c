/*
 * MCP23S17.c
 *
 *  Created on: Nov 29, 2024
 *      Author: romingo
 */

#include "components/MCP23S17.h"
#include "shell.h"

#define MCP23S17_ADDR 0x40//A0=A1=A2=GND
#define MCP23S17_SPI hspi3

void MCP23S17_init(void){

	/* 0x00 - IODIRA | 0x01 - IODIRB
	 * 0 : All Pins as Output
	 */
	MCP23S17_WriteRegister(0x00, 0x00) == HAL_OK ? debug(START,"MCP23S17 - IODIRA") : debug(D_ERROR,"MCP23S17 - IODIRA");
	osDelay(10);
	MCP23S17_WriteRegister(0x01, 0x00) == HAL_OK ? debug(START,"MCP23S17 - IODIRB") : debug(D_ERROR,"MCP23S17 - IODIRB");

	/* 0x12 - GPIOA | 0x13 - GPIOB
	 *
	 */
	MCP23S17_WriteRegister(0x12, 0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
	MCP23S17_WriteRegister(0x13, 0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

	for (int i = 0; i<9; i++){
		MCP23S17_WriteRegister(0x12, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
		osDelay(50);
	}
	for (int i = 0; i<=8; i++){
		MCP23S17_WriteRegister(0x13, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOB") : debug(D_ERROR,"MCP23S17 - GPIOB");
		osDelay(50);
	}
	osDelay(500);
	MCP23S17_WriteRegister(0x12, 0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
	MCP23S17_WriteRegister(0x13, 0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
	osDelay(500);
	for (int i =0;i<9;i++){
		MCP23S17_WriteRegister(0x12, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
		MCP23S17_WriteRegister(0x13, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOB") : debug(D_ERROR,"MCP23S17 - GPIOB");
		osDelay(50);
	}
	osDelay(100);
	for (int i = 8; i>=0; i--){
		MCP23S17_WriteRegister(0x12, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");
		MCP23S17_WriteRegister(0x13, (0xFF<<i)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOB") : debug(D_ERROR,"MCP23S17 - GPIOB");
		osDelay(50);
	}

}

HAL_StatusTypeDef MCP23S17_WriteRegister(uint8_t reg, uint8_t data){
	uint8_t pData[3];

	pData[0] = MCP23S17_ADDR ;
	pData[1] = reg;
	pData[2] = data;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_StatusTypeDef ret = HAL_SPI_Transmit(&MCP23S17_SPI, pData, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	return ret;

}
