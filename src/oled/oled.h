#pragma once

#include "bitmap.h"

#define SH1106_ADDR 0x3c

typedef struct
{
    const uint8_t width;
    const uint8_t height;
    const uint8_t col;
    const uint8_t page;
    const uint8_t size;
    uint8_t chr;
} glyph_t;

void OLED_init(void);
void OLED_clear_display(void);
// void OLED_display(uint8_t page, uint8_t col, bitmap_t bitmap_name);
// void OLED_display_x(uint8_t page, uint8_t col, bitmap_t bitmap_name, uint8_t width);
// void OLED_display_8pt(uint8_t *buffor, glyph_t *glyph);
// void OLED_display_24pt(uint8_t *buffor, glyph_t *glyph);
void OLED_displayNumber(uint8_t *number, glyph_t *glyph);
void OLED_displayCharacter(BM_chr_t chr, uint8_t page, uint8_t col);