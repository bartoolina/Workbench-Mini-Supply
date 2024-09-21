#pragma once
#include <stdint.h>
#include <avr/pgmspace.h>

#define SMALLFONT_WIDTH 5
#define MEDIUMFONT_WIDTH 9
#define BIGFONT_WIDTH 15
#define SMALLFONT_HEIGHT 8
#define MEDIUMFONT_HEIGHT 16
#define BIGFONT_HEIGHT 24

typedef enum
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    NULL_CHR,
    BM_NUM_MAX,
} BM_num_t;

typedef enum
{
    COMA,
    BIG_COMA,
    V,
    A,
    C,
    DEGREE,
    EREASE,
    BM_CHR_MAX
} BM_chr_t;

extern const uint8_t smallDigits_8ptBitmaps[BM_NUM_MAX][SMALLFONT_WIDTH] PROGMEM;
extern const uint8_t smallChars_8ptBitmaps[BM_CHR_MAX][SMALLFONT_WIDTH] PROGMEM;
extern const uint8_t cascadiaCode_22ptBitmaps[BM_NUM_MAX][BIGFONT_WIDTH * 3] PROGMEM;
extern const uint8_t verdana_12ptBitmaps[BM_NUM_MAX][BIGFONT_WIDTH * 2] PROGMEM;
