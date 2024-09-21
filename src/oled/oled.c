#include "USI_TWI_Master.h"
#include "oled.h"

#define SH1106_COMMAND 0x00
#define SH1106_DATA 0x40

#define SH1106_LOWCOLADDR 0x00		// 0x00h (0000 XXXX)
#define SH1106_HIGHCOLADDR 0x10		// 0x10h (0001 XXXX)
#define SH1106_PUMPVOLTAGE 0x30		// 0x32h (0011 00XX)
#define SH1106_LINEADDRESS 0x40		// 0x40h (01XX XXXX)
#define SH1106_CONTRASTMODE 0x81	//       (1000 0001)
#define SH1106_CONTRASTVALUE 0x80	// 0x80h (0x00h - 0xFFh) (XXXX XXXX)
#define SH1106_SEGMENTREMAP 0xA0	// 0xA0h (1010 000X) right(0), left(1)
#define SH1106_ENTIREDISPALY 0xA4	// 0xA4h (1010 010X) normal(0), entire(1)
#define SH1106_NORMALORREVERS 0xA6	// 0xA6h (1010 011X) normal(0), reverse(1)
#define SH1106_MULTIPLEXMODE 0xA8	//       (1010 1000)
#define SH1106_MILTIPLEXVALUE 0x3F	// 0x3Fh (00XX XXXX) (0x00h - 0x3Fh)
#define SH1106_DCCONTROLMODE 0xAD	//       (1010 1101)
#define SH1106_DCCONTROLVALON 0x8B	// 0x8Bh (1000 101X) ON(1), OFF(0)
#define SH1106_DISPLAYOFF 0xAE		// 0xAEh (1010 111X) ON(1), OFF(0)
#define SH1106_DISPLAYON 0xAF		// 0xAEh (1010 111X) ON(1), OFF(0)
#define SH1106_PAGEADDRESS 0xB0		// 0xB0h (1011 XXXX)
#define SH1106_OUTPUTSCANDIR 0xC0	// 0xC0h (1100 X000) Scan from COM0 to COM [N- 1] (0) or Scan from COM [N-1] to COM0 (1).
#define SH1106_OFFSETMODE 0xD3		//       (1101 0011)
#define SH1106_OFFSETVALUE 0x00		// 0x00h (00XX XXXX) COM0-63
#define SH1106_DIVRATIOFREQM 0xD5	//       (1101 0101)
#define SH1106_DIVRATIOFREQV 0x50	// 0x50h (FFFF DDDD) Frequency, Divide
#define SH1106_CHARGEMODE 0xD9		//       (1101 1001)
#define SH1106_CHARGEVALUE 0x22		// 0x22h (DDDD PPPP) Dis-charge Period, Pre-charge Period
#define SH1106_COMPADSCONFMODE 0xDA //       (1101 1010)
#define SH1106_COMPADSCONFVAL 0x12	// 0x12h (000X 0010)
#define SH1106_VCOMDESMODE 0xDD		//       (1101 1101)
#define SH1106_VCOMDESVALUE 0x35	// 0x35h (XXXX XXXX) VCOM (Beta x Vref)

void OLED_sendCommand(uint8_t *command, uint8_t size)
{
	uint8_t mode = SH1106_COMMAND;

	USI_TWI_Start((SH1106_ADDR << 1));

	USI_TWI_Write(&mode, 1);
	USI_TWI_Write(command, size);

	USI_TWI_Stop();
}

void OLED_sendData(uint8_t *data, uint8_t size)
{
	uint8_t mode = SH1106_DATA;

	USI_TWI_Start((SH1106_ADDR << 1));

	USI_TWI_Write(&mode, 1);
	USI_TWI_Write(data, size);

	USI_TWI_Stop();
}

void OLED_init(void)
{
	uint8_t display = SH1106_DISPLAYOFF;
	OLED_sendCommand(&display, 1);

	uint8_t config[5] = {0};
	config[0] = SH1106_LOWCOLADDR | 0x02;
	config[1] = SH1106_HIGHCOLADDR | 0x00; // 0x10h (0001 XXXX)
	config[2] = SH1106_PUMPVOLTAGE | 0x02; // 0x32h (0011 00XX)
	config[3] = SH1106_LINEADDRESS | 0x00; // 0x40h (01XX XXXX)
	config[4] = SH1106_PAGEADDRESS | 0x00; // 0xB0h (1011 XXXX)
	OLED_sendCommand(config, 5);

	config[0] = SH1106_CONTRASTMODE;
	config[1] = SH1106_CONTRASTVALUE;		  //       (1000 0001)  // 0x80h (0x00h - 0xFFh) (XXXX XXXX)
	config[2] = SH1106_SEGMENTREMAP | 0x01;	  // 0xA0h (1010 000X) right(0), left(1)
	config[3] = SH1106_ENTIREDISPALY | 0x00;  // 0xA4h (1010 010X) normal(0), entire(1)
	config[4] = SH1106_NORMALORREVERS | 0x00; // 0xA6h (1010 011X) normal(0), reverse(1)
	OLED_sendCommand(config, 5);

	config[0] = SH1106_MULTIPLEXMODE;
	config[1] = SH1106_MILTIPLEXVALUE; //       (1010 1000)  // 0x3Fh (00XX XXXX) (0x00h - 0x3Fh)
	config[2] = SH1106_DCCONTROLMODE;
	config[3] = SH1106_DCCONTROLVALON;		 //       (1010 1101)  // 0x8Bh (1000 101X) ON(1), OFF(0)
	config[4] = SH1106_OUTPUTSCANDIR | 0x08; // 0xC0h (1100 X000) Scan from COM0 to COM [N- 1] (0x00) or Scan from COM [N-1] to COM0 (0x08)
	OLED_sendCommand(config, 5);

	config[0] = SH1106_OFFSETMODE;
	config[1] = SH1106_OFFSETVALUE; //       (1101 0011)  // 0x00h (00XX XXXX) COM0-63
	config[2] = SH1106_DIVRATIOFREQM;
	config[3] = SH1106_DIVRATIOFREQV; //       (1101 0101)  // 0x50h (FFFF DDDD) Frequency, Divide
	config[4] = SH1106_CHARGEMODE;
	OLED_sendCommand(config, 5);

	config[0] = SH1106_CHARGEVALUE; //       (1101 1001)  // 0x22h (DDDD PPPP) Dis-charge Period, Pre-charge Period
	config[1] = SH1106_COMPADSCONFMODE;
	config[2] = SH1106_COMPADSCONFVAL; //       (1101 1010)  // 0x12h (000X 0010)
	config[3] = SH1106_VCOMDESMODE;
	config[4] = SH1106_VCOMDESVALUE; //       (1101 1101)  // 0x35h (XXXX XXXX) VCOM (Beta x Vref)
	OLED_sendCommand(config, 5);

	display = SH1106_DISPLAYON;
	OLED_sendCommand(&display, 1);
}

void OLED_clear_display(void)
{
	uint8_t data[8] = {0};
	uint8_t config[3] = {
		0,
		SH1106_LOWCOLADDR | 0x02,
		SH1106_HIGHCOLADDR | 0x00,
	};

	for (uint8_t page = 0; page < 8; page++)
	{
		config[0] = SH1106_PAGEADDRESS | page;
		OLED_sendCommand(config, 3);

		for (uint8_t i = 0; i < 16; i++)
		{
			OLED_sendData(data, 8);
		}
	}
}

void OLED_display_xx(glyph_t *glyph, uint8_t const *chr)
{

	uint8_t _bitmap[glyph->width];

	uint8_t low_col = (glyph->col + 2) & 0x0F;
	uint8_t high_col = ((glyph->col + 2) >> 4) & 0x0F;
	uint8_t config[3] = {
		0,
		SH1106_LOWCOLADDR | low_col,
		SH1106_HIGHCOLADDR | high_col,
	};

	for (uint8_t _page = glyph->page; _page < glyph->page + glyph->height / 8; _page++)
	{
		config[0] = SH1106_PAGEADDRESS | _page;
		OLED_sendCommand(config, 3);

		for (uint8_t i = 0; i < glyph->width; i++)
		{
			_bitmap[i] = pgm_read_byte(&chr[i + (_page - glyph->page) * glyph->width]);
		}

		OLED_sendData(_bitmap, glyph->width);
	}
}

void OLED_displayNumber(uint8_t *number, glyph_t *glyph)
{
	for (uint8_t i = 0; i < glyph[0].size; i++)
	{
		if (number[i] != glyph[i].chr)
		{
			glyph[i].chr = number[i];

			switch (glyph[i].height)
			{
			case SMALLFONT_HEIGHT:
				OLED_display_xx(&glyph[i], smallDigits_8ptBitmaps[glyph[i].chr]);
				break;
			case MEDIUMFONT_HEIGHT:
				OLED_display_xx(&glyph[i], verdana_12ptBitmaps[glyph[i].chr]);
				break;
			case BIGFONT_HEIGHT:
				OLED_display_xx(&glyph[i], cascadiaCode_22ptBitmaps[glyph[i].chr]);
				break;

			default:
				break;
			}
		}
	}
}

void OLED_displayCharacter(BM_chr_t chr, uint8_t page, uint8_t col)
{
	glyph_t glyph = {.chr = chr, .page = page, .col = col, .width = 5, .height = 8};
	OLED_display_xx(&glyph, smallChars_8ptBitmaps[chr]);
}