#include <avr/io.h>
#include <util/delay.h>
#include "i2c_soft.h"

uint8_t rxBuffer[I2C_BUFFER_LENGTH];
uint8_t rxBufferIndex;
uint8_t rxBufferLength;
uint8_t isTransmitting;
uint8_t error;
uint8_t sda;
uint8_t scl;
uint8_t pullup;

void _i2c_init(void);
uint8_t _i2c_start(uint8_t addr);
uint8_t _i2c_startRep(uint8_t addr);
void _i2c_stop(void);
uint8_t _i2c_write(uint8_t value);
uint8_t _i2c_read(uint8_t last);

void I2C_init(void)
{
	rxBufferIndex = 0;
	rxBufferLength = 0;
	error = 0;
	isTransmitting = FALSE;

	_i2c_init();
}

void I2C_beginTransmission(uint8_t address)
{
	if (isTransmitting)
	{
		error = (_i2c_startRep((address << 1) | I2C_WRITE) ? 0 : 2);
	}
	else
	{
		error = (_i2c_start((address << 1) | I2C_WRITE) ? 0 : 2);
	}
	// indicate that we are isTransmitting
	isTransmitting = 1;
}

uint8_t I2C_endTransmission(uint8_t sendStop)
{
	uint8_t transferError = error;
	if (sendStop)
	{
		_i2c_stop();
		isTransmitting = 0;
	}
	error = 0;
	return transferError;
}

// uint8_t I2C_write(uint8_t data)
// {
//     if (_i2c_write(data))
//     {
//         return 1;
//     }
//     else
//     {
//         if (error == 0)
//             error = 3;
//         return 0;
//     }
// }

uint8_t I2C_write(const uint8_t *data, uint8_t quantity)
{
	uint8_t progress = 0;
	for (uint8_t i = 0; i < quantity; ++i)
	{
		progress += _i2c_write(data[i]);
	}
	return progress;
}

uint8_t I2C_requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
	error = 0;
	uint8_t localerror = 0;
	if (isize > 0)
	{
		I2C_beginTransmission(address);
		// the maximum size of internal address is 3 bytes
		if (isize > 3)
		{
			isize = 3;
		}
		// write internal register address - most significant byte first
		while (isize-- > 0)
		{
			_i2c_write((uint8_t)(iaddress >> (isize * 8)));
		}
		I2C_endTransmission(FALSE);
	}
	// clamp to buffer length
	if (quantity > I2C_BUFFER_LENGTH)
	{
		quantity = I2C_BUFFER_LENGTH;
	}
	if (isTransmitting)
	{
		localerror = !_i2c_startRep((address << 1) | I2C_READ);
	}
	else
	{
		localerror = !_i2c_start((address << 1) | I2C_READ);
	}
	if (error == 0 && localerror)
		error = 2;
	// perform blocking read into buffer
	for (uint8_t count = 0; count < quantity; count++)
	{
		rxBuffer[count] = _i2c_read(count == quantity - 1);
	}
	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = error ? 0 : quantity;
	if (sendStop)
	{
		isTransmitting = 0;
		_i2c_stop();
	}
	return rxBufferLength;
}

// uint8_t I2C_requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
// {
//     return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
// }

// uint8_t I2C_requestFrom(int address, int quantity, int sendStop)
// {
//     return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
// }

// uint8_t I2C_requestFrom(uint8_t address, uint8_t quantity)
// {
//     return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)TRUE);
// }

// uint8_t I2C_requestFrom(int address, int quantity)
// {
//     return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)TRUE);
// }

int I2C_dataReady(void)
{
	return rxBufferLength - rxBufferIndex;
}

int I2C_dataPop(void)
{
	int value = -1;
	if (rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}
	return value;
}

int I2C_dataPeek(void)
{
	int value = -1;

	if (rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
	}
	return value;
}

/**
 * @brief Initializes the SoftI2C module.
 *
 * This function needs to be called once in the beginning to initialize the SoftI2C module.
 * It returns FALSE if either the SDA or SCL lines are low, which may indicate an I2C bus lockup
 * or that the lines are not properly pulled up.
 *
 * @return TRUE if initialization is successful, FALSE otherwise.
 */
void _i2c_init(void)
{
	PORT_USI |= (1 << PIN_USI_SDA); // Enable pullup on SDA, to set high as released state.
	PORT_USI |= (1 << PIN_USI_SCL); // Enable pullup on SCL, to set high as released state.

	DDR_USI |= (1 << PIN_USI_SCL); // Enable SCL as output.
	DDR_USI |= (1 << PIN_USI_SDA); // Enable SDA as output.

	// digitalWrite(sda, LOW);
	// digitalWrite(scl, LOW);
	// setPinHigh(sda);
	// setPinHigh(scl);
	// if (digitalRead(sda) == LOW || digitalRead(scl) == LOW)
	//     return FALSE;
	// return TRUE;
}

/**
 * @brief Start an I2C transfer.
 *
 * This function starts an I2C transfer by pulling the data line (SDA) low followed by the clock line (SCL).
 * The 8-bit I2C address (including the R/W bit) is passed as a parameter.
 *
 * @param addr The 8-bit I2C address to communicate with.
 * @return Returns TRUE if the slave responds with an "acknowledge" signal, FALSE otherwise.
 */
uint8_t _i2c_start(uint8_t addr)
{
	PORT_USI &= ~(1 << PIN_USI_SDA); // Force SDA LOW.
	_delay_us(DELAY);
	PORT_USI &= ~(1 << PIN_USI_SCL); // Pull SCL LOW.
	// setPinLow(sda);
	// delayMicroseconds(DELAY);
	// setPinLow(scl);
	return _i2c_write(addr);
}

/**
 * @brief Try to start transfer until an ACK is returned.
 *
 * This function attempts to start an I2C transfer to the specified address and
 * waits for an acknowledgment (ACK) to be returned. If the ACK is not received
 * within a certain number of retries, the function gives up and returns FALSE.
 *
 * @param addr The 7-bit I2C address of the target device.
 * @return Returns TRUE if the I2C start and ACK were successful, FALSE otherwise.
 */
// bool I2C_i2cStartWait(uint8_t addr)
// {
//     long retry = I2C_MAXWAIT;
//     while (!_i2c_start(addr))
//     {
//         i2cStop();
//         if (--retry == 0)
//             return FALSE;
//     }
//     return TRUE;
// }

/**
 * @brief Repeated start function.
 *
 * After having claimed the bus with a start condition,
 * you can address another or the same chip again without an intervening
 * stop condition.
 *
 * @param addr The 7-bit address of the slave device.
 *
 * @return TRUE if the slave replies with an "acknowledge", FALSE otherwise.
 */
uint8_t _i2c_startRep(uint8_t addr)
{
	PORT_USI |= (1 << PIN_USI_SDA);
	PORT_USI |= (1 << PIN_USI_SCL);
	_delay_us(DELAY);
	// setPinHigh(sda);
	// setPinHigh(scl);
	// delayMicroseconds(DELAY);
	return _i2c_start(addr);
}

/**
 * @brief Issue a stop condition, freeing the bus.
 *
 * This function generates a stop condition on the I2C bus,
 * releasing the bus for other devices to use.
 */
void _i2c_stop(void)
{
	PORT_USI &= ~(1 << PIN_USI_SDA);
	_delay_us(DELAY);
	PORT_USI |= (1 << PIN_USI_SCL);
	_delay_us(DELAY);
	PORT_USI |= (1 << PIN_USI_SDA);
	_delay_us(DELAY);
	// setPinLow(sda);
	// delayMicroseconds(DELAY);
	// setPinHigh(scl);
	// delayMicroseconds(DELAY);
	// setPinHigh(sda);
	// delayMicroseconds(DELAY);
}

/**
 * @brief Write one byte to the slave chip that had been addressed by the previous start call.
 *
 * @param value The byte to be sent.
 * @return TRUE if the slave replies with an "acknowledge," FALSE otherwise.
 */
uint8_t _i2c_write(uint8_t value)
{
	for (uint8_t curr = 0X80; curr != 0; curr >>= 1)
	{
		if (curr & value)
		{
			PORT_USI |= (1 << PIN_USI_SDA);
			// setPinHigh(sda);
		}
		else
		{
			PORT_USI &= ~(1 << PIN_USI_SDA);
			// setPinLow(sda);
		}
		PORT_USI |= (1 << PIN_USI_SCL);
		_delay_us(DELAY);
		PORT_USI &= ~(1 << PIN_USI_SCL);
		// setPinHigh(scl);
		// delayMicroseconds(DELAY);
		// setPinLow(scl);
	}
	PORT_USI |= (1 << PIN_USI_SDA);
	PORT_USI |= (1 << PIN_USI_SCL);
	_delay_us(DELAY / 2);
	// setPinHigh(sda);
	// setPinHigh(scl);
	// delayMicroseconds(DELAY / 2);

	uint8_t ack = PIN_USI & (1 << PIN_USI_SCL);
	// uint8_t ack = digitalRead(sda);

	PORT_USI &= ~(1 << PIN_USI_SCL);
	_delay_us(DELAY / 2);
	PORT_USI &= ~(1 << PIN_USI_SDA);
	// setPinLow(scl);
	// delayMicroseconds(DELAY / 2);
	// setPinLow(sda);
	return ack == 0;
}

/**
 * @brief Read one byte from the I2C bus.
 *
 * This function reads a single byte from the I2C bus. If the parameter `last` is TRUE,
 * a NAK (Not Acknowledge) signal is sent after receiving the byte to terminate the
 * read sequence.
 *
 * @param last Set to TRUE if NAK should be sent after the byte read.
 * @return The byte read from the I2C bus.
 */
uint8_t _i2c_read(uint8_t last)
{
	uint8_t receivedByte = 0;
	PORT_USI |= (1 << PIN_USI_SDA);
	// setPinHigh(sda);
	for (uint8_t i = 0; i < 8; i++)
	{
		receivedByte <<= 1;
		_delay_us(DELAY / 2);
		PORT_USI |= (1 << PIN_USI_SCL);
		// setPinHigh(scl);
		if (PIN_USI & (1 << PIN_USI_SDA))
		{
			receivedByte |= 1;
		}
		PORT_USI &= ~(1 << PIN_USI_SCL);
		// setPinLow(scl);
	}
	if (last)
	{
		PORT_USI |= (1 << PIN_USI_SDA);
		// setPinHigh(sda);
	}
	else
	{
		PORT_USI &= ~(1 << PIN_USI_SDA);
		// setPinLow(sda);
	}
	PORT_USI |= (1 << PIN_USI_SCL);
	_delay_us(DELAY / 2);
	PORT_USI &= ~(1 << PIN_USI_SCL);
	_delay_us(DELAY / 2);
	PORT_USI &= ~(1 << PIN_USI_SDA);
	// setPinHigh(scl);
	// delayMicroseconds(DELAY / 2);
	// setPinLow(scl);
	// delayMicroseconds(DELAY / 2);
	// setPinLow(sda);
	return receivedByte;
}
/**
 * @brief Set the specified pin to a low state.
 *
 * This function sets the specified pin to a low (logic level 0) state. If pull-up
 * is enabled, it first ensures that the pin is set to a low state and then sets it
 * as an output pin. Interrupts are temporarily disabled during this process.
 *
 * @param pin The pin to be set to a low state.
 */
// void I2C_setPinLow(uint8_t pin)
// {
// 	cli();
// 	if (pullup)
// 		digitalWrite(pin, LOW);
// 	pinMode(pin, OUTPUT);
// 	interrupts();
// }

/**
 * @brief Set the specified pin to a high level and configure its mode.
 *
 * This function sets the specified pin to a high level and configures its mode
 * according to the pull-up setting.
 *
 * @param pin The pin number to set high and configure.
 */
// void I2C_setPinHigh(uint8_t pin)
// {
// 	noInterrupts();
// 	if (pullup)
// 		pinMode(pin, INPUT_PULLUP);
// 	else
// 		pinMode(pin, INPUT);
// 	interrupts();
// }
