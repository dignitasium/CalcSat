/*
 * Splash Screen Module
 * 
 * Fun animated splash screens for calculator startup
 * Includes multiple themes and animation styles
 */

#ifndef SPLASH_H
#define SPLASH_H

#include "lcd.h"
#include "system.h"

// Splash screen types
typedef enum {
    SPLASH_CLASSIC,
    SPLASH_MATRIX,
    SPLASH_WAVE,
    SPLASH_ROCKET,
    SPLASH_SATELLITE,
    SPLASH_LOADING_BAR,
    SPLASH_TYPEWRITER,
    SPLASH_RADAR,
    SPLASH_CUSTOM_M0LSC
} SplashType;

// Function declarations
void Splash_Show(SplashType type);
void Splash_Classic(void);
void Splash_Matrix(void);
void Splash_Wave(void);
void Splash_Rocket(void);
void Splash_Satellite(void);
void Splash_LoadingBar(void);
void Splash_Typewriter(void);
void Splash_Radar(void);
void Splash_M0LSC(void);

// Animation helpers
void Splash_ScrollText(const char* text, int delay_ms);
void Splash_FillAnimation(void);
void Splash_RandomDots(int count, int delay_ms);
void Splash_DrawBox(void);

// Custom character definitions for splash screens
void Splash_CreateCustomChars(void);

#endif // SPLASH_H