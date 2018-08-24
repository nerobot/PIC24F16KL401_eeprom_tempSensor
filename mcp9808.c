#include <xc.h>
#include "mcp9808.h"
#include "i2c1.h"

static void write16(uint8_t address, uint16_t data){
    i2cStart();
    
    i2cAddress(MCP9808_I2CADDR_DEFAULT, WRITE_CMD);
    
    i2cWrite(address);
    
    i2cWrite(data >> 8);
    i2cWrite(data & 0xFF);
    
    i2cStop();
}

static uint16_t read16(uint8_t address){
    uint16_t buffer;
    
    i2cStart();
    
    i2cAddress(MCP9808_I2CADDR_DEFAULT, WRITE_CMD);
    i2cWrite(address);
    i2cRestart();
    
    i2cAddress(MCP9808_I2CADDR_DEFAULT, READ_CMD);
    buffer = (i2cRead(1) << 8);
    buffer += i2cRead(0);
    
    i2cStop();
    return buffer;
}

static void mcpShutdown_wake( uint8_t sw_ID )
{
    uint16_t conf_shutdown ;
    uint16_t conf_register = read16(MCP9808_REG_CONFIG);
    if (sw_ID == 1)
    {
       conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN ;
       write16(MCP9808_REG_CONFIG, conf_shutdown);
    }
    if (sw_ID == 0)
    {
       conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN ;
       write16(MCP9808_REG_CONFIG, conf_shutdown);
    }
}

void mcpShutdown(void)
{
  mcpShutdown_wake(1);
}

void mcpWake(void)
{
  mcpShutdown_wake(0);
//  __delay_ms(250);
}

uint8_t initMCP(){
    if (read16(MCP9808_REG_MANUF_ID) != 0x0054) return 0;
    if (read16(MCP9808_REG_DEVICE_ID) != 0x0400) return 0;
    
    write16( MCP9808_REG_CONFIG, 0x0);
    return 1;
}

uint16_t readTemp( void )
{
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);
  return t;
}

float readTempC( void )
{
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  float temp = t & 0x0FFF;
  temp /=  16.0;
  if (t & 0x1000) temp -= 256;

  return temp;
}
