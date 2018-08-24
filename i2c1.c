#include <xc.h>
#include "i2c1.h"
#include <libpic30.h>

void i2cInit()
{     
    // Setting up SSP Control Register 1 (SSPCON1)
        // I2C Mode Configuration
    SSP1CON1bits.SSPEN = 1;         // Enables the serial port and configures the SDAx and SCLx pins as the serial port pins    
    SSP1CON1bits.SSPM = 0b1000;     // I2C Master mode
    
    // Setting up SSP Address Register (SSPADD<6:0>)
        // I2C Bit Rate
    SSP1ADDbits.I2CADD = SSPADD;      //0x27;      // Focs = 8MHz, Gclock = 100kHz
    
    // Setting up SSP Status Register (SSPSTAT)
        // Slew Rate Control
        // Input Pin Threshold Levels (SMbus or I2C)
    SSP1STATbits.SMP = 1;           // Slew rate control is disabled for Standard Speed mode (100 kHz and 1 MHz)
    SSP1STATbits.CKE = 0;
    
    // Pin Direction Control (TRISC)
        // SCL / SDA Direction
    // SDA1 == RB9
    // SCL1 == RB7
    TRISBbits.TRISB9 = 1;
    TRISBbits.TRISB8 = 1;

    SSP1STAT = 0x00;                             //Resetting SSPSTAT register
    _SSP1IF=0;
//    __delay_ms(100);    
} // InitI2C

//define StartI2C1() SSP1CON2bits.SEN = 1; while(SSP1CON2bits.SEN);       // Initiates Start condition on SDAx and SCLx pins; automatically cleared by hardware
//#define IdleI2C1() SSP1IF = 1;                  // The  MSSP  module  will  wait  the  required  start  time  before  any  other operation takes place.    
//#define StopI2C1()  SSP1CON2bits.PEN = 1;       // Initiates Stop condition on SDAx and SCLx pins; automatically cleared by hardware

void i2cReady(){
    // Wait for buffer full and read / write flag
    while(SSP1STATbits.BF);    
}

// i2c_Wait - wait for I2C transfer to finish
void i2c_Wait(){
    while ( ( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) );
}

void mssp_wait(void)
{
    while(!_SSP1IF);                              //wait till SSPIF flag is set
    _SSP1IF=0;
}

void i2c_idle(void)
{
  // Wait for Status Idle (i.e. ACKEN, RCEN, PEN, RSEN, SEN) and I2C Bus stop detection
  while (( SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ));
}


// i2c_Address - Sends Slave Address and Read/Write mode
// mode is either I2C_WRITE or I2C_READ
uint8_t i2cAddress(uint8_t address, uint8_t mode)
{
    uint8_t l_address;

    l_address = address << 1;
    l_address += mode;
    
    SSP1BUF = l_address;
    i2cReady();
    if (SSP1CON2bits.ACKSTAT)       // Check for acknowledge bit
        return 0;
    else
        return 1;

}

uint8_t i2cWrite(uint8_t data){
    SSP1BUF = data;                 // Write data to SSPBUF
    mssp_wait();    
    i2cReady();
    if (SSP1CON2bits.ACKSTAT)       // Check for acknowledge bit
        return 0;
    else
        return 1;
    
    return 1;
}

uint8_t i2cStart(){
    i2c_idle();
    SSP1CON2bits.SEN = 1;           // Send start pulse
    while (SSP1CON2bits.SEN);       // Wait for completion of start pulse
    //SSPIF = 0;
    mssp_wait();  
    if (!SSP1STATbits.S)            // Check whether START detected last
        return 0;
    else
        return 1;
}

void i2cRestart(){
    i2c_idle();
    SSP1CON2bits.RSEN = 1;
    mssp_wait();    
    while (SSP1CON2bits.RSEN);
}

uint8_t i2cStop(){
    i2c_idle();
    i2cReady();
    
    SSP1CON2bits.PEN = 1;           // Stop communication
    mssp_wait();    
    while (SSP1CON2bits.PEN);       // Wait for end of stop pulse
    if (!SSP1STATbits.P)            // Check if STOP is detected last
        return 1;
    else
        return 0;
}

void i2cAck(){
    SSP1CON2bits.ACKDT = 0;         // Acknowledge data 1:NACK, 0:ACK
    SSP1CON2bits.ACKEN = 1;         // Enable ACK to send
    while (SSP1CON2bits.ACKEN);
}

void i2cNack(){
    SSP1CON2bits.ACKDT = 1;         // Acknowledge data 1:NACK, 0:ACK
    SSP1CON2bits.ACKEN = 1;         // Enable ACK to send
    while (SSP1CON2bits.ACKEN);
}

uint8_t i2cRead(uint8_t ack){
    uint8_t buffer;
    SSP1CON2bits.RCEN = 1;          // Enable receive
    
    // Wait for buffer full flag which when complete byte received
    while (!SSP1STATbits.BF);
    buffer = SSP1BUF;                // Copy SSPBUF to buffer
     SSP1CON2bits.RCEN = 0;
    
    // Send acknowledgement or negative acknowledgement after read to continue or stop reading.
    if (ack) 
        SSP1CON2bits.ACKDT=0;	        // Ack
    else       
        SSP1CON2bits.ACKDT=1;	        // NAck
    SSP1CON2bits.ACKEN=1;               // send acknowledge sequence
    
    i2cReady();
    
    return buffer;
}

