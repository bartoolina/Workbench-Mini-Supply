#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "USI_TWI_Master.h"
#include "oled.h"
#include "ads1115.h"
#include "mcp4651.h"
#include "ina219.h"
#include "main.h"

/*
PA0	-	(OUT)	VCC_ONOFF
PA1	-	(AIN0)	OUT_I*
PA2	-	(AIN1)	OUT_U*
PA3	-	(OUT)	CV/CC_LED*
PA4	-	(I2C)	SCL*
PA5	-	(IN)	BTN_OUT*
PA6	-	(I2C)	SDA*
PA7	-	(IN)	ENC_B*
PB0	-	(IN)	BTN_MODE*
PB1	-	(OUT)	OUT_ONOFF*
PB2	-	(INT0)	ENC_A*
*/

#define DIGITS_2 2
#define DIGITS_3 3
#define DIGITS_4 4
#define FIRST_DIGIT 0
#define SECOND_DIGIT 1



typedef enum
{
	VOLTAGE,
	CURRENT,
} curr_mode_t;

typedef enum
{
	NTC_RES,
	NTC_TRAN,
	NTC_MAX,
} res_type_t;

typedef struct
{
	uint16_t set_voltage;
	uint16_t set_current;
	uint16_t out_voltage;
	uint16_t out_current;
	uint16_t batt_voltage;
	uint16_t vcc_voltage;
	uint16_t res_temp;
	uint16_t tran_temp;

} app_data_t;

typedef struct
{
	uint8_t temp;
	uint16_t res[NTC_MAX];
} temp_t;

#define CONVTABLE_SIZE 19
temp_t conv_table[CONVTABLE_SIZE] = {
	{5, {2265, 2592}},
	{10, {1830, 2017}},
	{15, {1487, 1583}},
	{20, {1216, 1253}},
	{25, {1000, 1000}},
	{30, {827, 803}},
	{35, {687, 650}},
	{40, {574, 530}},
	{45, {481, 434}},
	{50, {406, 358}},
	{55, {344, 297}},
	{60, {293, 248}},
	{65, {250, 208}},
	{70, {215, 176}},
	{75, {185, 149}},
	{80, {160, 127}},
	{85, {139, 108}},
	{90, {121, 93}},
	{95, {106, 80}},
};
// glyph_t gl_test6_1[6] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 0, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 7, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 14, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 21, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 28, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 35, .chr = 10, .size = 6},
// };

// glyph_t gl_test6_2[6] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 45, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 52, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 59, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 66, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 73, .chr = 10, .size = 6},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 80, .chr = 10, .size = 6},
// };
// glyph_t gl_test4_1[4] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 0, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 7, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 14, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 21, .chr = 10, .size = 4},
// };
// glyph_t gl_test4_2[4] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 31, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 38, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 45, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 52, .chr = 10, .size = 4},
// };
// glyph_t gl_test4_3[4] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 62, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 69, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 76, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 83, .chr = 10, .size = 4},
// };
// glyph_t gl_test4_4[4] = {
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 93, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 100, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 107, .chr = 10, .size = 4},
// 	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 114, .chr = 10, .size = 4},
// };

glyph_t gl_res[DIGITS_2] = {
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 0, .chr = 10, .size = DIGITS_2},
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 7, .chr = 10, .size = DIGITS_2},
};
glyph_t gl_tran[DIGITS_2] = {
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 33, .chr = 10, .size = DIGITS_2},
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 40, .chr = 10, .size = DIGITS_2},
};
glyph_t gl_vcc[DIGITS_3] = {
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 97, .chr = 10, .size = DIGITS_3},
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 104, .chr = 10, .size = DIGITS_3},
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 116, .chr = 10, .size = DIGITS_3},
};
glyph_t gl_batt[DIGITS_2] = {
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 67, .chr = 10, .size = DIGITS_2},
	{.width = SMALLFONT_WIDTH, .height = SMALLFONT_HEIGHT, .page = 0, .col = 79, .chr = 10, .size = DIGITS_2},
};
glyph_t gl_volt_set[DIGITS_4] = {
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 2, .col = 80, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 2, .col = 91, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 2, .col = 109, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 2, .col = 120, .chr = 10, .size = DIGITS_4},
};
glyph_t gl_amper_set[DIGITS_4] = {
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 5, .col = 80, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 5, .col = 98, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 5, .col = 109, .chr = 10, .size = DIGITS_4},
	{.width = MEDIUMFONT_WIDTH, .height = MEDIUMFONT_HEIGHT, .page = 5, .col = 120, .chr = 10, .size = DIGITS_4},
};
glyph_t gl_volt_curr[DIGITS_4] = {
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 2, .col = 0, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 2, .col = 17, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 2, .col = 42, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 2, .col = 59, .chr = 10, .size = DIGITS_4},
};
glyph_t gl_amper_curr[DIGITS_4] = {
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 5, .col = 0, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 5, .col = 25, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 5, .col = 42, .chr = 10, .size = DIGITS_4},
	{.width = BIGFONT_WIDTH, .height = BIGFONT_HEIGHT, .page = 5, .col = 59, .chr = 10, .size = DIGITS_4},
};

volatile uint8_t soft_timer;
volatile uint8_t encoder;
volatile uint8_t comparator;

curr_mode_t curr_mode;
app_data_t app_data;

void timer_init(void);
void encoder_init(void);
void setOutputVoltage(int8_t inc);
void setOutputCurrent(int8_t inc);
uint8_t *decToBcdArray(uint16_t number, uint8_t size, uint8_t first_digit);
void handleEncoderChange(void);
void handleOutBtnPress(void);
void handleModeBtnPress(void);
void performMeasurementOutput(void);
void performMeasurementCycle(void);
void fillDisplay(void);
void heartBeepTimer(void);
void findNearest(uint16_t target, temp_t **low_tresh, temp_t **high_tresh, res_type_t res_type);

int main(void)
{
	// Clear WDRF in MCUSR
	MCUSR = 0x00;
	// Write logical one to WDCE and WDE
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	// Turn off WDT
	WDTCSR = 0x00;
	wdt_reset();

	DDRB |= (1 << PB1);
	DISABLE_OUTPUT;
	DDRA |= (1 << PA3);
	PORTA |= (1 << PA3);

	app_data.set_voltage = 500;
	app_data.set_current = 100;

	timer_init();
	encoder_init();
	USI_TWI_Init();
	OLED_init();
	OLED_clear_display();
	ADC_init();
	POT_init();
	INA_init();

	setOutputVoltage(0);
	setOutputCurrent(0);
	wdt_enable(WDTO_2S);
	sei();
	// uint8_t *_bcd_ptr;
	// for (uint8_t i = 73; i < 127; i++)
	// {
	// 	if (USI_TWI_Start(i << 1))
	// 	{
	// 		_bcd_ptr = decToBcdArray(i, DIGITS_4, FIRST_DIGIT);
	// 		OLED_displayNumber(_bcd_ptr, gl_test4_1);
	// 		while (OUT_READPIN)
	// 		{
	// 			wdt_reset();
	// 		}
	// 	}
	// 	USI_TWI_Stop();
	// }

	fillDisplay();

	while (1)
	{
		handleModeBtnPress();
		handleOutBtnPress();
		handleEncoderChange();
		performMeasurementOutput();
		performMeasurementCycle();

		heartBeepTimer();
		wdt_reset();
	}
}

void timer_init(void)
{
	// mode CTC
	TCCR0A |= (1 << WGM01);
	// 100Hz
	OCR0A = F_CPU / 1024 / 100;
	// prescaler 1024
	TCCR0B |= (1 << CS02) | (1 << CS00);
	// enable interupt
	TIMSK0 |= (1 << OCIE0A);
}

void encoder_init(void)
{
	// any logic change on INT0
	MCUCR |= (1 << ISC00);
	// INT0 enable
	GIMSK |= (1 << INT0);
}

void setOutputVoltage(int8_t inc)
{
	int16_t _new_set_voltage = (app_data.set_voltage + inc);
	if (_new_set_voltage < 0)
	{
		_new_set_voltage = 0;
	}

	if (_new_set_voltage > MAX_VOLTAGE)
	{
		_new_set_voltage = MAX_VOLTAGE;
	}

	app_data.set_voltage = _new_set_voltage;
	POT_setWpier((UINT8_MAX * _new_set_voltage) / MAX_VOLTAGE, VOLTAGE);
}

void setOutputCurrent(int8_t inc)
{
	int16_t _new_set_curr = (int16_t)(app_data.set_current + inc);
	if (_new_set_curr < 4)
	{
		_new_set_curr = 4;
	}

	if (_new_set_curr > MAX_CURRENT)
	{
		_new_set_curr = MAX_CURRENT;
	}

	app_data.set_current = (uint16_t)_new_set_curr;
	POT_setWpier(((UINT8_MAX * (app_data.set_current - 4)) / MAX_CURRENT), CURRENT);
}

uint8_t *decToBcdArray(uint16_t number, uint8_t size, uint8_t first_digit)
{
    const uint16_t pow10[] = {1000, 100, 10, 1};
    static uint8_t output[4] = {0};
    uint8_t chr;

    for (uint8_t i = 0; i < size; i++)
    {
        chr = number / pow10[i + first_digit];
        output[i] = (chr > 9) ? 10 : chr;
        number %= pow10[i + first_digit];
    }
    return output;
}
// uint8_t *_decToBcdArray(uint32_t number, uint8_t size, uint8_t first_digit)
// {
// 	const uint32_t _pow10[] = {100000, 10000, 1000, 100, 10, 1};
// 	static uint8_t _output[6] = {0};

// 	for (uint8_t i = 0; i < size; i++)
// 	{
// 		_output[i] = number / _pow10[i + first_digit];
// 		number = number % _pow10[i + first_digit];
// 	}
// 	return _output;
// }

void handleEncoderChange(void)
{
	static uint8_t _prev_enc = 0;
	int8_t _diff_enc;
	uint8_t *_bcd_ptr;

	if (_prev_enc != encoder)
	{
		_diff_enc = (int8_t)(encoder - _prev_enc);
		_prev_enc = encoder;
		switch (curr_mode)
		{
		case VOLTAGE:
			setOutputVoltage(_diff_enc * VOLT_STEP);
			_bcd_ptr = decToBcdArray(app_data.set_voltage, DIGITS_4, FIRST_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_volt_set);

			break;
		case CURRENT:
			setOutputCurrent(_diff_enc * CURR_STEP);
			_bcd_ptr = decToBcdArray(app_data.set_current, DIGITS_4, FIRST_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_amper_set);

			break;
		default:
			break;
		}
	}
}

void handleOutBtnPress(void)
{
	enum
	{
		IDLE,
		DEBIUNCE,
		PRESSED,
		LONG_PRESSED
	};

	static uint8_t _btn_state = IDLE;
	static uint8_t _btn_timer = 0;

	if (!OUT_READPIN)
	{
		switch (_btn_state)
		{
		case IDLE:
			_btn_timer = soft_timer;
			_btn_state = DEBIUNCE;
			break;
		case DEBIUNCE:
			if ((uint8_t)(soft_timer - _btn_timer) > 30u)
			{
				_btn_state = PRESSED;
			}
			break;
		case PRESSED:
			_btn_state = LONG_PRESSED;
			TOGGLE_OUTPUT;
			break;
		case LONG_PRESSED:
			break;

		default:
			break;
		}
	}
	else
	{
		_btn_state = IDLE;
	}
}

void handleModeBtnPress(void)
{
	if (MODE_READPIN)
	{
		curr_mode = CURRENT;
		OLED_displayCharacter(V, 4, 123);
	}
	else
	{
		curr_mode = VOLTAGE;
		OLED_displayCharacter(A, 7, 123);
	}
}

void performMeasurementOutput(void)
{
	enum
	{
		INA_VOLTAGE,
		INA_CURRENT,
		INA_REQUEST,
	};

	static uint8_t _prev_timer = 0;
	static uint8_t _ina_state = INA_VOLTAGE;
	uint8_t *_bcd_ptr;
	uint16_t _bus_voltage = 0, _cur_amper = 0;

	switch (_ina_state)
	{
	case INA_VOLTAGE:
		if ((uint8_t)(soft_timer - _prev_timer) > 5u)
		{
			_bus_voltage = INA_getBasVoltage();
			app_data.out_voltage = _bus_voltage - (app_data.out_current / 10);

			if (app_data.out_voltage > 9999)
			{
				app_data.out_voltage = 0;
			}

			_bcd_ptr = decToBcdArray(app_data.out_voltage, DIGITS_4, FIRST_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_volt_curr);

			// _bcd_ptr = _decToBcdArray(_bus_voltage, 6, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test6_1);

			_prev_timer = soft_timer;
			_ina_state = INA_CURRENT;
		}

		break;
	case INA_CURRENT:
		if ((uint8_t)(soft_timer - _prev_timer) > 5u)
		{
			_cur_amper = INA_getCurrent();
			app_data.out_current = _cur_amper; //- (app_data.out_voltage / 300);

			if (app_data.out_current > 5999)
			{
				app_data.out_current = 0;
			}

			_bcd_ptr = decToBcdArray(app_data.out_current, DIGITS_4, FIRST_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_amper_curr);

			// _bcd_ptr = _decToBcdArray(_cur_amper, 6, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test6_2);

			_prev_timer = soft_timer;
			_ina_state = INA_VOLTAGE;
		}

		break;

	default:
		_prev_timer = soft_timer;
		_ina_state = INA_VOLTAGE;
		break;
	}
}

void performMeasurementCycle(void)
{
	enum
	{
		ADCCHAN_TRA,
		ADCCHAN_RES,
		ADCCHAN_BAT,
		ADCCHAN_VCC,
		ADC_REQUEST,
	};

	static uint8_t _adc_state = ADC_REQUEST;
	static uint8_t _adc_chann = 3;
	uint8_t *_bcd_ptr;

	switch (_adc_state)
	{
	case ADCCHAN_BAT:
		if (ADC_isReady())
		{
			_adc_state = ADC_REQUEST;
			app_data.batt_voltage = ((ADC_getValue() >> 4) * 855UL) / 2048U; // max 512
			// app_data.batt_voltage = 850;									// conversion to human value ( 10 -> 0,1 )

			_bcd_ptr = decToBcdArray(app_data.batt_voltage, DIGITS_2, SECOND_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_batt);
		}
		break;
	case ADCCHAN_VCC:
		if (ADC_isReady())
		{
			_adc_state = ADC_REQUEST;
			app_data.vcc_voltage = ((ADC_getValue() >> 4) * 1490UL) / 2048U; // max 2048
			// app_data.vcc_voltage = 1200;									// conversion to human value ( 20 -> 00,2 )

			_bcd_ptr = decToBcdArray(app_data.vcc_voltage, DIGITS_3, FIRST_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_vcc);
		}
		break;
	case ADCCHAN_RES:
		if (ADC_isReady())
		{
			_adc_state = ADC_REQUEST;
			uint16_t adc_value = (ADC_getValue() >> 4);
			uint16_t ntc_value = ((15000UL * adc_value) / (5000U - adc_value)) / 10;

			// _bcd_ptr = _decToBcdArray(ntc_value, 6, FIRST_DIGIT);
			// _bcd_ptr = decToBcdArray(ntc_value, DIGITS_4, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test4_1);

			temp_t *low_tresh, *high_tresh;
			findNearest(ntc_value, &low_tresh, &high_tresh, NTC_RES);

			uint8_t fraction = (10 * (high_tresh->res[NTC_RES] - ntc_value)) / (high_tresh->res[NTC_RES] - low_tresh->res[NTC_RES]);
			app_data.res_temp = ((low_tresh->temp * fraction) + (high_tresh->temp * (10 - fraction)));

			// _bcd_ptr = decToBcdArray(low_tresh->res[NTC_RES], DIGITS_4, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test4_2);

			// _bcd_ptr = decToBcdArray(high_tresh->res[NTC_RES], DIGITS_4, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test4_3);

			// _bcd_ptr = decToBcdArray(app_data.res_temp, DIGITS_4, FIRST_DIGIT);
			// OLED_displayNumber(_bcd_ptr, gl_test4_4);

			_bcd_ptr = decToBcdArray(app_data.res_temp, DIGITS_2, SECOND_DIGIT);

			OLED_displayNumber(_bcd_ptr, gl_res);
		}
		break;
	case ADCCHAN_TRA:
		if (ADC_isReady())
		{
			_adc_state = ADC_REQUEST;
			uint16_t adc_value = (ADC_getValue() >> 4);
			uint16_t ntc_value = ((15000UL * adc_value) / (5000U - adc_value)) / 10;

			temp_t *low_tresh, *high_tresh;
			findNearest(ntc_value, &low_tresh, &high_tresh, NTC_TRAN);

			uint8_t fraction = (10 * (high_tresh->res[NTC_TRAN] - ntc_value)) / (high_tresh->res[NTC_TRAN] - low_tresh->res[NTC_TRAN]);
			app_data.tran_temp = ((low_tresh->temp * fraction) + (high_tresh->temp * (10 - fraction)));

			_bcd_ptr = decToBcdArray(app_data.tran_temp, DIGITS_2, SECOND_DIGIT);
			OLED_displayNumber(_bcd_ptr, gl_tran);
		}
		break;

	default:
		_adc_chann = (_adc_chann + 1) & 0x03;
		_adc_state = _adc_chann;
		ADC_requestConv(_adc_chann);
		break;
	}
}

void findNearest(uint16_t target, temp_t **low_tresh, temp_t **high_tresh, res_type_t res_type)
{
	uint8_t mid, left = 0, right = CONVTABLE_SIZE - 1;

	// if (conv_table[left].res[res_type] == target)
	// {
	// 	*low_tresh = &conv_table[left];
	// 	*high_tresh = &conv_table[left];
	// 	return;
	// }

	// if (conv_table[right].res[res_type] == target)
	// {
	// 	*low_tresh = &conv_table[right];
	// 	*high_tresh = &conv_table[right];
	// 	return;
	// }

	while ((right - left) > 1)
	{
		mid = (right + left + 1) / 2;
		// if (conv_table[mid].res[res_type] == target)
		// {
		// 	*low_tresh = &conv_table[mid];
		// 	*high_tresh = &conv_table[mid];
		// 	return;
		// } else
		if (conv_table[mid].res[res_type] < target)
		{
			right = mid;
		}
		else
		{
			left = mid;
		}
	}

	*low_tresh = &conv_table[right];
	*high_tresh = &conv_table[left];
}

void fillDisplay(void)
{
	uint8_t *_bcd_ptr;

	// Measure Voltage
	OLED_displayCharacter(BIG_COMA, 4, 35);
	_bcd_ptr = decToBcdArray(app_data.out_voltage, DIGITS_4, FIRST_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_volt_curr);

	// Measure Current
	OLED_displayCharacter(BIG_COMA, 7, 18);
	_bcd_ptr = decToBcdArray(app_data.out_current, DIGITS_4, FIRST_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_amper_curr);

	// Set Voltage
	OLED_displayCharacter(COMA, 3, 101);
	_bcd_ptr = decToBcdArray(app_data.set_voltage, DIGITS_4, FIRST_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_volt_set);

	// Set Current
	OLED_displayCharacter(COMA, 6, 90);
	_bcd_ptr = decToBcdArray(app_data.set_current, DIGITS_4, FIRST_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_amper_set);

	// Measure VCC
	OLED_displayCharacter(V, 0, 123);
	OLED_displayCharacter(COMA, 0, 110);
	_bcd_ptr = decToBcdArray(app_data.vcc_voltage, DIGITS_3, FIRST_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_vcc);

	// Measure Battery Voltage
	OLED_displayCharacter(V, 0, 86);
	OLED_displayCharacter(COMA, 0, 73);
	_bcd_ptr = decToBcdArray(app_data.batt_voltage, DIGITS_2, SECOND_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_batt);

	// Measure Resistor Temperature
	OLED_displayCharacter(DEGREE, 0, 14);
	OLED_displayCharacter(C, 0, 21);
	_bcd_ptr = decToBcdArray(app_data.res_temp, DIGITS_2, SECOND_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_res);

	// Measure Transistor Temperature
	OLED_displayCharacter(DEGREE, 0, 47);
	OLED_displayCharacter(C, 0, 54);
	_bcd_ptr = decToBcdArray(app_data.tran_temp, DIGITS_2, SECOND_DIGIT);
	OLED_displayNumber(_bcd_ptr, gl_tran);

	// Voltage and Current Indicators
	OLED_displayCharacter(V, 4, 123);
	OLED_displayCharacter(A, 7, 123);
}

void heartBeepTimer(void)
{
	static uint8_t _prev_timer = 0;
	static uint8_t _toggle = 1;

	if ((uint8_t)(soft_timer - _prev_timer) > 50u)
	{
		_prev_timer = soft_timer;
		PORTA ^= (1 << PA3);
		switch (curr_mode)
		{
		case CURRENT:
		{
			if (_toggle)
			{
				OLED_displayCharacter(A, 7, 123);
				_toggle = 0;
			}
			else
			{
				OLED_displayCharacter(EREASE, 7, 123);
				_toggle = 1;
			}
			break;
		}
		case VOLTAGE:
		{
			if (_toggle)
			{
				OLED_displayCharacter(V, 4, 123);
				_toggle = 0;
			}
			else
			{
				OLED_displayCharacter(EREASE, 4, 123);
				_toggle = 1;
			}
			break;
		}

		default:
			break;
		}
	}
}

ISR(TIM0_COMPA_vect)
{
	soft_timer++;
}

ISR(EXT_INT0_vect)
{
	static uint8_t dir;

	if (!(ENC_A_READPIN))
	{
		dir = (ENC_B_READPIN);
	}
	else
	{
		if (dir != (ENC_B_READPIN))
		{
			if (dir)
			{
				encoder--;
			}
			else
			{
				encoder++;
			}
		}
	}
}

// ISR(ANA_COMP_vect)
// {
// 	comparator = ACSR & (1 << ACO);
// 	if (comparator)
// 	{
// 		PORTA |= (1 << PA3);
// 	}
// 	else
// 	{
// 		PORTA &= ~(1 << PA3);
// 	}
// }