/*
 * File:   newmainXC16.c
 * Author: stevend
 *
 * Created on 24 August 2018, 10:37
 */

#include "config.h"
#include "PIC24F16KL401.h"
#include "xc.h"

#include "i2c1.h"
#include "mcp9808.h"

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

//! Checks what state the "selector" is in. This will determine if the systems reads the temperature, or sends the data.
uint8_t selector(){
    // TODO: add data for reading and sending the selector.
}

int main(void) {
    
    // init
    init();
    
    // check selector
    switch (selector()){
        default:
            break;
    }
    
    // if selector == sense
    
        // once a minute
    
            // wake up
    
            // read the current time
    
            // read the current temperature
    
            // store the data in the eeprom
    
            // go back to sleep
    
    // else if selector == send data
    
        // once a minute
    
            // for all pages (needs looking into) in eeprom
                
                // send page via uart
    
                // read uart values
    
                // if read != sent
    
                    // resend the data
    
            // wait
    return 0;
}
