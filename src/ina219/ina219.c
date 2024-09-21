#include "USI_TWI_Master.h"
#include "ina219.h"

#define INA219_ADDR 0x40

#define RST 15
#define BRNG 13
#define PG1 12
#define PG0 11
#define BADC4 10
#define BADC3 9
#define BADC2 8
#define BADC1 7
#define SADC4 6
#define SADC3 5
#define SADC2 4
#define SADC1 3
#define MODE3 2
#define MODE2 1
#define MODE1 0

#define INA219_CONFIG_RESET (1 << RST)

#define INA219_CONFIG_BVOLTAGERANGE_16V (0 << BRNG) // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V (1 << BRNG) // 0-32V Range

#define INA219_CONFIG_GAIN_1_40MV ((0 << PG1) | (0 << PG0))	 // Gain 1, 40mV Range
#define INA219_CONFIG_GAIN_2_80MV ((0 << PG1) | (1 << PG0))	 // Gain 2, 80mV Range
#define INA219_CONFIG_GAIN_4_160MV ((1 << PG1) | (0 << PG0)) // Gain 4, 160mV Range
#define INA219_CONFIG_GAIN_8_320MV ((1 << PG1) | (1 << PG0)) // Gain 8, 320mV Range

#define INA219_CONFIG_BADCRES_9BIT ((0 << BADC4) | (0 << BADC2) | (0 << BADC1))							   // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT ((0 << BADC4) | (0 << BADC2) | (1 << BADC1))						   // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT ((0 << BADC4) | (1 << BADC2) | (0 << BADC1))						   // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT ((0 << BADC4) | (1 << BADC2) | (1 << BADC1))						   // 12-bit bus res = 0..4097
#define INA219_CONFIG_BADCRES_12BIT_1S_532US ((1 << BADC4) | (0 << BADC3) | (0 << BADC2) | (0 << BADC1))   // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_2S_1060US ((1 << BADC4) | (0 << BADC3) | (0 << BADC2) | (0 << BADC1))  // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_4S_2130US ((1 << BADC4) | (0 << BADC3) | (0 << BADC2) | (1 << BADC1))  // 4 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_8S_4260US ((1 << BADC4) | (0 << BADC3) | (1 << BADC2) | (0 << BADC1))  // 8 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_16S_8510US ((1 << BADC4) | (1 << BADC3) | (1 << BADC2) | (1 << BADC1)) // 16 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_32S_17MS ((1 << BADC4) | (1 << BADC3) | (0 << BADC2) | (0 << BADC1))   // 32 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_64S_34MS ((1 << BADC4) | (1 << BADC3) | (0 << BADC2) | (1 << BADC1))   // 64 x 12-bit bus samples averaged together
#define INA219_CONFIG_BADCRES_12BIT_128S_69MS ((1 << BADC4) | (1 << BADC3) | (1 << BADC2) | (0 << BADC1))  // 128 x 12-bit bus samples averaged together

#define INA219_CONFIG_SADCRES_9BIT ((0 << SADC4) | (0 << SADC2) | (0 << SADC1))							   // 9-bit bus res = 0..511
#define INA219_CONFIG_SADCRES_10BIT ((0 << SADC4) | (0 << SADC2) | (1 << SADC1))						   // 10-bit bus res = 0..1023
#define INA219_CONFIG_SADCRES_11BIT ((0 << SADC4) | (1 << SADC2) | (0 << SADC1))						   // 11-bit bus res = 0..2047
#define INA219_CONFIG_SADCRES_12BIT ((0 << SADC4) | (1 << SADC2) | (1 << SADC1))						   // 12-bit bus res = 0..4097
#define INA219_CONFIG_SADCRES_12BIT_1S_532US ((1 << SADC4) | (0 << SADC3) | (0 << SADC2) | (0 << SADC1))   // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US ((1 << SADC4) | (0 << SADC3) | (0 << SADC2) | (0 << SADC1))  // 2 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US ((1 << SADC4) | (0 << SADC3) | (0 << SADC2) | (1 << SADC1))  // 4 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US ((1 << SADC4) | (0 << SADC3) | (1 << SADC2) | (0 << SADC1))  // 8 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US ((1 << SADC4) | (1 << SADC3) | (1 << SADC2) | (1 << SADC1)) // 16 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS ((1 << SADC4) | (1 << SADC3) | (0 << SADC2) | (0 << SADC1))   // 32 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS ((1 << SADC4) | (1 << SADC3) | (0 << SADC2) | (1 << SADC1))   // 64 x 12-bit bus samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS ((1 << SADC4) | (1 << SADC3) | (1 << SADC2) | (0 << SADC1))  // 128 x 12-bit bus samples averaged together

#define INA219_CONFIG_MODE_POWERDOWN ((0 << MODE3) | (0 << MODE2) | (0 << MODE1))			 // power down
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED ((0 << MODE3) | (0 << MODE2) | (1 << MODE1))		 // shunt voltage triggered
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED ((0 << MODE3) | (1 << MODE2) | (0 << MODE1))		 // bus voltage triggered
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED ((0 << MODE3) | (1 << MODE2) | (1 << MODE1))	 // shunt and bus voltage triggered
#define INA219_CONFIG_MODE_ADCOFF ((1 << MODE3) | (0 << MODE2) | (0 << MODE1))				 // ADC off
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS ((1 << MODE3) | (0 << MODE2) | (1 << MODE1))	 // shunt voltage continuous
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS ((1 << MODE3) | (1 << MODE2) | (0 << MODE1))	 // bus voltage continuous
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS ((1 << MODE3) | (1 << MODE2) | (1 << MODE1)) // shunt and bus voltage continuous

/** config register address **/
#define INA219_REG_CONFIG (0x00)

/** shunt voltage register **/
#define INA219_REG_SHUNTVOLTAGE (0x01)

/** bus voltage register **/
#define INA219_REG_BUSVOLTAGE (0x02)

/** power register **/
#define INA219_REG_POWER (0x03)

/** current register **/
#define INA219_REG_CURRENT (0x04)

/** calibration register **/
#define INA219_REG_CALIBRATION (0x05)

#define INA219_REG_CALIB_VALUE (4096)

// uint8_t voltMultiplier;
// uint8_t currDivider;
// uint8_t powerMultiplier;

static void _writeRegister(uint8_t reg, uint16_t value)
{
	uint8_t config[3] = {reg, (uint8_t)(value >> 8), (uint8_t)(value & 0xFF)};

	USI_TWI_Start(INA219_ADDR << 1);
	USI_TWI_Write(config, 3);
	USI_TWI_Stop();
}

static uint16_t _readRegisgter(uint8_t reg)
{
	uint8_t data[2] = {0};
	uint16_t value = 0;

	USI_TWI_Start((INA219_ADDR << 1));
	USI_TWI_Write(&reg, 1);
	USI_TWI_Stop();

	USI_TWI_Start((INA219_ADDR << 1) + 1);
	if (USI_TWI_Read(data, 2))
	{
		value = data[0] << 8 | data[1];
	}

	return value;
}

static uint16_t _getShuntVoltage_raw(void)
{
	uint16_t value = _readRegisgter(INA219_REG_SHUNTVOLTAGE);
	return value;
}

static uint16_t _getBusVoltage_raw(void)
{
	uint16_t value = _readRegisgter(INA219_REG_BUSVOLTAGE);
	// Shift to the right 3 to drop CNVR and OVF and multiply by LSB
	return (uint16_t)((value >> 3) * 4);
}

static uint16_t _getCurrent_raw(void)
{
	_writeRegister(INA219_REG_CALIBRATION, INA219_REG_CALIB_VALUE);

	// Now we can safely read the CURRENT register!
	uint16_t value = _readRegisgter(INA219_REG_CURRENT);
	return value;
}

static uint16_t _getPower_raw(void)
{
	_writeRegister(INA219_REG_CALIBRATION, INA219_REG_CALIB_VALUE);

	// Now we can safely read the POWER register!
	uint16_t value = _readRegisgter(INA219_REG_POWER);
	return value;
}

uint16_t INA_getShuntVoltage(void)
{
	return _getShuntVoltage_raw(); // 100;
}

uint16_t INA_getBasVoltage(void)
{
	return (_getBusVoltage_raw() + 10) / 10;
}

uint16_t INA_getCurrent(void)
{
	return _getCurrent_raw() / 10; // currDivider;
}

uint16_t getPower_mW(void)
{
	return _getPower_raw(); //* powerMultiplier;
}

void INA_init(void)
{
	_writeRegister(INA219_REG_CALIBRATION, INA219_REG_CALIB_VALUE);

	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
					  INA219_CONFIG_GAIN_8_320MV |
					  INA219_CONFIG_BADCRES_12BIT |
					  INA219_CONFIG_SADCRES_12BIT_4S_2130US |
					  INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	_writeRegister(INA219_REG_CONFIG, config);
}