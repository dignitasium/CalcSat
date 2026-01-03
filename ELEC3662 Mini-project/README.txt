CALCSAT – TM4C123GH6PM SCIENTIFIC CALCULATOR  
===========================================  

Project Summary  
---------------  
CalcSat is a feature-rich scientific-style calculator implemented on the TI TM4C123GH6PM LaunchPad.  
It combines a floating-point calculator with operator precedence, full memory functions, and a set of hidden games and easter eggs, all controlled via a 4×4 keypad and displayed on a 16×2 character LCD.  

***

1. Features Overview  
--------------------  

- Floating-point arithmetic: +, -, ×, ÷  
- Scientific notation using ×10^n (E key)  
- Operator precedence (PEMDAS) using a two-pass evaluation  
- Calculator-style memory functions: MS, MR, MC, M+, M-  
- Shift key for extended operations and memory access  
- Expression and result display on a 16×2 LCD  
- Error handling (e.g. divide-by-zero) with clear messages  
- Easter-egg codes and several mini-games activated from the calculator  

***

2. Hardware Connections  
-----------------------  

Target platform: **TI Tiva C TM4C123GH6PM LaunchPad**  
Peripherals: **16×2 HD44780 LCD** and **4×4 matrix keypad**  

LCD (4-bit mode)  
- D4–D7  ? Port B, pins PB4–PB7  
- RS     ? Port A, e.g. PA3  
- EN     ? Port A, e.g. PA2  
- RW     ? GND (write-only)  
- VCC    ? 5 V  
- VSS    ? GND  
- V0     ? Contrast via ~10 kO potentiometer between VCC and GND  

Keypad (4×4 matrix)  
- Rows   ? GPIO outputs, e.g. PE0–PE3  
- Columns? GPIO inputs with pull-ups, e.g. PB0–PB3  

Power  
- USB on the LaunchPad supplies power.  
- Ensure all modules share a common ground.  

(If your wiring differs, update `pin_definitions.h` to match.)  

***

3. Keypad Layout and Key Mappings  
---------------------------------  

Physical layout (logical view):  

    R1:  1   2   3   A  
    R2:  4   5   6   B  
    R3:  7   8   9   C  
    R4:  *   0   #   D  

NORMAL MODE  
- Digits: `0–9`     ? Number entry  
- `A`              ? `+`  
- `B`              ? `-`  
- `C`              ? Decimal point `.`  
- `D`              ? Toggle Shift mode  
- `*`              ? Equals  
- `#`              ? Backspace (delete last digit)  

SHIFTED MODE (press `D` once, then key)  
- `A` ? `×`  
- `B` ? `÷`  
- `C` ? `E` (×10^n scientific notation)  
- `#` ? Clear current entry (CE)  

Memory functions (Shift + digit)  
- `Shift + 1` ? MS  (Memory Store)  
- `Shift + 2` ? MR  (Memory Recall)  
- `Shift + 3` ? MC  (Memory Clear)  
- `Shift + 4` ? M+  (Memory Add)  
- `Shift + 5` ? M-  (Memory Subtract)  

Easter eggs and games  
- Enter a special number, then press `*` to trigger easter-egg messages or launch a mini-game.  

***

4. Project Structure  
--------------------  

Top-level layout (folders may vary slightly by IDE):  

- `src/`  
  - `main.c`  
    - System entry point, hardware initialisation, main loop, game activation logic.  
  - `calculator.c`  
    - Calculator state machine, input handling, operator precedence, memory operations, display updates.  
  - `lcd.c`  
    - 16×2 LCD driver in 4-bit mode (initialisation, command and data writes).  
  - `keypad.c`  
    - 4×4 keypad scan routine and mapping from row/column to characters.  
  - `system.c`  
    - System clock configuration, delay functions, board-level initialisation.  
  - `splash.c`  
    - Startup splash animations and title screens.  
  - `games.c`  
    - Easter-egg messages and mini-games, plus activation-code detection.  

- `inc/`  
  - `calculator.h`, `lcd.h`, `keypad.h`, `system.h`, `splash.h`, `games.h`  

- `config/`  
  - `pin_definitions.h` – all LCD and keypad pin mappings.  

- `Device/`  
  - Startup and system files for the TM4C123GH6PM microcontroller.  

***

5. Technical Details  
--------------------  

5.1 LCD Communication (4-bit HD44780)  
- Data is sent in **two nibbles**: upper 4 bits, then lower 4.  
- RS selects command (0) or data (1).  
- EN is pulsed high for each nibble to latch the value.  
- RW is tied low; the firmware never reads the busy flag, it uses delays.  
- Initialisation sequence:  
  1. Power-up delay.  
  2. Function set in 4-bit, 2-line mode.  
  3. Display on, cursor settings.  
  4. Clear display and set entry mode.  

5.2 Keypad Scanning  
- Rows configured as **outputs**; columns as **inputs with pull-ups**.  
- Scan algorithm:  
  1. Set all rows high.  
  2. For each row:  
     - Drive that row low.  
     - Read all columns.  
     - Any column reading low indicates a pressed key at (row, column).  
  3. Translate (row, column) into a character (`'0'–'9'`, `'A'–'D'`, `'*'`, `'#'`).  

5.3 Timing and Debouncing  
- A millisecond delay routine (`Delay_ms`) is used for:  
  - LCD command timing (clear, home, data writes).  
  - Keypad debounce.  
- Main loop behaviour:  
  - Read the keypad roughly every 50 ms.  
  - Compare the current key with `last_key`.  
  - Only act when `key != 0` and `key != last_key` (edge detection).  
- This prevents repeated processing while a key is held.  

5.4 Calculator Engine and Memory  
- All calculator state is stored in a `Calculator` struct.  
- Operands and operators are pushed into arrays as the user types.  
- On `=`, a two-pass evaluation runs:  
  - Pass 1: handle `×`, `÷`, and `E` (×10^n).  
  - Pass 2: handle `+` and `-`.  
- Memory register:  
  - MS/MR/MC/M+/M- operate on a single floating-point `memory` value.  
  - The first LCD line shows `M:` and the memory value whenever memory is non-zero.  

***

6. Building the Project  
-----------------------  

1. Create a new TM4C123GH6PM project in your IDE (e.g. Keil µVision, Code Composer Studio).  
2. Add all `.c` files from `src/` to the project.  
3. Add the `inc/` folder to the compiler’s include path.  
4. Ensure the correct startup and system files for TM4C123GH6PM are included (in `Device/`).  
5. Verify that `pin_definitions.h` matches your actual wiring for LCD and keypad.  
6. Build the project; resolve any missing paths or warnings.  
7. Flash the generated binary to the LaunchPad through the on-board debugger.  

***

7. Testing Guide  
----------------  

Power-on tests  
- On reset, the splash screen should appear, then the main calculator display should show `0`.  

Basic arithmetic  
- `2 + 3 *` ? expect result `5`.  
- `7 - 4 *` ? expect result `3`.  

Precedence (PEMDAS)  
- `2 + 3 × 4 *`  
  - Enter: `2 A 3 D A 4 *`  
  - Expect: `14` (3 × 4 = 12, then 2 + 12).  

Scientific notation  
- Enter `1`, `D`, `C`, `3`, `*` ? expect `1000` (1 × 10^3).  

Memory functions  
- Store: enter a value, then `D 1` (MS).  
- Recall: `D 2` (MR) should bring the stored value to the display.  
- Add: `D 4` (M+) should accumulate the current value into memory.  
- Subtract: `D 5` (M-) should subtract from memory.  
- Clear: `D 3` (MC) should reset memory to 0 and remove the memory indicator.  

Easter eggs and games  
- Enter codes like `42 *`, `1337 *`, `5318008 *`, `4321 *` and confirm that messages or games appear and the calculator resets correctly afterwards.  

***

8. Troubleshooting  
------------------  

No LCD text or random glyphs  
- Check LCD power, ground, and contrast.  
- Confirm that LCD pins match the configuration in `pin_definitions.h`.  
- Make sure `LCD_Init()` is called before any other LCD function.  

Keys not detected or wrong characters  
- Verify row and column wiring for the keypad.  
- Ensure rows are outputs and columns are inputs with pull-ups.  
- If one key produces the wrong character, review the row/column to key mapping table in `keypad.c`.  

Multiple key presses from one tap  
- Increase the debounce delay (`Delay_ms(50)` in the main loop).  
- Confirm that the `last_key` mechanism is still present and not modified.  

Calculator stuck showing an error  
- After an error message (e.g. divide-by-zero), press any non-shift key to clear and re-initialise the calculator.  

Games never start  
- Check that you typed the exact activation code followed by `*`.  
- Make sure `Games_CheckActivation` and `Games_Launch` are being called from `main.c` when `*` is pressed.  

***