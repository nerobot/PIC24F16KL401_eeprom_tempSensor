/* 
 * File:   mcp9808.h
 * Author: stevend
 *
 * Created on 23 July 2018, 07:53
 */

#ifndef MCP9808_H
#define	MCP9808_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MCP9808_I2CADDR_DEFAULT        0x18
#define MCP9808_REG_CONFIG             0x01

#define MCP9808_REG_CONFIG_SHUTDOWN    0x0100
#define MCP9808_REG_CONFIG_CRITLOCKED  0x0080
#define MCP9808_REG_CONFIG_WINLOCKED   0x0040
#define MCP9808_REG_CONFIG_INTCLR      0x0020
#define MCP9808_REG_CONFIG_ALERTSTAT   0x0010
#define MCP9808_REG_CONFIG_ALERTCTRL   0x0008
#define MCP9808_REG_CONFIG_ALERTSEL    0x0004
#define MCP9808_REG_CONFIG_ALERTPOL    0x0002
#define MCP9808_REG_CONFIG_ALERTMODE   0x0001

#define MCP9808_REG_UPPER_TEMP         0x02
#define MCP9808_REG_LOWER_TEMP         0x03
#define MCP9808_REG_CRIT_TEMP          0x04
#define MCP9808_REG_AMBIENT_TEMP       0x05
#define MCP9808_REG_MANUF_ID           0x06
#define MCP9808_REG_DEVICE_ID          0x07


//void mcpShutdown_wake( uint8_t sw_ID );     //! Wakes up or shuts-down the module dependig on sw_ID. Internal.
void mcpShutdown(void);                     //! Puts the MCP module into shutdown mode
void mcpWake(void);                         //! Wakes up the MCP module
uint8_t initMCP();                          //! Initialise the MCP module
uint16_t readTemp( void );                  //! Read the current temperature and return as a word (two bytes)
float readTempC( void );                    //! Read the current temperature and return as a float
//static void write16(uint8_t address, uint16_t data);
//static uint16_t read16(uint8_t address);

#ifdef	__cplusplus
}
#endif

#endif	/* MCP9808_H */

