#include <xc.h>
#include "PCF8563.h"
#include "i2c1.h"

uint8_t bcdToDec(uint8_t val)
{
    return ( (val/16*10) + (val%16) );
}

/* Private internal functions, but useful to look at if you need a similar func. */
uint8_t decToBcd(uint8_t val)
{
    return ( (val/10*16) + (val%10) );
}

void pcf8563_init(){
    /*i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    
    i2cWrite(0x00);     // Address
    
    i2cWrite(0x00);     // control / status 1
    i2cWrite(0x00);     // control / status 2
    
    i2cWrite(0x00);    //set seconds to 0 & VL to 0
    i2cWrite(0x00);    //set minutes to 0
    i2cWrite(0x00);    //set hour to 0
    i2cWrite(0x01);    //set day to 1
    i2cWrite(0x00);    //set weekday to 0
    i2cWrite(0x81);    //set month to 1, century to 1900
    i2cWrite(0x00);    //set year to 0
    i2cWrite(0x80);    //minute alarm value reset to 00
    i2cWrite(0x80);    //hour alarm value reset to 00
    i2cWrite(0x80);    //day alarm value reset to 00
    i2cWrite(0x80);    //weekday alarm value reset to 00
    i2cWrite(SQW_32KHZ); //set SQW to default, see: setSquareWave
    i2cWrite(0x0); //timer off
    
    i2cStop();*/
}

void zeroClock(){    
    i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    
    i2cWrite(0x00);     // Address
    
    i2cWrite(0x00);     // control / status 1
    i2cWrite(0x00);     // control / status 2
    
    i2cWrite(0x00);    //set seconds to 0 & VL to 0
    i2cWrite(0x00);    //set minutes to 0
    i2cWrite(0x00);    //set hour to 0
    i2cWrite(0x01);    //set day to 1
    i2cWrite(0x00);    //set weekday to 0
    i2cWrite(0x81);    //set month to 1, century to 1900
    i2cWrite(0x00);    //set year to 0
    i2cWrite(0x80);    //minute alarm value reset to 00
    i2cWrite(0x80);    //hour alarm value reset to 00
    i2cWrite(0x80);    //day alarm value reset to 00
    i2cWrite(0x80);    //weekday alarm value reset to 00
    i2cWrite(SQW_32KHZ); //set SQW to default, see: setSquareWave
    i2cWrite(0x0); //timer off
    
    i2cStop();      
}

/*
* Read all device registers in one operation
*/
void getDateTime(){
    uint8_t i;
    uint8_t readBuffer[16] = {0};
    
    /* Start at beginning, read entire memory in one go */
    i2cStart();
    
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);    
    i2cWrite(RTCC_STAT1_ADDR);
    i2cRestart();
    
    i2cAddress(PCF8563_I2CADDR_DEFAULT, READ_CMD);
    for (i = 0; i < 15; i++){
        readBuffer[i] = i2cRead(1);
    }
    readBuffer[15] = i2cRead(0);
    
    i2cStop();
    
    // Status bytes
    status1 = readBuffer[0];
    status2 = readBuffer[1];
    
    // time bytes
    // 0x7f = 0b01111111
    volt_low = readBuffer[2] & RTCC_VLSEC_MASK;  //VL_Seconds
    sec = bcdToDec(readBuffer[2] & ~RTCC_VLSEC_MASK);
    minute = bcdToDec(readBuffer[3] & 0x7f);
    //0x3f = 0b00111111
    hour = bcdToDec(readBuffer[4] & 0x3f);
    
    // date bytes
    //0x3f = 0b00111111
    day = bcdToDec(readBuffer[5] & 0x3f);
    //0x07 = 0b00000111
    weekday = bcdToDec(readBuffer[6] & 0x07);
    //get raw month data byte and set month and century with it.
    month = readBuffer[7];
    if (month & RTCC_CENTURY_MASK)
        century = true;
    else
        century = false;
    //0x1f = 0b00011111
    month = month & 0x1f;
    month = bcdToDec(month);
    year = bcdToDec(readBuffer[8]);

    // alarm bytes
    alarm_minute = readBuffer[9];
    if(0B10000000 & alarm_minute)
        alarm_minute = RTCC_NO_ALARM;
    else
        alarm_minute = bcdToDec(alarm_minute & 0B01111111);
    alarm_hour = readBuffer[10];
    if(0B10000000 & alarm_hour)
        alarm_hour = RTCC_NO_ALARM;
    else
        alarm_hour = bcdToDec(alarm_hour & 0B00111111);
    alarm_day = readBuffer[11];
    if(0B10000000 & alarm_day)
        alarm_day = RTCC_NO_ALARM;
    else
        alarm_day = bcdToDec(alarm_day  & 0B00111111);
    alarm_weekday = readBuffer[12];
    if(0B10000000 & alarm_weekday)
        alarm_weekday = RTCC_NO_ALARM;
    else
    alarm_weekday = bcdToDec(alarm_weekday & 0B00000111);
    
    // CLKOUT_control 0x03 = 0b00000011
    squareWave = readBuffer[13] & 0x03;

    // timer bytes
    timer_control = readBuffer[14] & 0x03;
    timer_value = readBuffer[15]; // current value != set value when running
}

/*i2cStart();
    
    i2cAddress(MCP9808_I2CADDR_DEFAULT, WRITE_CMD);
    i2cWrite(address);
    i2cRestart();
    
    i2cAddress(MCP9808_I2CADDR_DEFAULT, READ_CMD);
    buffer = (i2cRead(1) << 8);
    buffer += i2cRead(0);
    
    i2cStop();*/

uint8_t getSecond(){
    return sec;
}

uint8_t getMinute(){
    return minute;
}

uint8_t getHour(){
    return hour;
}

uint8_t constrain(uint8_t x, uint8_t a, uint8_t b){
    if (x < a)
        return a;
    else if (x > b)
        return b;
    else
        return x;
}

/* set the alarm values
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 * 
 * When value > 99, its alarm is switched off.
 */
    void setAlarm(uint8_t min, uint8_t hour, uint8_t day, uint8_t weekday)
{
    getDateTime();  // operate on current values
    if (min <99) {
        min = constrain(min, 0, 59);
        min = decToBcd(min);
        min &= ~RTCC_ALARM;
    } else {
        min = 0x0; min |= RTCC_ALARM;
    }

    if (hour <99) {
        hour = constrain(hour, 0, 23);
        hour = decToBcd(hour);
        hour &= ~RTCC_ALARM;
    } else {
        hour = 0x0; hour |= RTCC_ALARM;
    }

    if (day <99) {
        day = constrain(day, 1, 31);
        day = decToBcd(day); day &= ~RTCC_ALARM;
    } else {
        day = 0x0; day |= RTCC_ALARM;
    }

    if (weekday <99) {
        weekday = constrain(weekday, 0, 6);
        weekday = decToBcd(weekday);
        weekday &= ~RTCC_ALARM;
    } else {
        weekday = 0x0; weekday |= RTCC_ALARM;
    }

    alarm_hour = hour;
    alarm_minute = min;
    alarm_weekday = weekday;
    alarm_day = day;

    // First set alarm values, then enable
    //Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    //Wire.write((byte)RTCC_ALRM_MIN_ADDR);
    i2cWrite(RTCC_ALRM_MIN_ADDR);
    i2cWrite(alarm_minute);
    i2cWrite(alarm_hour);
    i2cWrite(alarm_day);
    i2cWrite(alarm_weekday);
    //Wire.endTransmission();
    i2cStop();

    enableAlarm();
}
/*    i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    
    i2cWrite(0x00);     // Address
    
    i2cWrite(0x00);     // control / status 1
    
    i2cStop();  */

/* enable alarm interrupt
 * whenever the clock matches these values an int will
 * be sent out pin 3 of the Pcf8563 chip
 */
void enableAlarm()
{
    getDateTime();  // operate on current values
    //set status2 AF val to zero
    status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_TIMER_TF;
    //enable the interrupt
    status2 |= RTCC_ALARM_AIE;

    //enable the interrupt
    //Wire.beginTransmission(Rtcc_Addr);  // Issue I2C start signal
    i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    i2cWrite(RTCC_STAT2_ADDR);
    i2cWrite(status2);
    //Wire.endTransmission();
    i2cStop();
}

// Clears the alarm and interupt.
void clearAlarm()
{
    //set status2 AF val to zero to reset alarm
    status2 &= ~RTCC_ALARM_AF;
    //set TF to 1 masks it from changing, as per data-sheet
    status2 |= RTCC_TIMER_TF;
    //turn off the interrupt
    status2 &= ~RTCC_ALARM_AIE;

    //Wire.beginTransmission(Rtcc_Addr);
    i2cStart();
    i2cAddress(PCF8563_I2CADDR_DEFAULT, WRITE_CMD);
    i2cWrite(RTCC_STAT2_ADDR);
    i2cWrite(status2);
    i2cStop();
}