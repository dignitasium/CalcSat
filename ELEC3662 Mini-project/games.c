/*
 * Games Module Implementation
 * 
 * Fun mini-games and easter eggs for CalcSat calculator
 */

#include "games.h"
#include "lcd.h"
#include "keypad.h"
#include "system.h"
#include <string.h>
#include <stdlib.h>

// Simple random number generator state
static unsigned long rand_seed = 12345;

// Initialize games
void Games_Init(GameState* game) {
    game->current_game = GAME_NONE;
    game->score = 0;
    game->high_score = 0;
    game->level = 1;
    game->running = 0;
}

// Simple pseudo-random number generator
int Games_RandomNumber(int min, int max) {
    rand_seed = (rand_seed * 1103515245 + 12345) & 0x7fffffff;
    return min + (rand_seed % (max - min + 1));
}

// Check if input matches game activation code
int Games_CheckActivation(const char* input) {
    // Game activation codes:
    // 5318008 = "BOOBIES" (upside down) -> Snake
    // 7734 = "hELL" (upside down) -> Quiz
    // 5373 = "SESE" -> Guess
    // 9009 = "POOP" -> Pong
    
    if(strcmp(input, "5318008") == 0) return GAME_SNAKE;
    if(strcmp(input, "7734") == 0) return GAME_QUIZ;
    if(strcmp(input, "5373") == 0) return GAME_GUESS;
    if(strcmp(input, "9009") == 0) return GAME_PONG;
    if(strcmp(input, "1234") == 0) return GAME_REACTION;
    if(strcmp(input, "4321") == 0) return GAME_MEMORY;
    
    return 0;
}

// Check for easter eggs
void Games_CheckEasterEgg(const char* input) {
    if(strcmp(input, "42") == 0) {
        Easter_42();
    }
    else if(strcmp(input, "1337") == 0) {
        Easter_1337();
    }
    else if(strcmp(input, "404") == 0) {
        Easter_404();
    }
    else if(strcmp(input, "314159") == 0 || strcmp(input, "31415") == 0) {
        Easter_Pi();
    }
    else if(strcmp(input, "8008135") == 0) {
        Easter_8008135();
    }
    else if(strcmp(input, "73") == 0) {
        Easter_73();
    }
}

// Launch a game
void Games_Launch(GameState* game, GameType type) {
    game->current_game = type;
    game->score = 0;
    game->level = 1;
    game->running = 1;
    
    LCD_Clear();
    
    switch(type) {
        case GAME_SNAKE:
            Game_Snake(game);
            break;
        case GAME_GUESS:
            Game_NumberGuess(game);
            break;
        case GAME_QUIZ:
            Game_MathQuiz(game);
            break;
        case GAME_REACTION:
            Game_ReactionTest(game);
            break;
        case GAME_MEMORY:
            Game_Memory(game);
            break;
        case GAME_PONG:
            Game_Pong(game);
            break;
        default:
            break;
    }
    
    game->running = 0;
}

// Exit game
void Games_Exit(GameState* game) {
    game->running = 0;
    game->current_game = GAME_NONE;
}

// ============================================================================
// GAME 1: SNAKE
// ============================================================================
void Game_Snake(GameState* game) {
    LCD_Clear();
    LCD_String("  SNAKE GAME");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" Use A B C D");
    Delay_ms(2000);
    
    // Simple snake implementation
    int snake_x = 7;
    int snake_y = 0;
    int food_x = 12;
    int food_y = 1;
    int score = 0;
    
    while(game->running) {
        LCD_Clear();
        
        // Draw snake head
        if(snake_y == 0) {
            LCD_Cmd(0x80 + snake_x);
        } else {
            LCD_Cmd(LCD_LINE2 + snake_x);
        }
        LCD_Char('O');
        
        // Draw food
        if(food_y == 0) {
            LCD_Cmd(0x80 + food_x);
        } else {
            LCD_Cmd(LCD_LINE2 + food_x);
        }
        LCD_Char('*');
        
        // Display score
        LCD_Cmd(0x80);
        LCD_Char('0' + (score / 10));
        LCD_Char('0' + (score % 10));
        
        // Get input
        char key = ReadKey();
        Delay_ms(200);
        
        if(key == 'A') snake_x--;      // Left
        else if(key == 'B') snake_x++; // Right
        else if(key == 'C') snake_y = (snake_y == 0) ? 1 : 0; // Toggle row
        else if(key == '*') break;     // Exit
        
        // Boundary check
        if(snake_x < 2) snake_x = 2;
        if(snake_x > 15) snake_x = 15;
        
        // Check collision with food
        if(snake_x == food_x && snake_y == food_y) {
            score++;
            food_x = Games_RandomNumber(2, 15);
            food_y = Games_RandomNumber(0, 1);
            
            // Victory animation
            for(int i = 0; i < 3; i++) {
                LCD_Cmd(0x08); // Display off
                Delay_ms(100);
                LCD_Cmd(0x0C); // Display on
                Delay_ms(100);
            }
        }
    }
    
    // Game over
    LCD_Clear();
    LCD_String("  GAME OVER!");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  Score: ");
    LCD_Char('0' + (score / 10));
    LCD_Char('0' + (score % 10));
    Delay_ms(3000);
}

// ============================================================================
// GAME 2: NUMBER GUESSING GAME
// ============================================================================
void Game_NumberGuess(GameState* game) {
    LCD_Clear();
    LCD_String(" GUESS NUMBER");
    LCD_Cmd(LCD_LINE2);
    LCD_String("   1 to 100");
    Delay_ms(2000);
    
    int target = Games_RandomNumber(1, 100);
    int guess = 0;
    int attempts = 0;
    int digit_pos = 0;
    char input[4] = {0};
    
    while(game->running && attempts < 7) {
        LCD_Clear();
        LCD_String("Guess: ");
        LCD_String(input);
        LCD_Cmd(LCD_LINE2);
        LCD_String("Tries: ");
        LCD_Char('0' + attempts);
        LCD_String("/7");
        
        char key = ReadKey();
        
        if(key >= '0' && key <= '9') {
            if(digit_pos < 3) {
                input[digit_pos++] = key;
                input[digit_pos] = '\0';
            }
            Delay_ms(300);
        }
        else if(key == '#') { // Backspace
            if(digit_pos > 0) {
                input[--digit_pos] = '\0';
            }
            Delay_ms(300);
        }
        else if(key == '*') { // Submit
            if(digit_pos > 0) {
                // Convert string to number
                guess = 0;
                for(int i = 0; i < digit_pos; i++) {
                    guess = guess * 10 + (input[i] - '0');
                }
                
                attempts++;
                
                if(guess == target) {
                    // WIN!
                    LCD_Clear();
                    LCD_String(" YOU WIN!");
                    LCD_Cmd(LCD_LINE2);
                    LCD_String(" ");
                    LCD_Char('0' + attempts);
                    LCD_String(" tries");
                    Games_AnimateWin();
                    Delay_ms(3000);
                    break;
                }
                else if(guess < target) {
                    LCD_Clear();
                    LCD_String("  TOO LOW!");
                    Delay_ms(1000);
                }
                else {
                    LCD_Clear();
                    LCD_String("  TOO HIGH!");
                    Delay_ms(1000);
                }
                
                // Reset input
                digit_pos = 0;
                input[0] = '\0';
            }
            Delay_ms(300);
        }
        else if(key == 'D') { // Exit
            break;
        }
        
        Delay_ms(50);
    }
    
    if(attempts >= 7 && guess != target) {
        LCD_Clear();
        LCD_String("  GAME OVER!");
        LCD_Cmd(LCD_LINE2);
        LCD_String("  It was: ");
        LCD_Char('0' + (target / 100));
        LCD_Char('0' + ((target / 10) % 10));
        LCD_Char('0' + (target % 10));
        Delay_ms(3000);
    }
}

// ============================================================================
// GAME 3: MATH QUIZ
// ============================================================================
void Game_MathQuiz(GameState* game) {
    LCD_Clear();
    LCD_String("  MATH QUIZ");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  Get ready!");
    Delay_ms(2000);
    
    int score = 0;
    int questions = 5;
    
    for(int q = 0; q < questions; q++) {
        int a = Games_RandomNumber(2, 20);
        int b = Games_RandomNumber(2, 20);
        int op = Games_RandomNumber(0, 1); // 0=add, 1=multiply
        int answer = (op == 0) ? (a + b) : (a * b);
        
        LCD_Clear();
        LCD_String("Q");
        LCD_Char('0' + (q + 1));
        LCD_String(": ");
        
        // Display question
        if(a >= 10) {
            LCD_Char('0' + (a / 10));
            LCD_Char('0' + (a % 10));
        } else {
            LCD_Char('0' + a);
        }
        
        LCD_Char(op == 0 ? '+' : '*');
        
        if(b >= 10) {
            LCD_Char('0' + (b / 10));
            LCD_Char('0' + (b % 10));
        } else {
            LCD_Char('0' + b);
        }
        
        LCD_String("=?");
        
        // Get answer
        char input[4] = {0};
        int digit_pos = 0;
        int submitted = 0;
        
        LCD_Cmd(LCD_LINE2);
        LCD_String("Ans: ");
        
        while(!submitted) {
            char key = ReadKey();
            
            if(key >= '0' && key <= '9') {
                if(digit_pos < 3) {
                    input[digit_pos++] = key;
                    input[digit_pos] = '\0';
                    LCD_Char(key);
                }
                Delay_ms(300);
            }
            else if(key == '#' && digit_pos > 0) {
                digit_pos--;
                input[digit_pos] = '\0';
                LCD_Cmd(LCD_LINE2);
                LCD_String("Ans: ");
                LCD_String(input);
                Delay_ms(300);
            }
            else if(key == '*' && digit_pos > 0) {
                submitted = 1;
            }
            
            Delay_ms(50);
        }
        
        // Check answer
        int user_answer = 0;
        for(int i = 0; i < digit_pos; i++) {
            user_answer = user_answer * 10 + (input[i] - '0');
        }
        
        if(user_answer == answer) {
            score++;
            LCD_Clear();
            LCD_String("  CORRECT!");
            LCD_Cmd(LCD_LINE2);
            LCD_String("  Score: ");
            LCD_Char('0' + score);
            Delay_ms(1500);
        } else {
            LCD_Clear();
            LCD_String("  WRONG!");
            LCD_Cmd(LCD_LINE2);
            LCD_String("  Ans: ");
            LCD_Char('0' + (answer / 10));
            LCD_Char('0' + (answer % 10));
            Delay_ms(1500);
        }
    }
    
    // Final score
    LCD_Clear();
    LCD_String("  QUIZ DONE!");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  Score: ");
    LCD_Char('0' + score);
    LCD_Char('/');
    LCD_Char('0' + questions);
    
    if(score == questions) {
        Games_AnimateWin();
    }
    
    Delay_ms(3000);
}

// ============================================================================
// GAME 4: REACTION TIME TEST
// ============================================================================
void Game_ReactionTest(GameState* game) {
    LCD_Clear();
    LCD_String(" REACTION TEST");
    LCD_Cmd(LCD_LINE2);
    LCD_String("Press * when GO");
    Delay_ms(2000);
    
    int total_time = 0;
    int rounds = 3;
    
    for(int r = 0; r < rounds; r++) {
        LCD_Clear();
        LCD_String("  ROUND ");
        LCD_Char('0' + (r + 1));
        LCD_Cmd(LCD_LINE2);
        LCD_String("  Get ready...");
        
        // Random delay
        int wait = Games_RandomNumber(2000, 5000);
        Delay_ms(wait);
        
        // GO!
        LCD_Clear();
        LCD_String("     GO!");
        LCD_Cmd(LCD_LINE2);
        LCD_String("   PRESS *");
        
        unsigned long start_time = millis();
        
        // Wait for key press
        while(1) {
            char key = ReadKey();
            if(key == '*') break;
            Delay_ms(10);
        }
        
        unsigned long reaction_time = millis() - start_time;
        total_time += reaction_time;
        
        LCD_Clear();
        LCD_String("  Time: ");
        LCD_Char('0' + (reaction_time / 100));
        LCD_Char('0' + ((reaction_time / 10) % 10));
        LCD_Char('0' + (reaction_time % 10));
        LCD_String("ms");
        Delay_ms(2000);
    }
    
    // Average
    int avg = total_time / rounds;
    LCD_Clear();
    LCD_String(" AVG TIME:");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  ");
    LCD_Char('0' + (avg / 100));
    LCD_Char('0' + ((avg / 10) % 10));
    LCD_Char('0' + (avg % 10));
    LCD_String(" ms");
    
    if(avg < 300) {
        Games_AnimateWin();
    }
    
    Delay_ms(3000);
}

// ============================================================================
// GAME 5: MEMORY GAME (Simon Says style)
// ============================================================================
void Game_Memory(GameState* game) {
    LCD_Clear();
    LCD_String("  MEMORY GAME");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  Simon Says");
    Delay_ms(2000);
    
    char sequence[20] = {0};
    int length = 1;
    int level = 1;
    
    while(level <= 10) {
        // Add new random key to sequence
        int r = Games_RandomNumber(0, 3);
        char keys[] = {'A', 'B', 'C', 'D'};
        sequence[length - 1] = keys[r];
        
        // Show sequence
        LCD_Clear();
        LCD_String(" Watch! Lvl:");
        LCD_Char('0' + (level / 10));
        LCD_Char('0' + (level % 10));
        Delay_ms(1000);
        
        for(int i = 0; i < length; i++) {
            LCD_Cmd(LCD_LINE2);
            LCD_String("      ");
            LCD_Char(sequence[i]);
            Delay_ms(600);
            LCD_Cmd(LCD_LINE2);
            LCD_String("       ");
            Delay_ms(400);
        }
        
        // User input
        LCD_Clear();
        LCD_String(" Your turn!");
        LCD_Cmd(LCD_LINE2);
        LCD_String(" ");
        
        int correct = 1;
        for(int i = 0; i < length; i++) {
            char key = Games_WaitForKey();
            LCD_Char(key);
            
            if(key != sequence[i]) {
                correct = 0;
                break;
            }
        }
        
        if(correct) {
            LCD_Clear();
            LCD_String("  CORRECT!");
            Delay_ms(1000);
            level++;
            length++;
        } else {
            LCD_Clear();
            LCD_String("  WRONG!");
            LCD_Cmd(LCD_LINE2);
            LCD_String("  Level: ");
            LCD_Char('0' + (level / 10));
            LCD_Char('0' + (level % 10));
            Delay_ms(3000);
            break;
        }
    }
    
    if(level > 10) {
        LCD_Clear();
        LCD_String("  YOU WIN!");
        LCD_Cmd(LCD_LINE2);
        LCD_String(" MEMORY MASTER");
        Games_AnimateWin();
        Delay_ms(3000);
    }
}

// ============================================================================
// GAME 6: PONG (Simple 1-player version)
// ============================================================================
void Game_Pong(GameState* game) {
    LCD_Clear();
    LCD_String("    PONG");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" Press A to hit");
    Delay_ms(2000);
    
    int score = 0;
    int ball_pos = 0;
    int ball_dir = 1; // 1 = right, -1 = left
    int paddle_pos = 7;
    
    while(score < 10) {
        LCD_Clear();
        
        // Draw score
        LCD_Cmd(0x80);
        LCD_Char('0' + score);
        
        // Draw paddle (left side)
        LCD_Cmd(LCD_LINE2 + paddle_pos);
        LCD_Char('|');
        
        // Draw ball
        if(ball_pos < 16) {
            LCD_Cmd(0x80 + ball_pos);
            LCD_Char('O');
        }
        
        // Move ball
        ball_pos += ball_dir;
        
        // Check collision with right wall
        if(ball_pos >= 15) {
            ball_dir = -1;
        }
        
        // Check collision with paddle
        if(ball_pos == 0) {
            char key = ReadKey();
            if(key == 'A' && paddle_pos == 7) { // Simplified timing
                score++;
                ball_dir = 1;
                ball_pos = 1;
            } else {
                // Miss!
                LCD_Clear();
                LCD_String("  MISSED!");
                LCD_Cmd(LCD_LINE2);
                LCD_String("  Score: ");
                LCD_Char('0' + score);
                Delay_ms(2000);
                break;
            }
        }
        
        Delay_ms(200);
    }
    
    if(score >= 10) {
        LCD_Clear();
        LCD_String("  YOU WIN!");
        LCD_Cmd(LCD_LINE2);
        LCD_String("  Perfect 10!");
        Games_AnimateWin();
        Delay_ms(3000);
    }
}

// ============================================================================
// EASTER EGGS
// ============================================================================

void Easter_42(void) {
    LCD_Clear();
    LCD_String("The Answer to");
    LCD_Cmd(LCD_LINE2);
    LCD_String("Life, Universe");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("and Everything");
    LCD_Cmd(LCD_LINE2);
    LCD_String("is 42");
    Delay_ms(3000);
}

void Easter_1337(void) {
    LCD_Clear();
    LCD_String("  1337 M0D3");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  4C7IV473D!");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("  H4CK 7H3");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  PL4N37!");
    Delay_ms(2000);
}

void Easter_404(void) {
    LCD_Clear();
    LCD_String("  ERROR 404");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" Result not found");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("Have you tried");
    LCD_Cmd(LCD_LINE2);
    LCD_String("turning it off?");
    Delay_ms(2000);
}

void Easter_Pi(void) {
    LCD_Clear();
    LCD_String("Pi = 3.14159265");
    LCD_Cmd(LCD_LINE2);
    LCD_String("35897932384626");
    
    // Scroll more digits
    Delay_ms(2000);
    for(int i = 0; i < 3; i++) {
        LCD_Cmd(0x18); // Scroll left
        Delay_ms(300);
    }
    Delay_ms(2000);
}

void Easter_8008135(void) {
    LCD_Clear();
    LCD_String("  8008135");
    LCD_Cmd(LCD_LINE2);
    LCD_String("Classic! :)");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("Turn calculator");
    LCD_Cmd(LCD_LINE2);
    LCD_String("upside down!");
    Delay_ms(2000);
}

void Easter_73(void) {
    LCD_Clear();
    LCD_String("  73 de M0XWI");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  73 de M0LSC");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("Best regards!");
    LCD_Cmd(LCD_LINE2);
    LCD_String("Keep calm & QSO");
    Delay_ms(2000);
}

void Easter_M0LSC(void) {
    LCD_Clear();
    LCD_String(" Leeds Space");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" Comms Group!");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String("  First in 40");
    LCD_Cmd(LCD_LINE2);
    LCD_String("    years!");
    Delay_ms(2000);
}

void Easter_Satellite(void) {
    LCD_Clear();
    LCD_String("  QO-100 LINK");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  ESTABLISHED");
    Delay_ms(2000);
    LCD_Clear();
    LCD_String(" 73 from GEO!");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" 36,000 km up");
    Delay_ms(2000);
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

char Games_WaitForKey(void) {
    char key = 0;
    while(key == 0) {
        key = ReadKey();
        Delay_ms(50);
    }
    // Wait for release
    while(ReadKey() != 0) {
        Delay_ms(50);
    }
    return key;
}

void Games_AnimateWin(void) {
    for(int i = 0; i < 5; i++) {
        LCD_Cmd(0x08); // Display off
        Delay_ms(100);
        LCD_Cmd(0x0C); // Display on
        Delay_ms(100);
    }
}

void Games_AnimateLose(void) {
    for(int i = 0; i < 3; i++) {
        LCD_Cmd(0x08);
        Delay_ms(200);
        LCD_Cmd(0x0C);
        Delay_ms(200);
    }
}

void Games_AnimateLevel(int level) {
    LCD_Clear();
    LCD_String("    LEVEL ");
    LCD_Char('0' + level);
    Delay_ms(1000);
}

void Games_DisplayScore(int score) {
    LCD_Clear();
    LCD_String("  Score: ");
    LCD_Char('0' + (score / 10));
    LCD_Char('0' + (score % 10));
    Delay_ms(1500);
}