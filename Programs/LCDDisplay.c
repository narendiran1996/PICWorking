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

#define set_En LATD |= (1 << 1)
#define clear_En LATD &= ~(1 << 1)

#define set_RS LATD |= (1 << 0)
#define clear_RS LATD &= ~(1 << 0)

#define LCD_set_MSB(val_) LATC = (LATC & 0xF0) | ((val_ & 0xF0) >> 4)
#define LCD_set_LSB(val_) LATC = (LATC & 0xF0) | ((val_ & 0x0F) >> 0)

void LCD_custom_delay(unsigned char del_val)
{
    for (unsigned char i = 0; i < 100; i++)
    {
        for (unsigned char j = 0; j < 10; j++)
        {
            NOP();
        }
    }
}
void LCD_send_cmd(uint8_t cmd_)
{
    LCD_set_MSB(cmd_);
    clear_RS;
    set_En;
    LCD_custom_delay(1);
    clear_En;

    LCD_set_LSB(cmd_);
    clear_RS;
    set_En;
    LCD_custom_delay(1);
    clear_En;
}
void LCD_send_data(uint8_t data_)
{
    LCD_set_MSB(data_);
    set_RS;
    set_En;
    LCD_custom_delay(1);
    clear_En;

    LCD_set_LSB(data_);
    set_RS;
    set_En;
    LCD_custom_delay(1);
    clear_En;
}
void LCD_sequence(void)
{
    /* The module powers up in 8-bit mode.
	   The initial start-up instructions are sent in 8-bit mode, 
	   with the lower four bits (which are not connected) of each instruction
	    as don't cares */
    LCD_send_cmd(0x02); // initialization in 4-bit mode

    /* setting 4bit communication and 2-line display and 5x8 dots format display */
    // Function Set --> D[7:4] -- 001(DL)_(N)(F)xx
    // DL === 0 -- 4bit mode
    // DL === 1 -- 8bit mode
    // N === 0 -- 1-line display
    // N === 1 -- 2-line display
    // F === 0 -- 5x8 dots format display mode
    // F === 1 -- 5x11 dots format display mode
    // so we select D[7:0] = 0b0010_10xx --- 0x28
    LCD_send_cmd(0x28);

    /* Setting up of cursor left movement on increment and display not shifting */
    /* Entry Mode */
    // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
    // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
    // I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
    // S === 0 -- Dont' Shifht the entire display
    // S === 1 -- Shift the entire display
    // so we select D[7:0] = 0b0000_0110 --- 0x06
    LCD_send_cmd(0x06);

    /* Setting up of disply on, cursor on and blink on	*/
    /* Display ON/OFF, Cursor ON/OFF  Blink ON/OFF*/
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_1111 = 0x0F;
    LCD_send_cmd(0x0E);

    /* Set DDRAM address */
    // when 1- line display (N=0), DDRAM address from 00H to 4FH -- TOTAL 40
    // when 2- line display (N=1), DDRAM address from 00H to 27H for first line AND 40H to 67H for seoncd ine -- totall 20 + 20 = 40
    // D[7:0] --- 0b1(AC6)(AC5)(AC4)(AC3)(AC2)(AC1)(AC0)
    // So we select D[7:0] = 0b1000_0000 = 0x80;

    LCD_send_cmd(0x80);
}

void LCD_Init()
{
    TRISC = TRISC & 0xF0;
    TRISD = TRISD & 0xFC;

    LATC = LATC & 0xF0;
    LATD = LATD & 0xFC;
    LCD_sequence();
}
void LCD_set_cursor(unsigned char r, unsigned char c)
{
    // r and c starts from 1
    if (r == 1)
    {
        LCD_send_cmd(0x80 + c - 1);
    }
    else if (r == 2)
    {
        LCD_send_cmd(0xC0 + c - 1);
    }
}

void LCD_clear(void)
{
    // 0x01 for clearing display
    LCD_send_cmd(0x01);
}
void LCD_home(void)
{
    // 0000_001x for moving cursor to inital position and moving DRAM address to 00H
    LCD_send_cmd(0x02);
}
void LCD_print_left(void)
{
    // moving left
    /* Entry Mode */
    // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
    // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
    // I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
    // S === 0 -- Dont' Shifht the entire display
    // S === 1 -- Shift the entire display
    // so we select D[7:0] = 0b0000_0100 --- 0x04
    LCD_send_cmd(0x04);
}
void LCD_print_right(void)
{
    // moving left
    /* Entry Mode */
    // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
    // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
    // I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
    // S === 0 -- Dont' Shifht the entire display
    // S === 1 -- Shift the entire display
    // so we select D[7:0] = 0b0000_0100 --- 0x06
    LCD_send_cmd(0x06);
}
void LCD_shift_left(void)
{
    /* Entry Mode */
    // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
    // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
    // I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
    // S === 0 -- Dont' Shifht the entire display
    // S === 1 -- Shift the entire display
    // so we select D[7:0] = 0b0000_0100 --- 0x06
    LCD_send_cmd(0x07);
}
void LCD_shift_right(void)
{
    /* Works when there is character before the first character */
    /* Entry Mode */
    // Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
    // I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
    // I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
    // S === 0 -- Dont' Shifht the entire display
    // S === 1 -- Shift the entire display
    // so we select D[7:0] = 0b0000_0100 --- 0x06
    LCD_send_cmd(0x05);
}
void LCD_display_off(void)
{
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_10xx = 0x0F;
    LCD_send_cmd(0x08);
}
void LCD_display_on_cursor_off_blink_off(void)
{
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_1100 = 0x0E;
    LCD_send_cmd(0x0C);
}
void LCD_display_on_cursor_on_blink_off(void)
{
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_1110 = 0x0E;
    LCD_send_cmd(0xE);
}
void LCD_display_on_cursor_off_blink_on(void)
{
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_1101 = 0x0D;
    LCD_send_cmd(0x0D);
}
void LCD_display_on_cursor_on_blink_on(void)
{
    // Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
    // D === 0 -- display off display data is remained in DDRAM
    // D === 1 -- display on
    // C === 0 -- cursor is disabled in current display, but I/D register data is reamins
    // C === 1 -- cursto is turned on
    // B === 0 -- Blink off
    // B === 1 -- Blink on
    // so we slect D[7:0] = 0b0000_1111 = 0x0F;
    LCD_send_cmd(0x0F);
}
void LCD_cursor_leftShift(void)
{
    // Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
    // (S/C)(R/L) === 00 -- Shift Cursor to left
    // (S/C)(R/L) === 01 --
    // (S/C)(R/L) === 10 --
    // (S/C)(R/L) === 11 --
    // so we slect D[7:0] = 0b0001_00xx = 0x10;
    LCD_send_cmd(0x010);
}
void LCD_cursor_rightShift(void)
{
    // Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
    // (S/C)(R/L) === 00 -- Shift Cursor to left
    // (S/C)(R/L) === 01 -- Shift Cursor to Right
    // (S/C)(R/L) === 10 --
    // (S/C)(R/L) === 11 --
    // so we slect D[7:0] = 0b0001_01xx = 0x14;
    LCD_send_cmd(0x014);
}
void LCD_display_leftShift(void)
{
    // Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
    // (S/C)(R/L) === 00 -- Shift Cursor to left
    // (S/C)(R/L) === 01 -- Shift Cursor to Right
    // (S/C)(R/L) === 10 -- Shift Display to Left
    // (S/C)(R/L) === 11 --
    // so we slect D[7:0] = 0b0001_10xx = 0x18;
    LCD_send_cmd(0x018);
}
void LCD_display_rightShift(void)
{
    // Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
    // (S/C)(R/L) === 00 -- Shift Cursor to left
    // (S/C)(R/L) === 01 -- Shift Cursor to Right
    // (S/C)(R/L) === 10 -- Shift Display to Left
    // (S/C)(R/L) === 11 -- Shift Display to Left
    // so we slect D[7:0] = 0b0001_11xx = 0x1C;
    LCD_send_cmd(0x01C);
}
void LCD_send_string(char *c_string)
{
    while (*c_string != '\0')
    {
        LCD_send_data(*c_string++);
    }
}

void LCD_send_string_l(unsigned char *c_string, unsigned char len)
{
    for (unsigned char i = 0; i < len; i++)
    {
        LCD_send_data(c_string[i]);
    }
}
char *itoa(int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36)
    {
        *result = '\0';
        return result;
    }

    char *ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
void LCD_print_int(int num, int base)
{
    char buff[16];
    itoa(num, buff, base);
    LCD_send_string(buff);
}
void LCD_print_float(float num)
{
    int decimal = (int)num;
    float fraction1 = (num - decimal) * 1000;
    int fraction = (int)fraction1;
    if (fraction < 0)
        fraction = -1 * fraction;
    char buff1[16];
    itoa(decimal, buff1, 10);
    LCD_send_string(buff1);
    LCD_send_data('.');
    char buff2[16];
    itoa(fraction, buff2, 10);
    LCD_send_string(buff2);
}

void main()
{
    LCD_Init();
    LCD_send_string("HI !");
    LCD_print_int(47, 10);
    while (1)
    {
    }
}
