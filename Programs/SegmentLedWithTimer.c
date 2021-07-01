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

#define enableDIGIT1 LATD |= (1 << 0)
#define disableDIGIT1 LATD &= ~(1 << 0)

#define enableDIGIT2 LATD |= (1 << 1)
#define disableDIGIT2 LATD &= ~(1 << 1)

#define setDigit(indx) LATC = SegmentValues[indx]

unsigned char SegmentValues[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x71};

unsigned char count = 0;

unsigned char dig = 1;

void __interrupt() timer0Interrupt(void) // interrupt function
{
    if ((INTCON & (1 << 2)) != 0)
    {
        if (dig == 1)
        {
            disableDIGIT2;
            setDigit(count % 10);
            enableDIGIT1;
            dig = 2;
        }
        else
        {
            disableDIGIT1;
            setDigit(count / 10);
            enableDIGIT2;
            dig = 1;
        }

        TMR0L = 30;         // load with the value of 256-TMR0L counts to get
        INTCON &= ~(1 << 2); // clear timer interupt flag
        INTCON |= (1 << 5);  // enable timer intterupt
    }
}
void timer0Init()
{
    TMR0L = 30;        // load with the value of 256-TMR0L counts to get
    T0CON |= (1 << 6);  // Timer 0 as 8-bit counting
    T0CON &= ~(1 << 5); // Timer 0 clock source as internal clk (CLKO)

    // presclar of 2
    T0CON |= (1 << 2);
    T0CON |= (1 << 1);
    T0CON |= (1 << 0);

    T0CON &= ~(1 << 3); // presclar assigned

    INTCON |= (1 << 5); // Timer0 interrupt enable
    INTCON |= (1 << 7); // Global Interrupt enable

    T0CON |= (1 << 7); // start the time0
}

void main()
{
    TRISC = 0x00;
    LATC = 0xFF;

    TRISD = TRISD & 0xFC;
    LATD = LATD & 0xFC;

    timer0Init();

    while (1)
    {
        count = count + 1;
        __delay_ms(1000);
    }
}
