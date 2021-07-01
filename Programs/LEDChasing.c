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

    // Make RC as output
    TRISC = TRISC & 0x00;

    // swich off RA0
    LATC = LATC & 0x00;
    unsigned char val = 0x01;
    unsigned char dir = 0;
    while (1)
    {
        LATC = ~val;
        __delay_ms(100);
        val = (dir == 0) ? val << 1 : val >> 1;
        if (val == 0x80)
            dir = 1;
        else if (val == 0x01)
            dir = 0;
    }
}
