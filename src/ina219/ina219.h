#pragma once

uint16_t INA_getShuntVoltage(void);
uint16_t INA_getBasVoltage(void);
uint16_t INA_getCurrent(void);
uint16_t getPower_mW(void);
void INA_init(void);