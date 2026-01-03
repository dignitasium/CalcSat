/*
 * Games Module - Mini Games and Easter Eggs
 * 
 * Fun mini-games and easter eggs for CalcSat calculator
 * Accessed through secret key combinations
 * 
 * Games included:
 * - Snake
 * - Number Guessing Game
 * - Math Quiz
 * - Reaction Time Test
 * - Memory Game
 * - Pong
 * 
 * Easter Eggs:
 * - Enter special numbers for surprises
 * - Hidden messages
 * - Animations
 */

#ifndef GAMES_H
#define GAMES_H

#include "lcd.h"
#include "keypad.h"
#include "system.h"

// Game types
typedef enum {
    GAME_NONE,
    GAME_SNAKE,
    GAME_GUESS,
    GAME_QUIZ,
    GAME_REACTION,
    GAME_MEMORY,
    GAME_PONG
} GameType;

// Game state
typedef struct {
    GameType current_game;
    int score;
    int high_score;
    int level;
    int running;
} GameState;

// Main game functions
void Games_Init(GameState* game);
void Games_CheckEasterEgg(const char* input);
int Games_CheckActivation(const char* input);
void Games_Launch(GameState* game, GameType type);
void Games_Exit(GameState* game);

// Individual games
void Game_Snake(GameState* game);
void Game_NumberGuess(GameState* game);
void Game_MathQuiz(GameState* game);
void Game_ReactionTest(GameState* game);
void Game_Memory(GameState* game);
void Game_Pong(GameState* game);

// Easter eggs
void Easter_42(void);           // Answer to everything
void Easter_1337(void);         // Leet speak
void Easter_404(void);          // Not found
void Easter_Pi(void);           // Pi digits
void Easter_8008135(void);      // Classic calculator word
void Easter_M0LSC(void);        // Amateur radio
void Easter_73(void);           // Ham radio greeting
void Easter_Konami(void);       // Konami code
void Easter_Satellite(void);    // Space themed

// Helper functions
void Games_DisplayScore(int score);
void Games_SaveHighScore(GameType game, int score);
int Games_LoadHighScore(GameType game);
char Games_WaitForKey(void);
int Games_RandomNumber(int min, int max);

// Animation helpers
void Games_AnimateWin(void);
void Games_AnimateLose(void);
void Games_AnimateLevel(int level);

#endif // GAMES_H