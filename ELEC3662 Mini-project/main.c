/*
 * Calculator Application with Games & Easter Eggs
 * 
 * Hardware:
 *   LCD - PA2 (EN), PA3 (RS), PB4-7 (Data)
 *   Keypad - PB0-3 (Columns), PE0-3 (Rows)
 * 
 * Key Mappings:
 *   Normal:  A=+  B=-  C=.  D=Shift  *=Equals  #=Backspace
 *   Shifted: A=×  B=÷  C=E  D=Cancel #=Clear
 * 
 * Easter Eggs & Games:
 *   Enter special numbers and press equals:
 *   42 = Answer to Life
 *   1337 = Leet mode
 *   404 = Error 404
 *   314159 = Pi digits
 *   73 = Ham radio greeting
 *   8008135 = Classic calculator
 * 
 *   Games (enter number and press equals):
 *   5318008 = Snake game
 *   7734 = Math quiz
 *   5373 = Number guessing
 *   9009 = Pong
 *   1234 = Reaction test
 *   4321 = Memory game
 */

#include "lcd.h"
#include "keypad.h"
#include "system.h"
#include "calculator.h"
#include "splash.h"
#include "games.h"

int main(void) {
    // Initialize system clock and peripherals
    System_Init();
    
    // Initialize LCD
    LCD_Init();
    
    // Initialize Keypad
    Keypad_Init();
    
    // Show splash screen (choose your favorite!)
    // Options: SPLASH_SATELLITE, SPLASH_M0LSC, SPLASH_ROCKET, 
    //          SPLASH_LOADING_BAR, SPLASH_MATRIX, SPLASH_WAVE
    Splash_Show(SPLASH_SATELLITE);
    
    // Initialize Calculator
    Calculator calc;
    Calculator_Init(&calc);
    
    // Initialize Games
    GameState game;
    Games_Init(&game);
    
    // Display initial state
    Calculator_DisplayUpdate(&calc);
    
    char last_key = 0;
    
    while(1) {
        char key = ReadKey();
        
        // Process key on press (not release)
        if(key != 0 && key != last_key) {
            // Check for easter eggs when equals is pressed
            if(key == '*') {
                // Check for easter eggs first
                Games_CheckEasterEgg(calc.expression);
                
                // Check for game activation codes
                int game_type = Games_CheckActivation(calc.expression);
                if(game_type != 0) {
                    Games_Launch(&game, (GameType)game_type);
                    Calculator_Clear(&calc);
                    Calculator_DisplayUpdate(&calc);
                    last_key = key;
                    continue;
                }
            }
            
            Calculator_ProcessKey(&calc, key);
            last_key = key;
        }
        
        // Reset last_key when no key pressed
        if(key == 0) {
            last_key = 0;
        }
        
        Delay_ms(50);  // Debounce delay
    }
}