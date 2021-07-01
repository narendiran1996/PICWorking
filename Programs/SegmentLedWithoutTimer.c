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

void displayDigits(unsigned char twoDigitNumber)
{
    if (twoDigitNumber < 10)
    {
        // digit2 is 0
        setDigit(twoDigitNumber);
        enableDIGIT1;
        __delay_us(39500);
        disableDIGIT1;

        setDigit(0);
        enableDIGIT2;
        __delay_us(39500);
        disableDIGIT2;
    }
    else if (twoDigitNumber < 100)
    {
        // proceed normally
        setDigit(twoDigitNumber % 10);
        enableDIGIT1;
        __delay_us(39500);
        disableDIGIT1;

        setDigit((unsigned char)(twoDigitNumber / 10));
        enableDIGIT2;
        __delay_us(39500);
        disableDIGIT2;
    }
    else
    {
        // display FF
        setDigit(10);
        enableDIGIT1;
        __delay_us(39500);
        disableDIGIT1;

        setDigit(10);
        enableDIGIT2;
        __delay_us(39500);
        disableDIGIT2;
    }
}

void main()
{
    TRISC = 0x00;
    LATC = 0xFF;

    TRISD = TRISD & 0xFC;
    LATD = LATD & 0xFC;
    while (1)
    {
        displayDigits(0);
    }
}
