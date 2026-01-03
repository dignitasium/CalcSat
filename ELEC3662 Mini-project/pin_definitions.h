/*
 * Pin Definitions and Register Addresses
 * TM4C123GH6PM Microcontroller
 */

#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

// System Control Registers
#define SYSCTL_RCGC2_R      (*((volatile unsigned long *)0x400FE108))

// SysTick Timer Registers
#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))

// GPIO Port A Registers
#define GPIO_PORTA_LOCK_R   (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R     (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R  (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_DIR_R    (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_DEN_R    (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_AFSEL_R  (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_PCTL_R   (*((volatile unsigned long *)0x4000452C))
#define GPIO_PORTA_DATA_R   (*((volatile unsigned long *)0x400043FC))

// GPIO Port B Registers
#define GPIO_PORTB_LOCK_R   (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R     (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R  (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_DIR_R    (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_DEN_R    (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AFSEL_R  (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PCTL_R   (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_DATA_R   (*((volatile unsigned long *)0x400053FC))

// GPIO Port E Registers
#define GPIO_PORTE_LOCK_R   (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R     (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R  (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_DIR_R    (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_DEN_R    (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AFSEL_R  (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PCTL_R   (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_PDR_R    (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_PUR_R    (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DATA_R   (*((volatile unsigned long *)0x400243FC))

// Pin Definitions
// LCD Control Pins (Port A)
#define LCD_EN_PIN      0x04    // PA2
#define LCD_RS_PIN      0x08    // PA3

// LCD Data Pins (Port B upper nibble)
#define LCD_DATA_MASK   0xF0    // PB4-7

// Keypad Column Pins (Port B lower nibble)
#define KEYPAD_COL_MASK 0x0F    // PB0-3
#define KEYPAD_COL0     0x01    // PB0
#define KEYPAD_COL1     0x02    // PB1
#define KEYPAD_COL2     0x04    // PB2
#define KEYPAD_COL3     0x08    // PB3

// Keypad Row Pins (Port E)
#define KEYPAD_ROW_MASK 0x0F    // PE0-3
#define KEYPAD_ROW0     0x01    // PE0
#define KEYPAD_ROW1     0x02    // PE1
#define KEYPAD_ROW2     0x04    // PE2
#define KEYPAD_ROW3     0x08    // PE3

#endif // PIN_DEFINITIONS_H