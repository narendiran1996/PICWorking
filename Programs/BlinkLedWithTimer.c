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


void __interrupt() timer0Interrupt(void) // interrupt function
{
    if ((INTCON & (1 << 2)) != 0)
    {

        LATA = ((LATA & 0x01) == 0) ? (LATA | 0x01) : (LATA & 0xFE);
        TMR0L = 100;         // load with the value of 256-TMR0L counts to get
        INTCON &= ~(1 << 2); // clear timer interupt flag
        INTCON |= (1 << 5);  // enable timer intterupt
    }
}
void timer0Init()
{
    TMR0L = 100;        // load with the value of 256-TMR0L counts to get
    T0CON |= (1 << 6);  // Timer 0 as 8-bit counting
    T0CON &= ~(1 << 5); // Timer 0 clock source as internal clk (CLKO)

    // presclar of 32
    T0CON |= (1 << 2);
    T0CON &= ~(1 << 1);
    T0CON &= ~(1 << 0);

    T0CON &= ~(1 << 3); // presclar assigned

    INTCON |= (1 << 5); // Timer0 interrupt enable
    INTCON |= (1 << 7); // Global Interrupt enable

    T0CON |= (1 << 7); // start the time0
}

void main()
{
    TRISA = TRISA & ~(1 << 0);
    LATA = LATA & ~(1 << 0);

    timer0Init();

    while (1)
    {
    }
}
