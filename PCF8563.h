/* 
 * File:   PCF8563.h
 * Author: stevend
 *
 * Created on 25 July 2018, 07:28
 */

#ifndef PCF8563_H
#define	PCF8563_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PCF8563_I2CADDR_DEFAULT     0b01010001      // 0x51
    
#define RTCC_SEC        1
#define RTCC_MIN        2
#define RTCC_HR         3
#define RTCC_DAY        4
#define RTCC_WEEKDAY    5
#define RTCC_MONTH      6
#define RTCC_YEAR       7
#define RTCC_CENTURY    8

/* register addresses in the rtc */
#define RTCC_STAT1_ADDR     0x0
#define RTCC_STAT2_ADDR     0x01
#define RTCC_SEC_ADDR       0x02
#define RTCC_MIN_ADDR       0x03
#define RTCC_HR_ADDR        0x04
#define RTCC_DAY_ADDR       0x05
#define RTCC_WEEKDAY_ADDR   0x06
#define RTCC_MONTH_ADDR     0x07
#define RTCC_YEAR_ADDR      0x08
#define RTCC_ALRM_MIN_ADDR  0x09
#define RTCC_SQW_ADDR       0x0D
#define RTCC_TIMER1_ADDR    0x0E
#define RTCC_TIMER2_ADDR    0x0F

/* setting the alarm flag to 0 enables the alarm.
 * set it to 1 to disable the alarm for that value.
 */
#define RTCC_ALARM          0x80
#define RTCC_ALARM_AIE      0x02
#define RTCC_ALARM_AF       0x08
/* optional val for no alarm setting */
#define RTCC_NO_ALARM       99

#define RTCC_TIMER_TIE      0x01  // Timer Interrupt Enable

#define RTCC_TIMER_TF       0x04  // Timer Flag, read/write active state
                                  // When clearing, be sure to set RTCC_TIMER_AF
                                  // to 1 (see note above).
#define RTCC_TIMER_TI_TP    0x10  // 0: INT is active when TF is active
                                  //    (subject to the status of TIE)
                                  // 1: INT pulses active
                                  //    (subject to the status of TIE);
                                  // Note: TF stays active until cleared
                                  // no matter what RTCC_TIMER_TI_TP is.
#define RTCC_TIMER_TD10     0x03  // Timer source clock, TMR_1MIN saves power
#define RTCC_TIMER_TE       0x80  // Timer 1:enable/0:disable

/* Timer source-clock frequency constants */
#define TMR_4096HZ      B00000000
#define TMR_64Hz        B00000001
#define TMR_1Hz         B00000010
#define TMR_1MIN        B00000011

#define RTCC_CENTURY_MASK   0x80
#define RTCC_VLSEC_MASK     0x80

/* date format flags */
#define RTCC_DATE_WORLD     0x01
#define RTCC_DATE_ASIA      0x02
#define RTCC_DATE_US        0x04
/* time format flags */
#define RTCC_TIME_HMS       0x01
#define RTCC_TIME_HM        0x02

/* square wave contants */
#define SQW_DISABLE     0B00000000
#define SQW_32KHZ       0B10000000
#define SQW_1024HZ      0B10000001
#define SQW_32HZ        0B10000010
#define SQW_1HZ         0B10000011
    
    
/* time variables */
static volatile uint8_t hour;
static volatile uint8_t minute;
static volatile uint8_t volt_low;
static volatile uint8_t sec;
static volatile uint8_t day;
static volatile uint8_t weekday;
static volatile uint8_t month;
static volatile uint8_t year;
/* alarm */
static volatile uint8_t alarm_hour;
static volatile uint8_t alarm_minute;
static volatile uint8_t alarm_weekday;
static volatile uint8_t alarm_day;
/* CLKOUT */
static volatile uint8_t squareWave;
/* timer */
static volatile uint8_t timer_control;
static volatile uint8_t timer_value;
/* support */
static volatile uint8_t status1;
static volatile uint8_t status2;
static volatile uint8_t century;    


void pcf8563_init();
void zeroClock();

/*
* Read all device registers in one operation
*/
void getDateTime();

uint8_t getSecond();
uint8_t getMinute();
uint8_t getHour();   
void enableAlarm();
void clearAlarm();
void setAlarm(uint8_t min, uint8_t hour, uint8_t day, uint8_t weekday);

#ifdef	__cplusplus
}
#endif

#endif	/* PCF8563_H */

