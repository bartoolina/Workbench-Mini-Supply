#include "USI_TWI_Master.h"
#include "mcp4651.h"

#define MCP4651_WPIER_0 (0 << 4)
#define MCP4651_WPIER_1 (1 << 4)
#define MCP4651_TCON_REG (4 << 4)

#define MCP4651_WRITE (0 << 2)
#define MCP4651_READ (1 << 2)
#define MCP4651_INC (2 << 2)
#define MCP4651_DEC (3 << 2)

#define MCP4651_ADDR 0x2F

void POT_init(void)
{
    uint8_t config[2] = {MCP4651_TCON_REG, 0xFF};
    USI_TWI_Start(MCP4651_ADDR << 1);
    USI_TWI_Write(config, 2);
    USI_TWI_Stop();
}

void POT_setWpier(uint16_t value, uint8_t chan)
{
    USI_TWI_Start(MCP4651_ADDR << 1);
    // uint16_t data = (MCP4651_WPIER_0 << 8) | (MCP4651_WRITE << 8) | (value & 0x1FF);
    uint8_t data[2] = {
        (chan ? MCP4651_WPIER_1 : MCP4651_WPIER_0) | MCP4651_WRITE | ((value >> 8) & 0x01),
        value & 0xFF,
    };
    USI_TWI_Write(data, 2);
    USI_TWI_Stop();
}