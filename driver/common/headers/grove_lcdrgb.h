#ifndef __GROVE_LCDRGB_H__
#define __GROVE_LCDRGB_H__

#include <stdint.h>

// Device I2C Arress
#define GROVE_LCDRGB_LCD_ADDRESS     (0x7c>>1)
#define GROVE_LCDRGB_RGB_ADDRESS     (0xc4>>1)

// color define
#define GROVE_LCDRGB_WHITE           0
#define GROVE_LCDRGB_RED             1
#define GROVE_LCDRGB_GREEN           2
#define GROVE_LCDRGB_BLUE            3

#define GROVE_LCDRGB_REG_RED         0x04        // pwm2
#define GROVE_LCDRGB_REG_GREEN       0x03        // pwm1
#define GROVE_LCDRGB_REG_BLUE        0x02        // pwm0

#define GROVE_LCDRGB_REG_MODE1       0x00
#define GROVE_LCDRGB_REG_MODE2       0x01
#define GROVE_LCDRGB_REG_OUTPUT      0x08

// commands
#define GROVE_LCDRGB_LCD_CLEARDISPLAY 0x01
#define GROVE_LCDRGB_LCD_RETURNHOME 0x02
#define GROVE_LCDRGB_LCD_ENTRYMODESET 0x04
#define GROVE_LCDRGB_LCD_DISPLAYCONTROL 0x08
#define GROVE_LCDRGB_LCD_CURSORSHIFT 0x10
#define GROVE_LCDRGB_LCD_FUNCTIONSET 0x20
#define GROVE_LCDRGB_LCD_SETCGRAMADDR 0x40
#define GROVE_LCDRGB_LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define GROVE_LCDRGB_LCD_ENTRYRIGHT 0x00
#define GROVE_LCDRGB_LCD_ENTRYLEFT 0x02
#define GROVE_LCDRGB_LCD_ENTRYSHIFTINCREMENT 0x01
#define GROVE_LCDRGB_LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define GROVE_LCDRGB_LCD_DISPLAYON 0x04
#define GROVE_LCDRGB_LCD_DISPLAYOFF 0x00
#define GROVE_LCDRGB_LCD_CURSORON 0x02
#define GROVE_LCDRGB_LCD_CURSOROFF 0x00
#define GROVE_LCDRGB_LCD_BLINKON 0x01
#define GROVE_LCDRGB_LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define GROVE_LCDRGB_LCD_DISPLAYMOVE 0x08
#define GROVE_LCDRGB_LCD_CURSORMOVE 0x00
#define GROVE_LCDRGB_LCD_MOVERIGHT 0x04
#define GROVE_LCDRGB_LCD_MOVELEFT 0x00

// flags for function set
#define GROVE_LCDRGB_LCD_8BITMODE 0x10
#define GROVE_LCDRGB_LCD_4BITMODE 0x00
#define GROVE_LCDRGB_LCD_2LINE 0x08
#define GROVE_LCDRGB_LCD_1LINE 0x00
#define GROVE_LCDRGB_LCD_5x10DOTS 0x04
#define GROVE_LCDRGB_LCD_5x8DOTS 0x00


void GROVE_LCDRGB_Init(uint8_t cols, uint8_t lines, uint8_t dotsize);
void GROVE_LCDRGB_Clear();
void GROVE_LCDRGB_Home();
void GROVE_LCDRGB_SetCursor(uint8_t col, uint8_t row);

// Turn the display on/off (quickly)
void GROVE_LCDRGB_NoDisplay();
void GROVE_LCDRGB_Display();
// Turns the underline cursor on/off
void GROVE_LCDRGB_NoCursor();
void GROVE_LCDRGB_Cursor();

// Turn on and off the blinking cursor
void GROVE_LCDRGB_NoBlink();
void GROVE_LCDRGB_Blink();
// These commands scroll the display without changing the RAM
void GROVE_LCDRGB_ScrollDisplayLeft(void);
void GROVE_LCDRGB_ScrollDisplayRight(void);

// This is for text that flows Left to Right
void GROVE_LCDRGB_LeftToRight(void);
// This is for text that flows Right to Left
void GROVE_LCDRGB_RightToLeft(void);

// This will 'right justify' text from the cursor
void GROVE_LCDRGB_AutoScroll(void);
// This will 'left justify' text from the cursor
void GROVE_LCDRGB_NoAutoScroll(void);
// Allows us to fill the first 8 CGRAM locations
// with custom characters
void GROVE_LCDRGB_CreateChar(uint8_t location, uint8_t charmap[]);
/*********** mid level commands, for sending data/cmds */

// send command
void GROVE_LCDRGB_Command(uint8_t value);
// send data
void GROVE_LCDRGB_Write(uint8_t value);
void GROVE_LCDRGB_Print(char *str);
void GROVE_LCDRGB_SetRGB(uint8_t r, uint8_t g, uint8_t b);
void GROVE_LCDRGB_SetRGB_R(uint8_t r);
void GROVE_LCDRGB_SetRGB_G(uint8_t g);
void GROVE_LCDRGB_SetRGB_B(uint8_t b);
void GROVE_LCDRGB_SetPWM(uint8_t color, uint8_t pwm);

#endif

