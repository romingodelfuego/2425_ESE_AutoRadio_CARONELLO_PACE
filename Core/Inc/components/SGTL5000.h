/*
 * SGTL5000.h
 *
 *  Created on: Dec 3, 2024
 *      Author: romingo
 */

#ifndef INC_COMPONENTS_SGTL5000_H_
#define INC_COMPONENTS_SGTL5000_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "sai.h"
#include "shell.h"

#define CHIP_ANA_POWER 0x0030
#define CHIP_LINREG_CTRL 0x0026
#define CHIP_REF_CTRL 0x0028
#define CHIP_LINE_OUT_CTRL 0x002C
#define CHIP_SHORT_CTRL 0x003C
#define CHIP_ANA_CTRL 0x0024
#define CHIP_LINE_OUT_VOL 0x002E
#define CHIP_CLK_CTRL 0x0004
#define CHIP_DIG_POWER 0x0002
#define CHIP_I2S_CTRL 0x0006
#define CHIP_ADCDAC_CTRL 0x000E
#define CHIP_DAC_VOL 0x0010
#define CHIP_SSS_CTRL 0x000A

typedef struct{

}h_sgtl5000_t;

void SGTL_readregister(uint16_t reg, uint8_t*pData);
void SGTL_writeregister(uint16_t reg, uint16_t data);
int sgtl5000_i2c_read_register(uint16_t reg_address, uint16_t * p_data);
int sgtl5000_i2c_write_register(uint16_t reg_address, uint16_t data);
int sgtl5000_i2c_set_bit(uint16_t reg_address, uint16_t mask);
int sgtl5000_i2c_clear_bit(uint16_t reg_address, uint16_t mask);
void SGTL_init(void);
HAL_StatusTypeDef sgtl5000_init(void);

typedef enum sgtl5000_registers_enum
{
	SGTL5000_CHIP_ID = 0x0000,
	SGTL5000_CHIP_DIG_POWER = 0x0002,
	SGTL5000_CHIP_CLK_CTRL = 0x0004,
	SGTL5000_CHIP_I2S_CTRL = 0x0006,
	SGTL5000_CHIP_SSS_CTRL = 0x000A,
	SGTL5000_CHIP_ADCDAC_CTRL = 0x000E,
	SGTL5000_CHIP_DAC_VOL = 0x0010,
	SGTL5000_CHIP_PAD_STRENGTH = 0x0014,
	SGTL5000_CHIP_ANA_ADC_CTRL = 0x0020,
	SGTL5000_CHIP_ANA_HP_CTRL = 0x0022,
	SGTL5000_CHIP_ANA_CTRL = 0x0024,
	SGTL5000_CHIP_LINREG_CTRL = 0x0026,
	SGTL5000_CHIP_REF_CTRL = 0x0028,
	SGTL5000_CHIP_MIC_CTRL = 0x002A,
	SGTL5000_CHIP_LINE_OUT_CTRL = 0x002C,
	SGTL5000_CHIP_LINE_OUT_VOL = 0x002E,
	SGTL5000_CHIP_ANA_POWER = 0x0030,
	SGTL5000_CHIP_PLL_CTRL = 0x0032,
	SGTL5000_CHIP_CLK_TOP_CTRL = 0x0034,
	SGTL5000_SHIP_ANA_STATUS = 0x0036,
	SGTL5000_CHIP_ANA_TEST1 = 0x0038,
	SGTL5000_CHIP_ANA_TEST2 = 0x003A,
	SGTL5000_CHIP_SHORT_CTRL = 0x003C,
	SGTL5000_DAP_CONTROL = 0x0100,
	SGTL5000_DAP_PEQ = 0x0102,
	SGTL5000_DAP_BASS_ENHANCE = 0x0104,
	SGTL5000_DAP_BASS_ENHANCE_CTRL = 0x0106,
	SGTL5000_DAP_AUDIO_EQ = 0x0108,
	SGTL5000_DAP_SGTL_SURROUND = 0x010A,
	SGTL5000_DAP_FILTER_COEF_ACCESS = 0x010C,
	SGTL5000_DAP_COEF_WR_B0_MSB = 0x010E,
	SGTL5000_DAP_COEF_WR_B0_LSB = 0x0110,
	SGTL5000_DAP_AUDIO_EQ_BASS_BAND0 = 0x0116,
	SGTL5000_DAP_AUDIO_EQ_BAND1 = 0x0118,
	SGTL5000_DAP_AUDIO_EQ_BAND2 = 0x011A,
	SGTL5000_DAP_AUDIO_EQ_BAND3 = 0x011C,
	SGTL5000_DAP_AUDIO_EQ_TREBLE_BAND4 = 0x011E,
	SGTL5000_DAP_MAIN_CHAN = 0x0120,
	SGTL5000_DAP_MIX_CHAN = 0x0122,
	SGTL5000_DAP_AVC_CTRL = 0x0124,
	SGTL5000_DAP_AVC_THRESHOLD = 0x0126,
	SGTL5000_DAP_AVC_ATTACK = 0x0128,
	SGTL5000_DAP_AVC_DECAY = 0x012A,
	SGTL5000_DAP_COEF_WR_B1_MSB = 0x012C,
	SGTL5000_DAP_COEF_WR_B1_LSB = 0x012E,
	SGTL5000_DAP_COEF_WR_B2_MSB = 0x0130,
	SGTL5000_DAP_COEF_WR_B2_LSB = 0x0132,
	SGTL5000_DAP_COEF_WR_A1_MSB = 0x0134,
	SGTL5000_DAP_COEF_WR_A1_LSB = 0x0136,
	SGTL5000_DAP_COEF_WR_A2_MSB = 0x0138,
	SGTL5000_DAP_COEF_WR_A2_LSB = 0x013A
} sgtl5000_registers_t;

#endif /* INC_COMPONENTS_SGTL5000_H_ */


