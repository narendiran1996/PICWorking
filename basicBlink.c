// BEGIN CONFIG
#pragma config OSC = HS   // Oscillator Selection (HS oscillator)
#pragma config OSCS = ON  // Oscillator Switch Disbale
#pragma config PWRT = OFF // Power UpTime Disbale (PWRT disabled)
#pragma config BOR = ON   // Brown-out Reset (BOR enabled)
#pragma config WDT = OFF  // Watch Dog timer disbaled
#pragma config LVP = OFF  // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//END CONFIG

#define _XTAL_FREQ 4000000L

#include <xc.h>

void main()
{

    // Make RA0 as output
    TRISA = TRISA & ~(1 << 0);

    // swich off RA0
    LATA = LATA & ~(1 << 0);

    while (1)
    {
        // swich off RA0
        LATA = LATA & ~(1 << 0);
        __delay_ms(2);

        // swich on RA0
        LATA = LATA | (1 << 0);
        __delay_ms(2);
    }
}
