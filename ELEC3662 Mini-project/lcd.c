/*
 * LCD Driver Implementation
 * 
 * LCD Connections:
 *   PA2 = EN (Enable)
 *   PA3 = RS (Register Select)
 *   PB4-7 = DB4-7 (LCD Data)
 */

#include "lcd.h"
#include "pin_definitions.h"
#include "system.h"

void LCD_Cmd(unsigned char cmd) {
    GPIO_PORTA_DATA_R &= ~0x08;  // RS = 0 for command
    
    // Send high nibble
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | (cmd & 0xF0);
    GPIO_PORTA_DATA_R |= 0x04;   // EN = 1
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;  // EN = 0
    
    // Send low nibble
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | ((cmd << 4) & 0xF0);
    GPIO_PORTA_DATA_R |= 0x04;   // EN = 1
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;  // EN = 0
    
    Delay_ms(2);
}

void LCD_Char(unsigned char data) {
    GPIO_PORTA_DATA_R |= 0x08;  // RS = 1 for data
    
    // Send high nibble
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | (data & 0xF0);
    GPIO_PORTA_DATA_R |= 0x04;   // EN = 1
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;  // EN = 0
    
    // Send low nibble
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | ((data << 4) & 0xF0);
    GPIO_PORTA_DATA_R |= 0x04;   // EN = 1
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;  // EN = 0
    
    Delay_ms(2);
}

void LCD_Init(void) {
    volatile unsigned long delay;
    
    // Enable clocks for Port A and Port B
    SYSCTL_RCGC2_R |= 0x03;  // Enable Port A and B
    delay = SYSCTL_RCGC2_R;
    
    // Port A: LCD Control (PA2=EN, PA3=RS)
    GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTA_CR_R = 0xFF;
    GPIO_PORTA_AMSEL_R = 0x00;
    GPIO_PORTA_PCTL_R = 0x00;
    GPIO_PORTA_DIR_R = 0x0C;      // PA2, PA3 as outputs
    GPIO_PORTA_AFSEL_R = 0x00;
    GPIO_PORTA_DEN_R = 0x0C;      // Enable PA2, PA3
    GPIO_PORTA_DATA_R = 0x00;
    
    // Port B: LCD Data (PB4-7) + Keypad Columns (PB0-3)
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R = 0xFF;
    GPIO_PORTB_AMSEL_R = 0x00;
    GPIO_PORTB_PCTL_R = 0x00;
    GPIO_PORTB_DIR_R = 0xFF;      // All outputs
    GPIO_PORTB_AFSEL_R = 0x00;
    GPIO_PORTB_DEN_R = 0xFF;      // Enable all pins
    GPIO_PORTB_DATA_R = 0x00;
    
    // LCD Initialization sequence
    Delay_ms(50);
    GPIO_PORTA_DATA_R &= ~0x0C;  // EN=0, RS=0
    
    // 8-bit mode initialization
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | 0x30;
    GPIO_PORTA_DATA_R |= 0x04;
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;
    Delay_ms(5);
    
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | 0x30;
    GPIO_PORTA_DATA_R |= 0x04;
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;
    Delay_ms(1);
    
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | 0x30;
    GPIO_PORTA_DATA_R |= 0x04;
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;
    Delay_ms(1);
    
    // Switch to 4-bit mode
    GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & 0x0F) | 0x20;
    GPIO_PORTA_DATA_R |= 0x04;
    Delay_ms(1);
    GPIO_PORTA_DATA_R &= ~0x04;
    Delay_ms(1);
    
    // Configure LCD
    LCD_Cmd(0x28);  // 4-bit mode, 2 lines, 5x8 font
    LCD_Cmd(0x0C);  // Display ON, cursor OFF
    LCD_Cmd(0x01);  // Clear display
    Delay_ms(2);
    LCD_Cmd(0x06);  // Entry mode: increment cursor
}

void LCD_String(char* str) {
    while(*str) {
        LCD_Char(*str++);
    }
}

void LCD_Clear(void) {
    LCD_Cmd(0x01);
    Delay_ms(2);
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
    unsigned char address;
    if(row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }
    LCD_Cmd(address);
}