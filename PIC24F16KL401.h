/* 
 * File:   PIC24F16KL401.h
 * Author: scdag
 *
 * Created on 22 July 2018, 11:46
 */

#ifndef PIC24F16KL401_H
#define	PIC24F16KL401_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PIC24F16KL401
#define XTAL_FREQ       8000000UL           // Raw oscillator freq
#define FCY             XTAL_FREQ / 2       // Most but not all PIC24s
    
// Debugging LED    
#define LED             PORTBbits.RB4       
#define LEDTRIS         TRISBbits.TRISB4
#define LEDANS          ANSBbits.ANSB4
    
// Used for the input from the RTC (will be interrupt later on)
#define RTCIN           PORTAbits.RA6
#define RTCINTTRIS      TRISAbits.TRISA6
    
#ifdef	__cplusplus
}
#endif

#endif	/* PIC24F16KL401_H */

