# LCD and Keypad Interface Project

## Overview
This project implements a 16x2 LCD display interface and 4x4 matrix keypad driver for the TM4C123GH6PM microcontroller.

## Hardware Connections

### LCD (16x2 HD44780 compatible)
- **PA2**: LCD Enable (EN)
- **PA3**: LCD Register Select (RS)
- **PB4-7**: LCD Data lines (DB4-DB7) - 4-bit mode

### Keypad (4x4 Matrix)
- **PB0-3**: Columns (Outputs)
- **PE0-3**: Rows (Inputs with pull-down resistors)

### Keypad Layout
```
1  2  3  A
4  5  6  B
7  8  9  C
*  0  #  D
```

## Project Structure

```
project/
+-- src/
¦   +-- main.c              - Main application
¦   +-- lcd.c               - LCD driver implementation
¦   +-- keypad.c            - Keypad driver implementation
¦   +-- system.c            - System utilities (delays, init)
+-- inc/
¦   +-- lcd.h               - LCD driver header
¦   +-- keypad.h            - Keypad driver header
¦   +-- system.h            - System utilities header
¦   +-- pin_definitions.h   - Hardware register definitions
+-- README.md
```

## File Descriptions

### Main Application (main.c)
- Initializes system, LCD, and keypad
- Displays "Press Key:" on the LCD
- Continuously scans keypad and displays pressed keys

### LCD Driver (lcd.c / lcd.h)
**Functions:**
- `LCD_Init()` - Initialize LCD in 4-bit mode
- `LCD_Cmd(cmd)` - Send command to LCD
- `LCD_Char(data)` - Display single character
- `LCD_String(str)` - Display string
- `LCD_Clear()` - Clear display
- `LCD_SetCursor(row, col)` - Set cursor position

**Common Commands:**
- `LCD_CLEAR` - Clear display
- `LCD_LINE1` - Move to line 1
- `LCD_LINE2` - Move to line 2

### Keypad Driver (keypad.c / keypad.h)
**Functions:**
- `Keypad_Init()` - Initialize keypad GPIO
- `ReadKey()` - Scan keypad, return pressed key (0 if none)
- `WaitForKey()` - Block until key pressed and released

**Returns:**
- Characters: '0'-'9', 'A'-'D', '*', '#'
- 0 if no key pressed

### System Utilities (system.c / system.h)
**Functions:**
- `System_Init()` - Initialize clocks and SysTick
- `Delay_ms(ms)` - Millisecond delay
- `Delay_us(us)` - Microsecond delay
- `millis()` - Get approximate milliseconds since startup

### Pin Definitions (pin_definitions.h)
- All TM4C123 register addresses
- Pin masks and definitions
- Hardware abstraction macros

## How to Use

### Basic LCD Usage
```c
LCD_Init();
LCD_String("Hello World!");
LCD_SetCursor(1, 0);  // Move to line 2
LCD_String("Line 2");
```

### Basic Keypad Usage
```c
Keypad_Init();

// Method 1: Non-blocking
char key = ReadKey();
if(key != 0) {
    // Key was pressed
}

// Method 2: Blocking
char key = WaitForKey();  // Waits until key pressed
```

## Technical Details

### LCD Communication
- 4-bit parallel interface
- Standard HD44780 timing requirements
- Initialize sequence follows datasheet

### Keypad Scanning
- Column scanning method
- Pull-down resistors on row inputs
- 5ms scan delay per column
- Debouncing in WaitForKey()

### Timing
- System runs at 80MHz (assuming PLL enabled)
- SysTick provides accurate timing
- 1ms delay = 80,000 clock cycles

## Building the Project

1. Add all `.c` files to your Keil/CCS project
2. Include all `.h` files in your include path
3. Configure for TM4C123GH6PM target
4. Build and flash to microcontroller

## Testing

1. Connect LCD and keypad as per pin definitions
2. Power on the board
3. LCD should display "Press Key:"
4. Press any key on keypad
5. Key should appear on LCD line 2

## Troubleshooting

**LCD shows nothing:**
- Check contrast potentiometer
- Verify connections (EN, RS, data pins)
- Check 5V power supply

**LCD shows blocks:**
- Contrast too high
- Initialization issue

**Keypad not responding:**
- Check column outputs (PB0-3)
- Verify row inputs (PE0-3) have pull-down enabled
- Check for short circuits in matrix

**Wrong keys displayed:**
- Verify row/column connections
- Check keypad matrix wiring
- Confirm pull-down resistors on PE0-3

## Notes

- Port B pins are shared between LCD data (upper nibble) and keypad columns (lower nibble)
- Care is taken in code to preserve LCD data when scanning keypad
- All timing requirements for LCD met with Delay_ms()
- System assumes 80MHz clock - adjust delays if different

## License
Free to use for educational purposes.