/*
 * Splash Screen Implementation
 * 
 * Multiple fun animated splash screens for calculator startup
 */

#include "splash.h"
#include "lcd.h"
#include "system.h"
#include <string.h>
#include <stdint.h>

// Custom character definitions
static const unsigned char char_satellite[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b01110,
    0b00100,
    0b10001,
    0b01010,
    0b00100
};

static const unsigned char char_rocket[8] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b01010,
    0b01010
};

static const unsigned char char_antenna[8] = {
    0b00100,
    0b00100,
    0b01110,
    0b10101,
    0b00100,
    0b00100,
    0b11111,
    0b00000
};

static const unsigned char char_radio_wave[8] = {
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    0b01000,
    0b00100,
    0b00010
};

static const unsigned char char_full_block[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

static const unsigned char char_heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

static const unsigned char char_smile[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

// Create custom characters in LCD CGRAM
void Splash_CreateCustomChars(void) {
    // Custom character 0: Satellite
    LCD_Cmd(0x40);  // Set CGRAM address 0
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_satellite[i]);
    }
    
    // Custom character 1: Rocket
    LCD_Cmd(0x48);  // Set CGRAM address 1
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_rocket[i]);
    }
    
    // Custom character 2: Antenna
    LCD_Cmd(0x50);  // Set CGRAM address 2
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_antenna[i]);
    }
    
    // Custom character 3: Radio wave
    LCD_Cmd(0x58);  // Set CGRAM address 3
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_radio_wave[i]);
    }
    
    // Custom character 4: Full block
    LCD_Cmd(0x60);  // Set CGRAM address 4
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_full_block[i]);
    }
    
    // Custom character 5: Heart
    LCD_Cmd(0x68);  // Set CGRAM address 5
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_heart[i]);
    }
    
    // Custom character 6: Smile
    LCD_Cmd(0x70);  // Set CGRAM address 6
    for(int i = 0; i < 8; i++) {
        LCD_Char(char_smile[i]);
    }
    
    // Return to DDRAM
    LCD_Cmd(LCD_HOME);
}

// Main splash screen dispatcher
void Splash_Show(SplashType type) {
    Splash_CreateCustomChars();
    
    switch(type) {
        case SPLASH_CLASSIC:
            Splash_Classic();
            break;
        case SPLASH_MATRIX:
            Splash_Matrix();
            break;
        case SPLASH_WAVE:
            Splash_Wave();
            break;
        case SPLASH_ROCKET:
            Splash_Rocket();
            break;
        case SPLASH_SATELLITE:
            Splash_Satellite();
            break;
        case SPLASH_LOADING_BAR:
            Splash_LoadingBar();
            break;
        case SPLASH_TYPEWRITER:
            Splash_Typewriter();
            break;
        case SPLASH_RADAR:
            Splash_Radar();
            break;
        case SPLASH_CUSTOM_M0LSC:
            Splash_M0LSC();
            break;
        default:
            Splash_Classic();
            break;
    }
}

// Classic splash - simple fade in
void Splash_Classic(void) {
    LCD_Clear();
    
    // Line 1
    LCD_String("   CalcSat v1");
    
    // Line 2
    LCD_Cmd(LCD_LINE2);
    LCD_String("  By William");
    
    Delay_ms(2000);
    
    // Blink effect
    for(int i = 0; i < 3; i++) {
        LCD_Cmd(0x08);  // Display off
        Delay_ms(200);
        LCD_Cmd(0x0C);  // Display on
        Delay_ms(200);
    }
}

// Matrix-style rain effect
void Splash_Matrix(void) {
    LCD_Clear();
    
    // Simulate falling characters
    for(int col = 0; col < 16; col++) {
        LCD_Cmd(0x80 + col);  // Line 1
        LCD_Char('1' + (col % 2));
        Delay_ms(50);
        
        LCD_Cmd(0xC0 + col);  // Line 2
        LCD_Char('0' + (col % 2));
        Delay_ms(50);
    }
    
    Delay_ms(500);
    LCD_Clear();
    
    // Show title
    LCD_String("  CALCULATOR");
    LCD_Cmd(LCD_LINE2);
    LCD_String("   ONLINE");
    Delay_ms(1500);
}

// Wave animation
void Splash_Wave(void) {
    LCD_Clear();
    
    // Animated wave across screen
    for(int wave = 0; wave < 2; wave++) {
        for(int i = 0; i < 16; i++) {
            LCD_Cmd(0x80 + i);
            LCD_Char(0x04);  // Custom block character
            Delay_ms(50);
            LCD_Cmd(0x80 + i);
            LCD_Char(' ');
        }
    }
    
    LCD_Clear();
    LCD_String("    CALC-SAT");
    LCD_Cmd(LCD_LINE2);
    LCD_String("   READY!");
    Delay_ms(1500);
}

// Rocket launch animation
void Splash_Rocket(void) {
    LCD_Clear();
    
    // Countdown
    for(int i = 3; i > 0; i--) {
        LCD_Clear();
        LCD_String("   Launching...");
        LCD_Cmd(LCD_LINE2);
        LCD_Char(' ');
        LCD_Char(' ');
        LCD_Char(' ');
        LCD_Char(' ');
        LCD_Char(' ');
        LCD_Char(' ');
        LCD_Char('0' + i);
        Delay_ms(500);
    }
    
    // Launch sequence
    LCD_Clear();
    
    // Rocket on line 2, moves up
    for(int row = 2; row >= 1; row--) {
        LCD_Clear();
        if(row == 2) {
            LCD_Cmd(LCD_LINE2 + 7);
            LCD_Char(0x01);  // Rocket custom char
            LCD_Char(0x01);
        } else {
            LCD_Cmd(0x80 + 7);
            LCD_Char(0x01);
            LCD_Char(0x01);
        }
        Delay_ms(300);
    }
    
    // Final message
    LCD_Clear();
    LCD_String(" CalcSat v1.0");
    LCD_Cmd(LCD_LINE2);
    LCD_String("   IGNITION!");
    Delay_ms(1500);
}

// Satellite communication theme
void Splash_Satellite(void) {
    LCD_Clear();
    
    // Ground station to satellite animation
    LCD_String("GS");
    LCD_Cmd(0x80 + 14);
    LCD_Char(0x00);  // Satellite custom char
    
    // Transmit signal
    for(int i = 2; i < 14; i++) {
        LCD_Cmd(0x80 + i);
        LCD_Char(0x03);  // Wave custom char
        Delay_ms(100);
        if(i > 3) {
            LCD_Cmd(0x80 + i - 2);
            LCD_Char(' ');
        }
    }
    
    Delay_ms(500);
    
    // Line 2: Data received
    LCD_Cmd(LCD_LINE2);
    LCD_String(" LINK   ");
    
    // Blinking cursor effect
    for(int i = 0; i < 3; i++) {
        LCD_String(" ");
        Delay_ms(200);
        LCD_Cmd(LCD_LINE2 + 6);
        LCD_String("*");
        Delay_ms(200);
        LCD_Cmd(LCD_LINE2 + 6);
    }
    
    LCD_Clear();
    LCD_String("  M0LSC / M0XWI");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  CalcSat v1");
    Delay_ms(2000);
}

// Loading bar animation
void Splash_LoadingBar(void) {
    LCD_Clear();
    LCD_String("  Initializing");
    LCD_Cmd(LCD_LINE2);
    LCD_String("[");
    
    // Progress bar
    for(int i = 0; i < 14; i++) {
        LCD_Cmd(LCD_LINE2 + 1 + i);
        LCD_Char(0x04);  // Full block custom char
        Delay_ms(150);
    }
    
    LCD_Cmd(LCD_LINE2 + 15);
    LCD_String("]");
    
    Delay_ms(500);
    
    LCD_Clear();
    LCD_String(" Calculator");
    LCD_Cmd(LCD_LINE2);
    LCD_String("   Ready!");
    Delay_ms(1000);
}

// Typewriter effect
void Splash_Typewriter(void) {
    LCD_Clear();
    
    const char* line1 = "  CALCULATOR";
    const char* line2 = "  v1.0 - 2025";
    
    // Type line 1
    for(int i = 0; line1[i] != '\0'; i++) {
        LCD_Cmd(0x80 + i);
        LCD_Char(line1[i]);
        Delay_ms(100);
    }
    
    Delay_ms(300);
    
    // Type line 2
    for(int i = 0; line2[i] != '\0'; i++) {
        LCD_Cmd(LCD_LINE2 + i);
        LCD_Char(line2[i]);
        Delay_ms(100);
    }
    
    Delay_ms(1500);
}

// Radar sweep animation
void Splash_Radar(void) {
    LCD_Clear();
    LCD_String("   SCANNING");
    LCD_Cmd(LCD_LINE2);
    
    // Sweep animation
    for(int sweep = 0; sweep < 2; sweep++) {
        for(int i = 0; i < 16; i++) {
            LCD_Cmd(LCD_LINE2 + i);
            LCD_Char('.');
            Delay_ms(80);
        }
        
        // Clear sweep
        LCD_Cmd(LCD_LINE2);
        LCD_String("                ");
    }
    
    // Target found
    LCD_Clear();
    LCD_String("  TARGET FOUND");
    LCD_Cmd(LCD_LINE2);
    LCD_String("  Calculator");
    
    Delay_ms(1500);
}

// Custom M0LSC themed splash
void Splash_M0LSC(void) {
    LCD_Clear();
    
    // Amateur radio themed
    LCD_String(" ");
    LCD_Char(0x02);  // Antenna custom char
    LCD_String(" M0LSC/M0XWI ");
    LCD_Char(0x02);
    
    LCD_Cmd(LCD_LINE2);
    LCD_String("  73 de Leeds");
    
    Delay_ms(2000);
    
    // Transition effect
    for(int i = 0; i < 16; i++) {
        LCD_Cmd(0x80 + i);
        LCD_Char(' ');
        Delay_ms(50);
    }
    
    LCD_Clear();
    LCD_String("  CalcSat v1.0");
    LCD_Cmd(LCD_LINE2);
    LCD_String(" RF/Sat/CCSDS");
    
    Delay_ms(1500);
}

// Helper: Scroll text horizontally
void Splash_ScrollText(const char* text, int delay_ms) {
    int len = strlen(text);
    
    for(int offset = 0; offset < len - 16; offset++) {
        LCD_Clear();
        for(int i = 0; i < 16 && (offset + i) < len; i++) {
            LCD_Char(text[offset + i]);
        }
        Delay_ms(delay_ms);
    }
}

// Helper: Fill screen animation
void Splash_FillAnimation(void) {
    LCD_Clear();
    
    // Fill line 1
    for(int i = 0; i < 16; i++) {
        LCD_Cmd(0x80 + i);
        LCD_Char(0x04);  // Full block
        Delay_ms(50);
    }
    
    // Fill line 2
    for(int i = 0; i < 16; i++) {
        LCD_Cmd(LCD_LINE2 + i);
        LCD_Char(0x04);  // Full block
        Delay_ms(50);
    }
    
    Delay_ms(300);
}

// Helper: Random dots animation
void Splash_RandomDots(int count, int delay_ms) {
    LCD_Clear();
    
    for(int i = 0; i < count; i++) {
        // Random position (simple pseudo-random)
        int pos = (i * 7) % 32;
        int row = pos / 16;
        int col = pos % 16;
        
        if(row == 0) {
            LCD_Cmd(0x80 + col);
        } else {
            LCD_Cmd(LCD_LINE2 + col);
        }
        
        LCD_Char('*');
        Delay_ms(delay_ms);
    }
}

// Helper: Draw box
void Splash_DrawBox(void) {
    LCD_Clear();
    
    // Top border
    LCD_Cmd(0x80);
    LCD_Char(0x04);
    for(int i = 1; i < 15; i++) {
        LCD_Char(0x04);
    }
    LCD_Char(0x04);
    
    // Bottom border
    LCD_Cmd(LCD_LINE2);
    LCD_Char(0x04);
    for(int i = 1; i < 15; i++) {
        LCD_Char(0x04);
    }
    LCD_Char(0x04);
    
    Delay_ms(1000);
}