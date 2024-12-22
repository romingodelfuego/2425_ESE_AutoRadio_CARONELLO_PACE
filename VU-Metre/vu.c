/*
 * vu.c
 *
 *  Created on: Dec 10, 2024
 *      Author: romingo
 */

#include "vu.h"
#include "components/MCP23S17.h"
#include "shell.h"

extern uint16_t data_SAI_rx[SIZE_SAI_BUFFER];
#define U_MAX 2.0

void vu_run(void*){
	MCP23S17_init();
	for(;;){

		ulTaskNotifyTake(pdTRUE, HAL_MAX_DELAY);
		float mean_R=0;
		float mean_L=0;
		for (int i=0;i<SIZE_SAI_BUFFER/2;i++){
			mean_R+=data_SAI_rx[2*i];
			mean_L+=data_SAI_rx[2*(i+1)];
		}
		mean_R = mean_R/SIZE_SAI_BUFFER;
		if (mean_R < U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<0)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 2*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<1)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 3*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<2)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 4*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<3)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 5*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<4)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 6*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<5)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if (mean_R < 7*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<6)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
		else if(mean_R < 8*U_MAX/9.0){
			MCP23S17_WriteRegister(0x12, (0xFF<<7)&0xFF) == HAL_OK ? debug(START,"MCP23S17 - GPIOA") : debug(D_ERROR,"MCP23S17 - GPIOA");

		}
	}
}
