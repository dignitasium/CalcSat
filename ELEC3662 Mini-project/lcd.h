/*
 * LCD Driver Header
 * 
 * LCD Connections:
 *   PA2 = EN (Enable)
 *   PA3 = RS (Register Select)
 *   PB4-7 = DB4-7 (LCD Data)
 */

#ifndef LCD_H
#define LCD_H

// Function declarations
void LCD_Init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_String(char* str);
void LCD_Clear(void);
void LCD_SetCursor(unsigned char row, unsigned char col);

// Common LCD Commands
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRY_MODE      0x06
#define LCD_DISPLAY_OFF     0x08
#define LCD_DISPLAY_ON      0x0C
#define LCD_CURSOR_BLINK    0x0F
#define LCD_SHIFT_LEFT      0x18
#define LCD_SHIFT_RIGHT     0x1C
#define LCD_LINE1           0x80
#define LCD_LINE2           0xC0

#endif // LCD_H