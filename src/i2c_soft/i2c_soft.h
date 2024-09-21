#pragma once

/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the MIT License.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <avr/io.h>

#ifndef I2C_BUFFER_LENGTH
#define I2C_BUFFER_LENGTH 32
#endif

#define DDR_USI DDRA
#define PORT_USI PORTA
#define PIN_USI PINA
#define PORT_USI_SDA PORTA6
#define PORT_USI_SCL PORTA4
#define PIN_USI_SDA PINA6
#define PIN_USI_SCL PINA4

#define WIRE_HAS_END 1
#define I2C_READ 1
#define I2C_WRITE 0
#define DELAY 4 // usec delay
#define I2C_MAXWAIT 5000

#define TRUE 1
#define FALSE 0

void I2C_init(void);

void I2C_beginTransmission(uint8_t address);
uint8_t I2C_endTransmission(uint8_t sendStop);

uint8_t I2C_write(const uint8_t *data, uint8_t quantity);

uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop);

int I2C_dataReady(void);
int I2C_dataPop(void);
int I2C_dataPeek(void);

// inline uint8_t write(unsigned long n) { return write((uint8_t)n); }
// inline uint8_t write(long n) { return write((uint8_t)n); }
// inline uint8_t write(unsigned int n) { return write((uint8_t)n); }
// inline uint8_t write(int n) { return write((uint8_t)n); }
