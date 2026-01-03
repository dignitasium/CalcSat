/*
 * System Utilities Implementation
 * Provides timing and system initialization functions
 */

#include "system.h"
#include "pin_definitions.h"

void Delay_ms(unsigned long ms) {
    unsigned long i;
    for(i = 0; i < ms; i++) {
        NVIC_ST_RELOAD_R = 80000 - 1;  // 1ms at 80MHz (assuming 80MHz clock after PLL)
        NVIC_ST_CURRENT_R = 0;
        while((NVIC_ST_CTRL_R & 0x00010000) == 0);
    }
}

void Delay_us(unsigned long us) {
    unsigned long i;
    for(i = 0; i < us; i++) {
        NVIC_ST_RELOAD_R = 80 - 1;  // 1us at 80MHz
        NVIC_ST_CURRENT_R = 0;
        while((NVIC_ST_CTRL_R & 0x00010000) == 0);
    }
}

void System_Init(void) {
    volatile unsigned long delay;
    
    // Enable clocks for all required ports
    SYSCTL_RCGC2_R |= 0x1B;  // Enable Port A, B, D, E
    delay = SYSCTL_RCGC2_R;  // Wait for clock to stabilize
    
    // Initialize SysTick timer
    NVIC_ST_CTRL_R = 0;              // Disable SysTick during setup
    NVIC_ST_RELOAD_R = 0x00FFFFFF;   // Maximum reload value
    NVIC_ST_CURRENT_R = 0;           // Clear current value
    NVIC_ST_CTRL_R = 0x05;           // Enable SysTick with core clock
}

unsigned long millis(void) {
    // Returns approximate milliseconds since startup
    // Note: This is a simple implementation. For accurate timekeeping,
    // you would need to track timer overflows
    return NVIC_ST_CURRENT_R / 80000;
}