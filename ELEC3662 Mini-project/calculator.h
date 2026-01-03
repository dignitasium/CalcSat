/*
 * Calculator Header
 * 
 * Features:
 * - Basic operations: +, -, *, /, ^ (power)
 * - Floating point calculations
 * - Multi-step calculations with PEMDAS operator precedence
 * - Memory functions (Store, Recall, Clear, Add, Subtract)
 * - Shift key for extended functions
 * 
 * Key Mappings (16-Key Keypad with Shift):
 * 
 * Normal Mode:
 * - 0-9: Digit entry
 * - A: Plus (+)
 * - B: Minus (-)
 * - C: Decimal point (.)
 * - D: Shift key (toggle shift mode)
 * - *: End input / Equals
 * - #: Rubout last character (backspace)
 * 
 * Shifted Mode (Press D first):
 * - A: Times (×)
 * - B: Divide (÷)
 * - C: Times ten to the power (×10^)
 * - D: Cancel shift
 * - #: Delete entire entry (clear)
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

// Calculator constants
#define MAX_EXPRESSION_LENGTH   32
#define MAX_OPERATORS          8
#define MAX_OPERANDS           9
#define MAX_INPUT_LENGTH       16

// Calculator states
typedef enum {
    STATE_ENTERING_NUMBER,
    STATE_ENTERING_OPERATOR,
    STATE_SHOW_RESULT,
    STATE_ERROR
} CalcState;

// Operator types
typedef enum {
    OP_NONE = 0,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_POWER10      // ×10^n (scientific notation)
} Operator;

// Calculator data structure
typedef struct {
    char expression[MAX_EXPRESSION_LENGTH];  // Display buffer
    char input_buffer[MAX_INPUT_LENGTH];     // Current input string
    int input_pos;                           // Position in input buffer
    float operands[MAX_OPERANDS];            // Numbers in expression
    Operator operators[MAX_OPERATORS];       // Operators in expression
    int operand_count;
    int operator_count;
    float current_number;
    int has_decimal;
    int decimal_places;
    CalcState state;
    float memory;                            // Memory storage
    int shift_active;                        // Shift key state
    char error_msg[20];                      // Error message
} Calculator;

// Function declarations

// Core calculator functions
void Calculator_Init(Calculator* calc);
void Calculator_ProcessKey(Calculator* calc, char key);
void Calculator_Clear(Calculator* calc);
void Calculator_ClearEntry(Calculator* calc);
float Calculator_Calculate(Calculator* calc);
void Calculator_DisplayUpdate(Calculator* calc);

// Input processing
void Calculator_EnterDigit(Calculator* calc, char digit);
void Calculator_EnterDecimal(Calculator* calc);
void Calculator_EnterOperator(Calculator* calc, Operator op);
void Calculator_Equals(Calculator* calc);
void Calculator_Backspace(Calculator* calc);

// Shift key handling
void Calculator_ToggleShift(Calculator* calc);

// Memory functions
void Calculator_MemoryStore(Calculator* calc);
void Calculator_MemoryRecall(Calculator* calc);
void Calculator_MemoryClear(Calculator* calc);
void Calculator_MemoryAdd(Calculator* calc);
void Calculator_MemorySubtract(Calculator* calc);

// Helper functions
int Calculator_GetOperatorPrecedence(Operator op);
char Calculator_OperatorToChar(Operator op);
void Calculator_SetError(Calculator* calc, const char* msg);
void Calculator_FormatNumber(char* buffer, float number, int buffer_size);
void Calculator_UpdateInputBuffer(Calculator* calc);

// Key to operation mapping
Operator Calculator_KeyToOperator(char key, int shifted);

#endif // CALCULATOR_H