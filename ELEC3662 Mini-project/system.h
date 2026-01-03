/*
 * System Utilities Header
 * Provides timing and system initialization functions
 */

#ifndef SYSTEM_H
#define SYSTEM_H

// Function declarations
void System_Init(void);
void Delay_ms(unsigned long ms);
void Delay_us(unsigned long us);
unsigned long millis(void);

#endif // SYSTEM_H