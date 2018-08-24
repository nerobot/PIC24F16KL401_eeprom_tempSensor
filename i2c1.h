/*! @file i2c1.h
 * 
 * @brief Library for controlling the i2c1 module (MSSP based)
 * 
 * @author Steven Daglish
 * 
 * @date 26 July 2018
 */

#ifndef I2C1_H
#define	I2C1_H

#ifdef	__cplusplus
extern "C" {
#endif
    
// Oscillator config
#define XTAL_FREQ       8000000UL               // Raw oscillator freq
#define FCY             XTAL_FREQ / 2           // Most but not all PIC24s
    
#define FCLOCK 100000                               //! Desired I2C clock frequency
#define SSPADD (FCY / (2* FCLOCK)) - 1;
    
#define true            1
#define false           0
#define bool            uint8_t
    
#define READ_CMD        1
#define WRITE_CMD       0

void i2cInit();                                     //! Initialises the i2c (1) module
void i2cReady();
void i2c_Wait();
uint8_t i2cAddress(uint8_t address, uint8_t mode);  //! Send the address (+ R/W bit) byte. Comes after start, but before write / read
uint8_t i2cWrite(uint8_t data);                     //! Send the data byte and wait for acknowledgement
uint8_t i2cStart();                                 //! Send the start bit
void i2cRestart();                                  
uint8_t i2cStop();                                  //! Send the stop bit
void i2cAck();
void i2cNack();
uint8_t i2cRead(uint8_t ack);                       //! Read a single byte from the slave

#ifdef	__cplusplus
}
#endif

#endif	/* I2C1_H */

