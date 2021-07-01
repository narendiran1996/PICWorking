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

unsigned char isPressed()
{
    return ((PORTB & 0x01) == 0x00);
}
unsigned char Patterns[6] = {
    0xF7,
    0xDD,
    0xD5,
    0xAA,
    0xA2,
    0x88};

unsigned char pseudoRandomNumber(unsigned int limit)
{
    unsigned char result;
    static unsigned int num = 1;
    num = (num * 32719 + 3) % 32749;
    result = (unsigned char)(num % limit);
    return result;
}

void main()
{
    TRISC = 0x00;
    LATC = 0xFF;
    TRISB |= (1 << 0);
    unsigned char indx = 0;

    while (1)
    {
        if (isPressed() == 1)
        {
            // limit
            LATC = Patterns[pseudoRandomNumber(6)];
            __delay_ms(1000);
            indx = indx + 1;
            if (indx == 6)
                indx = 0;
        }
        LATC = 0xFF;
    }
}
