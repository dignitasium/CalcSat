/*
 * Keypad Driver Implementation
 * 
 * Keypad Connections:
 *   PB0-3 = Columns (OUTPUTS)
 *   PE0-3 = Rows (INPUTS with pull-down)
 * 
 * Keypad Layout:
 *   1 2 3 A
 *   4 5 6 B
 *   7 8 9 C
 *   * 0 # D
 */

#include "keypad.h"
#include "pin_definitions.h"
#include "system.h"

void Keypad_Init(void) {
    volatile unsigned long delay;
    
    // Enable clock for Port E
    SYSCTL_RCGC2_R |= 0x10;  // Enable Port E
    delay = SYSCTL_RCGC2_R;
    
    // Port E: Keypad Rows (PE0-3 inputs with pull-down)
    GPIO_PORTE_LOCK_R = 0x4C4F434B;
    GPIO_PORTE_CR_R = 0x0F;
    GPIO_PORTE_AMSEL_R = 0x00;
    GPIO_PORTE_PCTL_R = 0x00;
    GPIO_PORTE_DIR_R = 0x00;      // PE0-3 as inputs
    GPIO_PORTE_AFSEL_R = 0x00;
    GPIO_PORTE_PDR_R = 0x0F;      // Enable pull-down resistors
    GPIO_PORTE_DEN_R = 0x0F;      // Enable PE0-3
}

char ReadKey(void) {
    unsigned char pe_val;
    unsigned char pb_current;
    
    // Scan Column 0 (PB0)
    pb_current = GPIO_PORTB_DATA_R & 0xF0;  // Save LCD bits (PB4-7)
    GPIO_PORTB_DATA_R = pb_current | 0x01;   // Set PB0 HIGH
    Delay_ms(5);
    pe_val = GPIO_PORTE_DATA_R & 0x0F;
    if(pe_val == 0x01) return '1';
    if(pe_val == 0x02) return '4';
    if(pe_val == 0x04) return '7';
    if(pe_val == 0x08) return '*';
    
    // Scan Column 1 (PB1)
    pb_current = GPIO_PORTB_DATA_R & 0xF0;
    GPIO_PORTB_DATA_R = pb_current | 0x02;   // Set PB1 HIGH
    Delay_ms(5);
    pe_val = GPIO_PORTE_DATA_R & 0x0F;
    if(pe_val == 0x01) return '2';
    if(pe_val == 0x02) return '5';
    if(pe_val == 0x04) return '8';
    if(pe_val == 0x08) return '0';
    
    // Scan Column 2 (PB2)
    pb_current = GPIO_PORTB_DATA_R & 0xF0;
    GPIO_PORTB_DATA_R = pb_current | 0x04;   // Set PB2 HIGH
    Delay_ms(5);
    pe_val = GPIO_PORTE_DATA_R & 0x0F;
    if(pe_val == 0x01) return '3';
    if(pe_val == 0x02) return '6';
    if(pe_val == 0x04) return '9';
    if(pe_val == 0x08) return '#';
    
    // Scan Column 3 (PB3)
    pb_current = GPIO_PORTB_DATA_R & 0xF0;
    GPIO_PORTB_DATA_R = pb_current | 0x08;   // Set PB3 HIGH
    Delay_ms(5);
    pe_val = GPIO_PORTE_DATA_R & 0x0F;
    if(pe_val == 0x01) return 'A';
    if(pe_val == 0x02) return 'B';
    if(pe_val == 0x04) return 'C';
    if(pe_val == 0x08) return 'D';
    
    // Clear all column outputs (PB0-3)
    GPIO_PORTB_DATA_R = GPIO_PORTB_DATA_R & 0xF0;
    
    return 0;  // No key pressed
}

char WaitForKey(void) {
    char key;
    
    // Wait for key press
    do {
        key = ReadKey();
    } while(key == 0);
    
    // Wait for key release (debounce)
    Delay_ms(50);
    while(ReadKey() != 0);
    Delay_ms(50);
    
    return key;
}