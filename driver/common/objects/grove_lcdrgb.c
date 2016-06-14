#include "grove_lcdrgb.h"
#include "../i2c/i2c.h"
#include "../generic/generic.h"


static uint8_t _displaycontrol;
static uint8_t _displaymode;

void i2c_send_byte(uint8_t dta)
{
	AIR_I2C_Write(GROVE_LCDRGB_LCD_ADDRESS,&dta,1);
}

void i2c_send_byteS(uint8_t *dta, uint8_t len)
{
	AIR_I2C_Write(GROVE_LCDRGB_LCD_ADDRESS,dta,len);
}

void rgb_lcd_set_reg(uint8_t addr, uint8_t data)
{
	uint8_t dta[] = {addr, data};
	AIR_I2C_Write(GROVE_LCDRGB_RGB_ADDRESS,dta,2);
}

void GROVE_LCDRGB_Init(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	uint8_t _displayfunction = 0;

	if (lines > 1) {
		_displayfunction |= GROVE_LCDRGB_LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= GROVE_LCDRGB_LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	// If this is required for a particular platform we would recommend they put it in their setup before starting.
// 	AIR_GENERIC_UDelay(5000000);


	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23

	// Send function set rgb_lcd_command sequence
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_FUNCTIONSET | _displayfunction);
	AIR_GENERIC_UDelay(4500);

	// second try
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_FUNCTIONSET | _displayfunction);
	AIR_GENERIC_UDelay(150);

	// third go
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_FUNCTIONSET | _displayfunction);


	// finally, set # lines, font size, etc.
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_FUNCTIONSET | _displayfunction);

	// turn the rgb_lcd_display on with no cursor or blinking default
	_displaycontrol = GROVE_LCDRGB_LCD_DISPLAYON | GROVE_LCDRGB_LCD_CURSOROFF | GROVE_LCDRGB_LCD_BLINKOFF;
	GROVE_LCDRGB_Display();

	// GROVE_LCDRGB_Clear it off
	GROVE_LCDRGB_Clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = GROVE_LCDRGB_LCD_ENTRYLEFT | GROVE_LCDRGB_LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_ENTRYMODESET | _displaymode);


	// backlight init
	rgb_lcd_set_reg(0, 0);
	rgb_lcd_set_reg(1, 0);
	rgb_lcd_set_reg(0x08, 0xAA);	 // all led control by pwm

	GROVE_LCDRGB_SetCursor(0, 0);

}

/********** high level commands, for the user! */
void GROVE_LCDRGB_Clear()
{
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_CLEARDISPLAY);		// rgb_lcd_clear rgb_lcd_display, set cursor position to zero
	AIR_GENERIC_UDelay(2000);
}

void GROVE_LCDRGB_Home()
{
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_RETURNHOME);		// set cursor position to zero
	AIR_GENERIC_UDelay(2000);
}

void GROVE_LCDRGB_SetCursor(uint8_t col, uint8_t row)
{
	col = (row == 0 ? col|0x80 : col|0xc0);
	uint8_t dta[2] = {0x80, col};
	i2c_send_byteS(dta, 2);
}

// Turn the rgb_lcd_display on/off (quickly)
void GROVE_LCDRGB_NoDisplay()
{
	_displaycontrol &= ~GROVE_LCDRGB_LCD_DISPLAYON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

void GROVE_LCDRGB_Display() {
	_displaycontrol |= GROVE_LCDRGB_LCD_DISPLAYON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void GROVE_LCDRGB_NoCursor()
{
	_displaycontrol &= ~GROVE_LCDRGB_LCD_CURSORON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

void GROVE_LCDRGB_Cursor() {
	_displaycontrol |= GROVE_LCDRGB_LCD_CURSORON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void GROVE_LCDRGB_NoBlink()
{
	_displaycontrol &= ~GROVE_LCDRGB_LCD_BLINKON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

void GROVE_LCDRGB_Blink()
{
	_displaycontrol |= GROVE_LCDRGB_LCD_BLINKON;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the rgb_lcd_display without changing the RAM
void GROVE_LCDRGB_ScrollDisplayLeft(void)
{
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_CURSORSHIFT | GROVE_LCDRGB_LCD_DISPLAYMOVE | GROVE_LCDRGB_LCD_MOVELEFT);
}
void GROVE_LCDRGB_ScrollDisplayRight(void)
{
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_CURSORSHIFT | GROVE_LCDRGB_LCD_DISPLAYMOVE | GROVE_LCDRGB_LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void GROVE_LCDRGB_LeftToRight(void)
{
	_displaymode |= GROVE_LCDRGB_LCD_ENTRYLEFT;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void GROVE_LCDRGB_RightToLeft(void)
{
	_displaymode &= ~GROVE_LCDRGB_LCD_ENTRYLEFT;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void GROVE_LCDRGB_AutoScroll(void)
{
	_displaymode |= GROVE_LCDRGB_LCD_ENTRYSHIFTINCREMENT;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void GROVE_LCDRGB_NoAutoScroll(void)
{
	_displaymode &= ~GROVE_LCDRGB_LCD_ENTRYSHIFTINCREMENT;
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void GROVE_LCDRGB_CreateChar(uint8_t location, uint8_t charmap[])
{

	location &= 0x7; // we only have 8 locations 0-7
	GROVE_LCDRGB_Command(GROVE_LCDRGB_LCD_SETCGRAMADDR | (location << 3));


	uint8_t dta[9];
	dta[0] = 0x40;
	int i;
	for(i=0; i<8; i++)
	{
		dta[i+1] = charmap[i];
	}
	i2c_send_byteS(dta, 9);
}

/*********** mid level commands, for sending data/cmds */

// send rgb_lcd_command
void GROVE_LCDRGB_Command(uint8_t value)
{
	uint8_t dta[2] = {0x80, value};
	i2c_send_byteS(dta, 2);
}

// send data
void GROVE_LCDRGB_Write(uint8_t value)
{
	uint8_t dta[2] = {0x40, value};
	i2c_send_byteS(dta, 2);
}

void GROVE_LCDRGB_Print(char *str)
{
	while(*str)
	{
		GROVE_LCDRGB_Write(*str);
		str++;
	}
}

void GROVE_LCDRGB_SetRGB(uint8_t r, uint8_t g, uint8_t b)
{
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_RED, r);
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_GREEN, g);
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_BLUE, b);
}

void GROVE_LCDRGB_SetRGB_R(uint8_t r)
{
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_RED, r);
}

void GROVE_LCDRGB_SetRGB_G(uint8_t g)
{
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_GREEN, g);
}

void GROVE_LCDRGB_SetRGB_B(uint8_t b)
{
	rgb_lcd_set_reg(GROVE_LCDRGB_REG_BLUE, b);
}

void GROVE_LCDRGB_SetPWM(uint8_t color, uint8_t pwm)
{
	rgb_lcd_set_reg(color, pwm);
}

const uint8_t color_define[4][3] =
{
	{255, 255, 255},			// white
	{255, 0, 0},				// red
	{0, 255, 0},				// green
	{0, 0, 255},				// blue
};

void rgb_lcd_setColor(uint8_t color)
{
	if(color > 3)return ;
	GROVE_LCDRGB_SetRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
}
