#include "USI_TWI_Master.h"
#include "ads1115.h"

static uint16_t config_reg;

static void _writeRegister(uint8_t address, uint8_t reg, uint16_t value)
{
	uint8_t config[3] = {reg, (uint8_t)(value >> 8), (uint8_t)(value & 0xFF)};

	USI_TWI_Start(address << 1);
	USI_TWI_Write(config, 3);
	USI_TWI_Stop();
}

static uint16_t _readRegister(uint8_t address, uint8_t reg)
{
	uint8_t data[2] = {0};
	uint16_t value = 0;

	USI_TWI_Start((address << 1));
	USI_TWI_Write(&reg, 1);
	USI_TWI_Stop();

	USI_TWI_Start((address << 1) + 1);
	if (USI_TWI_Read(data, 2))
	{
		value = data[0] << 8 | data[1];
	}

	return value;
}

void ADC_init(void)
{
	config_reg = ADS1X15_PGA_2_048V | ADS1X15_MODE_SINGLE | ADS1X15_DATA_RATE_128_SPS | ADS1X15_COMP_QUE_NONE;
	_writeRegister(ADS1115_ADDR, ADS1X15_REG_CONFIG, config_reg);
}

uint8_t ADC_isReady(void)
{
	uint16_t val = _readRegister(ADS1115_ADDR, ADS1X15_REG_CONFIG);
	return ((val & ADS1X15_OS_NOT_BUSY) > 0);
}

uint16_t ADC_getValue(void)
{
	uint16_t raw = _readRegister(ADS1115_ADDR, ADS1X15_REG_CONVERT);
	return raw;
}

void ADC_requestConv(uint8_t channel)
{
	uint16_t _config_reg = config_reg | ADS1X15_OS_START_SINGLE;

	switch (channel)
	{
	case 0:
		_config_reg |= ADS1X15_READ_0;
		break;
	case 1:
		_config_reg |= ADS1X15_READ_1;
		break;
	case 2:
		_config_reg |= ADS1X15_READ_2;
		break;
	case 3:
		_config_reg |= ADS1X15_READ_3;
		break;

	default:
		return;
	}
	_writeRegister(ADS1115_ADDR, ADS1X15_REG_CONFIG, _config_reg);
}
