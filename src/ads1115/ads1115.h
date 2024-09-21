#pragma once

#define ADS1115_ADDR 0x48
#define ADS1015_CONVERSION_DELAY 1
#define ADS1115_CONVERSION_DELAY 8

//  Kept #defines a bit in line with Adafruit library.

//  REGISTERS
#define ADS1X15_REG_CONVERT 0x00
#define ADS1X15_REG_CONFIG 0x01
#define ADS1X15_REG_LOW_THRESHOLD 0x02
#define ADS1X15_REG_HIGH_THRESHOLD 0x03

//  CONFIG REGISTER

//  BIT 15      Operational Status           // 1 << 15
#define ADS1115_OS_BASE 15
#define ADS1X15_OS_BUSY (0 << ADS1115_OS_BASE)
#define ADS1X15_OS_NOT_BUSY (1 << ADS1115_OS_BASE)
#define ADS1X15_OS_START_SINGLE (1 << ADS1115_OS_BASE)

//  BIT 12-14   read differential
#define ADS1115_MUX_BASE 12
#define ADS1X15_MUX_DIFF_0_1 (0 << ADS1115_MUX_BASE)
#define ADS1X15_MUX_DIFF_0_3 (1 << ADS1115_MUX_BASE)
#define ADS1X15_MUX_DIFF_1_3 (2 << ADS1115_MUX_BASE)
#define ADS1X15_MUX_DIFF_2_3 (3 << ADS1115_MUX_BASE)
//              read single
#define ADS1X15_READ_0 (4 << ADS1115_MUX_BASE)
#define ADS1X15_READ_1 (5 << ADS1115_MUX_BASE)
#define ADS1X15_READ_2 (6 << ADS1115_MUX_BASE)
#define ADS1X15_READ_3 (7 << ADS1115_MUX_BASE)

//  BIT 9-11    gain                         //  (0..5) << 9
#define ADS1115_PGA_BASE 9
#define ADS1X15_PGA_6_144V (0 << ADS1115_PGA_BASE)
#define ADS1X15_PGA_4_096V (1 << ADS1115_PGA_BASE)
#define ADS1X15_PGA_2_048V (2 << ADS1115_PGA_BASE)
#define ADS1X15_PGA_1_024V (3 << ADS1115_PGA_BASE)
#define ADS1X15_PGA_0_512V (4 << ADS1115_PGA_BASE)
#define ADS1X15_PGA_0_256V (5 << ADS1115_PGA_BASE)

//  BIT 8       mode                         //  1 << 8
#define ADS1115_MODE_BASE 8
#define ADS1X15_MODE_CONTINUE (0 << ADS1115_MODE_BASE)
#define ADS1X15_MODE_SINGLE (1 << ADS1115_MODE_BASE)

//  BIT 5-7     data rate sample per second  // (0..7) << 5
/*
differs for different devices, check datasheet or readme.md

|  data rate  |  ADS101x  |  ADS111x  |   Notes   |
|:-----------:|----------:|----------:|:---------:|
|     0       |   128     |    8      |  slowest  |
|     1       |   250     |    16     |           |
|     2       |   490     |    32     |           |
|     3       |   920     |    64     |           |
|     4       |   1600    |    128    |  default  |
|     5       |   2400    |    250    |           |
|     6       |   3300    |    475    |           |
|     7       |   3300    |    860    |  fastest  |
*/
#define ADS1115_DATA_RATE_BASE 5
#define ADS1X15_DATA_RATE_8_SPS (0 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_16_SPS (1 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_32_SPS (2 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_64_SPS (3 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_128_SPS (4 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_250_SPS (5 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_475_SPS (6 << ADS1115_DATA_RATE_BASE)
#define ADS1X15_DATA_RATE_860_SPS (7 << ADS1115_DATA_RATE_BASE)

//  BIT 4 comparator modi                    // 1 << 4
#define ADS1X15_COMP_MODE_TRADITIONAL 0x0000
#define ADS1X15_COMP_MODE_WINDOW 0x0010

//  BIT 3 ALERT active value                 // 1 << 3
#define ADS1X15_COMP_POL_ACTIV_LOW 0x0000
#define ADS1X15_COMP_POL_ACTIV_HIGH 0x0008

//  BIT 2 ALERT latching                     // 1 << 2
#define ADS1X15_COMP_NON_LATCH 0x0000
#define ADS1X15_COMP_LATCH 0x0004

//  BIT 0-1 ALERT mode                       // (0..3)
#define ADS1115_COMP_QUE_BASE 0
#define ADS1X15_COMP_QUE_1_CONV (0 << ADS1115_COMP_QUE_BASE) //  trigger alert after 1 convert
#define ADS1X15_COMP_QUE_2_CONV (1 << ADS1115_COMP_QUE_BASE) //  trigger alert after 2 converts
#define ADS1X15_COMP_QUE_4_CONV (2 << ADS1115_COMP_QUE_BASE) //  trigger alert after 4 converts
#define ADS1X15_COMP_QUE_NONE (3 << ADS1115_COMP_QUE_BASE)   //  disable comparator

// _CONFIG masks
//
//  |  bit  |  description           |
//  |:-----:|:-----------------------|
//  |   0   |  # channels            |
//  |   1   |  -                     |
//  |   2   |  resolution            |
//  |   3   |  -                     |
//  |   4   |  GAIN supported        |
//  |   5   |  COMPARATOR supported  |
//  |   6   |  -                     |
//  |   7   |  -                     |
//
#define ADS_CONF_CHAN_1 0x00
#define ADS_CONF_CHAN_4 0x01
#define ADS_CONF_RES_12 0x00
#define ADS_CONF_RES_16 0x04
#define ADS_CONF_NOGAIN 0x00
#define ADS_CONF_GAIN 0x10
#define ADS_CONF_NOCOMP 0x00
#define ADS_CONF_COMP 0x20

void ADC_init(void);
uint8_t ADC_isReady(void);
uint16_t ADC_getValue(void);
void ADC_requestConv(uint8_t channel);