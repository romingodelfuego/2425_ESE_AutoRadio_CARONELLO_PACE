/*
 * SGTL5000.c
 *
 *  Created on: Dec 3, 2024
 *      Author: romingo
 */


#include "components/SGTL5000.h"



#define SGTL5000_DEV_ADDR 0x14
void SGTL_readregister(uint16_t reg, uint8_t* pData){

	HAL_I2C_Mem_Read(&hi2c2, 0x14, reg, 2, pData, 2, HAL_MAX_DELAY);

}

void SGTL_writeregister(uint16_t reg, uint16_t data){
	uint8_t pData[2];
	pData[0] = (data >> 8);
	pData[1] = (data & 0xFF);
	HAL_I2C_Mem_Write(&hi2c2, 0x14, reg, 2, pData, 2, HAL_MAX_DELAY);
}



void SGTL_init(void){


//	SGTL_writeregister(CHIP_ANA_POWER,0x7260);
//	/*
//	 *
//	 * */
//	SGTL_writeregister(CHIP_DIG_POWER,0x0073);
//	//SGTL_writeregister(CHIP_LINREG_CTRL,0x006);		// because VDDA and VDIO greather than 3.1v
//
//	//SGTL_writeregister(CHIP_LINE_OUT_CTRL,0x0008);;
//	SGTL_writeregister(CHIP_ANA_CTRL,0x1818);
//	SGTL_writeregister(CHIP_I2S_CTRL, 0x0010);
//
	/*
	 * volume max
	 * */
	SGTL_writeregister(CHIP_ANA_POWER, 0x6AFF);
	SGTL_writeregister(CHIP_LINREG_CTRL, 0x0060);

	/*
	//---- Reference Voltage and Bias Current Configuration----
	 * */
	SGTL_writeregister(CHIP_REF_CTRL, 0x01FF);


	SGTL_writeregister(CHIP_CLK_CTRL,0x0004);
	SGTL_writeregister(CHIP_I2S_CTRL,0x0130);
	/*
	 * Le Codex est un slave
	 * DLEN on envoie du 16
	 * */
	SGTL_writeregister(CHIP_DAC_VOL,0x3C3C);
	SGTL_writeregister(CHIP_ANA_CTRL, 0x0004);
	/*
	 * enable input LINEIN
	 * enable output LINOUT
	 * */
	SGTL_writeregister(CHIP_LINE_OUT_CTRL, 0x031E);
	SGTL_writeregister(CHIP_LINE_OUT_VOL, 0x1111);

	/*
	 * oN ALLUME TOUT
	 * */
	SGTL_writeregister(CHIP_ADCDAC_CTRL, 0x0000);
	SGTL_writeregister(CHIP_DIG_POWER, 0x0073);

	uint8_t data[2];
	SGTL_readregister(CHIP_CLK_CTRL, data);
	printf(separator);
	printf("register CHIP_CLK_CTRL, %02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_I2S_CTRL, data);
	printf("register CHIP_I2S_CTRL, %02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_DAC_VOL, data);
	printf("register CHIP_DAC_VOL ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_ANA_CTRL, data);
	printf("register CHIP_ANA_CTRL ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_LINREG_CTRL, data);
	printf("register CHIP_LINREG_CTRL ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_LINE_OUT_CTRL, data);
	printf("register CHIP_LINE_OUT_CTRLl ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_LINE_OUT_VOL, data);
	printf("register CHIP_LINE_OUT_VOL ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_ANA_POWER, data);
	printf("register CHIP_ANA_POWE ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_ADCDAC_CTRL, data);
	printf("register CHIP_ADCDAC_CTRL ,%02X%02X\r\n", data[0],data[1]);

	SGTL_readregister(CHIP_DIG_POWER, data);
	printf("register CHIP_DIG_POWER ,%02X%02X\r\n", data[0],data[1]);
	printf(separator);
}



HAL_StatusTypeDef sgtl5000_init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	uint16_t mask;

	// Chip Powerup and Supply Configurations //

	//--------------- Power Supply Configuration----------------
	// NOTE: This next 2 Write calls is needed ONLY if VDDD is
	// Configure VDDD level to 1.8V (bits 3:0)
	// Write CHIP_LINREG_CTRL 0x????
	// OK, pas touche!
	// Power up internal linear regulator (Set bit 9)
	// Write CHIP_ANA_POWER 0x7260
	// Pas touche non plus

	// NOTE: This next Write call is needed ONLY if VDDD is
	// externally driven
	// Turn off startup power supplies to save power (Clear bit 12 and 13)
	// Write CHIP_ANA_POWER 0x4260
	mask = (1 << 12) | (1 << 13);
	sgtl5000_i2c_clear_bit(SGTL5000_CHIP_ANA_POWER, mask);

	// NOTE: The next Write calls is needed only if both VDDA and
	// VDDIO power supplies are less than 3.1V.
	// Enable the internal oscillator for the charge pump (Set bit 11)
	// Write CHIP_CLK_TOP_CTRL 0x0800
	// Enable charge pump (Set bit 11)
	// Write CHIP_ANA_POWER 0x4A60
	// VDDA and VDDIO = 3.3V so not necessary

	// NOTE: The next modify call is only needed if both VDDA and
	// VDDIO are greater than 3.1 V
	// Configure the charge pump to use the VDDIO rail (set bit 5 and bit 6)
	// Write CHIP_LINREG_CTRL 0x006C
	// VDDA and VDDIO = 3.3V so it IS necessary
	mask = (1 << 5) | (1 << 6);
	sgtl5000_i2c_set_bit(SGTL5000_CHIP_LINREG_CTRL, mask);

	//---- Reference Voltage and Bias Current Configuration----
	// NOTE: The value written in the next 2 Write calls is dependent
	// on the VDDA voltage value.
	// Set ground, ADC, DAC reference voltage (bits 8:4). The value should
	// be set to VDDA/2. This example assumes VDDA = 1.8 V. VDDA/2 = 0.9 V.
	// The bias current should be set to 50% of the nominal value (bits 3:1)
	// Write CHIP_REF_CTRL 0x004E
	mask = 0x01FF;	// VAG_VAL = 1.575V, BIAS_CTRL = -50%, SMALL_POP = 1
	sgtl5000_i2c_write_register(SGTL5000_CHIP_REF_CTRL, mask);

	// Set LINEOUT reference voltage to VDDIO/2 (1.65 V) (bits 5:0)
	// and bias current (bits 11:8) to the recommended value of 0.36 mA
	// for 10 kOhm load with 1.0 nF capacitance
	// Write CHIP_LINE_OUT_CTRL 0x0322
//	mask = 0x0322;	// LO_VAGCNTRL = 1.65V, OUT_CURRENT = 0.36mA (?)
	mask = 0x031E;
	sgtl5000_i2c_write_register(SGTL5000_CHIP_LINE_OUT_CTRL, mask);

	//------------Other Analog Block Configurations--------------
	// Configure slow ramp up rate to minimize pop (bit 0)
	// Write CHIP_REF_CTRL 0x004F
	// Déjà fait

	// Enable short detect mode for headphone left/right
	// and center channel and set short detect current trip level
	// to 75 mA
	// Write CHIP_SHORT_CTRL 0x1106
	mask = 0x1106;	// MODE_CM = 2, MODE_LR = 1, LVLADJC = 200mA, LVLADJL = 75mA, LVLADJR = 50mA
	sgtl5000_i2c_write_register(SGTL5000_CHIP_SHORT_CTRL, mask);

	// Enable Zero-cross detect if needed for HP_OUT (bit 5) and ADC (bit 1)
	// Write CHIP_ANA_CTRL 0x0133
	mask = 0x0004;	// Unmute all + SELECT_ADC = LINEIN
//	mask = 0x0000;	// Unmute all + SELECT_ADC = MIC
	sgtl5000_i2c_write_register( SGTL5000_CHIP_ANA_CTRL, mask);

	//------------Power up Inputs/Outputs/Digital Blocks---------
	// Power up LINEOUT, HP, ADC, DAC
	// Write CHIP_ANA_POWER 0x6AFF
	mask = 0x6AFF;	// LINEOUT_POWERUP, ADC_POWERUP, CAPLESS_HEADPHONE_POWERUP, DAC_POWERUP, HEADPHONE_POWERUP, REFTOP_POWERUP, ADC_MONO = stereo
	// VAG_POWERUP, VCOAMP_POWERUP = 0, LINREG_D_POWERUP, PLL_POWERUP = 0, VDDC_CHRGPMP_POWERUP, STARTUP_POWERUP = 0, LINREG_SIMPLE_POWERUP,
	// DAC_MONO = stereo
	sgtl5000_i2c_write_register(SGTL5000_CHIP_ANA_POWER, mask);
	// Power up desired digital blocks
	// I2S_IN (bit 0), I2S_OUT (bit 1), DAP (bit 4), DAC (bit 5),
	// ADC (bit 6) are powered on
	// Write CHIP_DIG_POWER 0x0073
	mask = 0x0073;	// I2S_IN_POWERUP, I2S_OUT_POWERUP, DAP_POWERUP, DAC_POWERUP, ADC_POWERUP
	sgtl5000_i2c_write_register(SGTL5000_CHIP_DIG_POWER, mask);

	//----------------Set LINEOUT Volume Level-------------------
	// Set the LINEOUT volume level based on voltage reference (VAG)
	// values using this formula
	// Value = (int)(40*log(VAG_VAL/LO_VAGCNTRL) + 15)
	// Assuming VAG_VAL and LO_VAGCNTRL is set to 0.9 V and
	// 1.65 V respectively, the // left LO vol (bits 12:8) and right LO
	// volume (bits 4:0) value should be set // to 5
	// Write CHIP_LINE_OUT_VOL 0x0505
	mask = 0x1f1f;	// TODO recalculer
	sgtl5000_i2c_write_register(SGTL5000_CHIP_LINE_OUT_VOL, mask);

	// System MCLK and Sample Clock //

	// Configure SYS_FS clock to 48 kHz
	// Configure MCLK_FREQ to 256*Fs
	// Modify CHIP_CLK_CTRL->SYS_FS 0x0002 // bits 3:2
	// Modify CHIP_CLK_CTRL->MCLK_FREQ 0x0000 // bits 1:0
	mask = 0x0004;	// SYS_FS = 48kHz
	sgtl5000_i2c_write_register(SGTL5000_CHIP_CLK_CTRL, mask);
	// Configure the I2S clocks in master mode
	// NOTE: I2S LRCLK is same as the system sample clock
	// Modify CHIP_I2S_CTRL->MS 0x0001 // bit 7
	// Non, on reste en slave!
	mask = 0x0130;	// DLEN = 16 bits
	sgtl5000_i2c_write_register(SGTL5000_CHIP_I2S_CTRL, mask);

	// PLL Configuration //
	// Pas utilisé

	// Input/Output Routing //
	// Laissons tout par défaut pour l'instant
//	mask = 0x0000;	// ADC -> DAC
//	sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_SSS_CTRL, mask);

	// Le reste //
	mask = 0x0000;	// Unmute
	sgtl5000_i2c_write_register(SGTL5000_CHIP_ADCDAC_CTRL, mask);

	mask = 0x3C3C;
//	mask = 0x4747;
	sgtl5000_i2c_write_register(SGTL5000_CHIP_DAC_VOL, mask);

	mask = 0x0251;	// BIAS_RESISTOR = 2, BIAS_VOLT = 5, GAIN = 1
	sgtl5000_i2c_write_register(SGTL5000_CHIP_MIC_CTRL, mask);

//	for (int i = 0 ; register_map[i] != SGTL5000_DAP_COEF_WR_A2_LSB ; i++)
//	{
//		uint16_t reg = 0;
//		sgtl5000_i2c_read_register(h_sgtl5000, register_map[i], &reg);
//		printf("%02d: [0x%04x] = 0x%04x\r\n", i, register_map[i], reg);
//	}

	return ret;
}
int sgtl5000_i2c_read_register(uint16_t reg_address, uint16_t * p_data)
{
	HAL_StatusTypeDef ret;
	uint8_t reg[2];
	ret = HAL_I2C_Mem_Read(&hi2c2, SGTL5000_DEV_ADDR, reg_address, I2C_MEMADD_SIZE_16BIT, reg, 2, HAL_MAX_DELAY);

	*p_data = (reg[0] << 8) + reg[1];

	if (ret == HAL_OK)
	{
		// HAL_I2C_Mem_Read s'est bien passé
		return 0;
	}
	else
	{
		// HAL_I2C_Mem_Read a retourné une erreur
		return -1;
	}
}

int sgtl5000_i2c_write_register(uint16_t reg_address, uint16_t data)
{
	HAL_StatusTypeDef ret;
	uint8_t reg[2];

	reg[0] = (data >> 8);
	reg[1] = data & 0xFF;	// Masque (et logique bit à bit entre data et 0x00FF)

	ret = HAL_I2C_Mem_Write(&hi2c2, SGTL5000_DEV_ADDR, reg_address, I2C_MEMADD_SIZE_16BIT, reg, 2, HAL_MAX_DELAY);

	if (ret == HAL_OK)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int sgtl5000_i2c_set_bit(uint16_t reg_address, uint16_t mask)
{
	uint16_t reg;
	// Lire le registre
	sgtl5000_i2c_read_register(reg_address, &reg);

	// Modifier les bits selon le masque
	// Tous les 0 dans le masque ne modifient pas le registre
	// Tous les 1 dans le masque imposent des 1 dans le registre
	// Pour faire ça, on utilise un OU logique (symbole | : altgr+6)
	// Exemple : 0x0001 0010 | 0x0000 0001 = 0x0001 0011
	reg = reg | mask;

	// Écrire le registre
	sgtl5000_i2c_write_register(reg_address, reg);

	return 0;
}

int sgtl5000_i2c_clear_bit(uint16_t reg_address, uint16_t mask)
{
	uint16_t reg;
	// Lire le registre
	sgtl5000_i2c_read_register(reg_address, &reg);

	// Modifier les bits selon le masque
	// Tilde inverse tous les bits (les 0 deviennent des 1, les 1 deviennent des 0)
	// Tilde : altgr+2
	// ~(0x0001 0000) = 0x1110 1111
	reg = reg & (~mask);

	// Écrire le registre
	sgtl5000_i2c_write_register(reg_address, reg);

	return 0;
}



