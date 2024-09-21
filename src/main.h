#pragma once


#define MAX_VOLTAGE 1200
#define MAX_CURRENT 1000
#define VOLT_STEP 5
#define CURR_STEP 4

#define WIPER_VOLTAGE 0
#define WIPER_CURRENT 1

#define ENC_A_READPIN (PINB & (1 << PB2))
#define ENC_B_READPIN (PINA & (1 << PA7))
#define MODE_READPIN (PINB & (1 << PB0))
#define OUT_READPIN (PINA & (1 << PA5))
#define TOGGLE_OUTPUT (PORTB ^= (1 << PB1))
#define DISABLE_OUTPUT (PORTB &= ~(1 << PB1))
#define ENABLE_OUTPUT (PORTB |= (1 << PB1))
#define TOGGLE_VCC (PORTA ^= (1 << PA0))
#define DISABLE_VCC (PORTA &= ~(1 << PA0))
#define ENABLE_VCC (PORTA |= (1 << PA0))
#define LEDMODE_CV PORTA &= ~(1 << PA3)
#define LEDMODE_CC PORTA |= (1 << PA3)