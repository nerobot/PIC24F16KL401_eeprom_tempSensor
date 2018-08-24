/*
 * File:   newmainXC16.c
 * Author: stevend
 *
 * Created on 24 August 2018, 10:37
 */

#include "config.h"
#include "PIC24F16KL401.h"
#include "xc.h"
#include <libpic30.h>

#include "i2c1.h"
#include "mcp9808.h"

// Data structure that will contain all data being sent via the RFM69 RF module.
// Serial gateway needs updating whenever this gets updated.
struct dataStruct{
    uint16_t    temp;
    uint8_t     hour;
    uint8_t     minute;
    uint8_t     second;
    uint8_t     day;
    uint8_t     month;
    uint8_t     year;
} data;

void initPorts(){
    TRISA = 0x0000;
    TRISB = 0x0000;
    
    // Individual pins
    LEDTRIS = 0;
    LEDANS = 0;
    LED = 0;
    
    RTCINTTRIS = 1;
    
    // Used for powering RTC
    // TODO: (re-name in defines))
    _TRISA3 = 0;
    _ANSA3 = 0;
    _RA3 = 1;
    
    // TODO: Need to add inputs for selector switch
}

void initPMD(){
    // Turn off all modules, only turning on the needed ones.
    PMD1 = 0xffff;
    PMD2 = 0xffff;
    PMD3 = 0xffff;
    PMD4 = 0xffff;
    
    // Now turning on the needed modules.
    _T1MD = 0;              // Timer1
    _SSP1MD = 0;            // I2C
    _U1MD = 0;              // UART
    //_SSP2MD = 0;            // SPI        // No longer being used
}

void initInterrupts(){
    // INT2 pin change interrupt (high to low)
    _INT2EP = 1;
    _INT2IE = 1;
    _INT2IP = 0b111;
}

void init(){
    initPorts();
    initPMD();
    initInterrupts();        
}

enum selector{
    sensing,
    sending
};

//! Checks what state the "selector" is in. This will determine if the systems reads the temperature, or sends the data.
uint8_t selector(){
    // TODO: add data for reading and sending the selector.
    return sensing;
}

int main(void) {
    // variables
    uint8_t sel = 0;
    
    // init
    init();
    
    // check selector
    /*switch (selector()){
        case sensing:
            break;
        case sending:
            break;
        default:
            break;
    } */
    sel = selector();
    
    // if selector == sense
    if (sel == sensing){                
    
        // wake up - nothing to be done.

        // read the current time                        

        // read the current temperature
        _SSP1MD = 0;
        i2cInit();
        while(!initMCP()){}
        __delay_ms(1000);
        data.temp = readTemp();

        // store the data in the eeprom

        // sleep for one minute until RTC interrupt (or just delay for now).
    }
    
    // else if selector == send data
    
        // once a minute
    
            // for all pages (needs looking into) in eeprom
                
                // send page via uart
    
                // read uart values
    
                // if read != sent
    
                    // resend the data
    
            // wait
    
    while(1){}
    
    return 0;
}
