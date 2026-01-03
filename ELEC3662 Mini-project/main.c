/*
 * Calculator Application with Games & Easter Eggs
 *
 * Hardware platform: TM4C123GH6PM LaunchPad
 *
 * Peripherals:
 *   LCD    - PA2 (EN), PA3 (RS), PB4–PB7 (data bus)
 *   Keypad - PB0–PB3 (columns), PE0–PE3 (rows)
 *
 * Key Mappings:
 *   Normal:  A = +    B = -    C = .    D = Shift   * = Equals   # = Backspace
 *   Shifted: A = ×    B = ÷    C = E    D = Cancel  # = Clear entry
 *
 * Memory functions (implemented in calculator.c):
 *   Shift+1 = MS  (memory store)
 *   Shift+2 = MR  (memory recall)
 *   Shift+3 = MC  (memory clear)
 *   Shift+4 = M+  (add to memory)
 *   Shift+5 = M-  (subtract from memory)
 *
 * Easter eggs and games:
 *   Enter the numbers below and press equals:
 *     42     -> "Answer to Life" message
 *     1337   -> Leet mode
 *     404    -> Error 404 message
 *     314159 -> Pi digits
 *     73     -> Ham radio greeting
 *     8008135-> Classic calculator gag
 *
 *   Game activation codes:
 *     5318008 -> Snake game
 *     7734    -> Math quiz
 *     5373    -> Number guessing
 *     9009    -> Pong
 *     1234    -> Reaction test
 *     4321    -> Memory game
 *
 * The main loop scans the keypad, debounces key presses, and forwards
 * valid keys to the calculator module. When a game code is entered,
 * control is passed to the games module and the calculator is reset.
 */

#include "lcd.h"
#include "keypad.h"
#include "system.h"
#include "calculator.h"
#include "splash.h"
#include "games.h"

int main(void) {
    // Configure system clock and enable GPIO peripherals
    System_Init();

    // Initialise LCD in 4-bit mode and clear display
    LCD_Init();

    // Configure keypad GPIO directions and internal pull-ups
    Keypad_Init();

    // Display animated splash screen on power-up
    // Options: SPLASH_SATELLITE, SPLASH_M0LSC, SPLASH_ROCKET,
    //          SPLASH_LOADING_BAR, SPLASH_MATRIX, SPLASH_WAVE
    Splash_Show(SPLASH_SATELLITE);

    // Create and initialise calculator context
    Calculator calc;
    Calculator_Init(&calc);

    // Create and initialise global game state
    GameState game;
    Games_Init(&game);

    // Draw initial calculator screen
    Calculator_DisplayUpdate(&calc);

    char last_key = 0;   // Used for simple edge detection (press vs hold)

    while (1) {
        // Read current key from keypad driver (0 means "no key pressed")
        char key = ReadKey();

        // Only act on a new key press (ignore repeats while held)
        if (key != 0 && key != last_key) {
            // When equals is pressed, first check for easter eggs or games
            if (key == '*') {
                // Display fun messages for special constants
                Games_CheckEasterEgg(calc.expression);

                // Check if expression matches any game activation code
                int game_type = Games_CheckActivation(calc.expression);
                if (game_type != 0) {
                    // Launch selected game and temporarily leave calculator mode
                    Games_Launch(&game, (GameType)game_type);

                    // Reset calculator state when returning from a game
                    Calculator_Clear(&calc);
                    Calculator_DisplayUpdate(&calc);
                    last_key = key;
                    continue;
                }
            }

            // Normal calculator processing for all keys
            Calculator_ProcessKey(&calc, key);
            last_key = key;
        }

        // When no key is pressed, clear last_key so the next press is detected
        if (key == 0) {
            last_key = 0;
        }

        // Simple software debounce delay for the keypad
        Delay_ms(50);
    }
}
