/*
 * Keypad Driver Header
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

#ifndef KEYPAD_H
#define KEYPAD_H

// Function declarations
void Keypad_Init(void);
char ReadKey(void);
char WaitForKey(void);

// Key definitions
#define KEY_1    '1'
#define KEY_2    '2'
#define KEY_3    '3'
#define KEY_4    '4'
#define KEY_5    '5'
#define KEY_6    '6'
#define KEY_7    '7'
#define KEY_8    '8'
#define KEY_9    '9'
#define KEY_0    '0'
#define KEY_A    'A'
#define KEY_B    'B'
#define KEY_C    'C'
#define KEY_D    'D'
#define KEY_STAR '*'
#define KEY_HASH '#'

#endif // KEYPAD_H