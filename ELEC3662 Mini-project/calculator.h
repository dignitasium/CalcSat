#ifndef CALCULATOR_H
#define CALCULATOR_H

// -----------------------------
// Calculator configuration
// -----------------------------

// Maximum length of the full expression string shown on the LCD
#define MAX_EXPRESSION_LENGTH   32
// Maximum number of operators and operands stored for PEMDAS evaluation
#define MAX_OPERATORS           8
#define MAX_OPERANDS            9
// Maximum length of the current numeric input (digits + decimal point)
#define MAX_INPUT_LENGTH        16

// -----------------------------
// Calculator state machine
// -----------------------------
typedef enum {
    // User is typing a number (digits and optional decimal point)
    STATE_ENTERING_NUMBER,
    // User has just entered an operator (+, -, ×, ÷, E)
    STATE_ENTERING_OPERATOR,
    // A result has been calculated and is being shown
    STATE_SHOW_RESULT,
    // An error has occurred (e.g. divide by zero)
    STATE_ERROR
} CalcState;

// -----------------------------
// Supported operators
// -----------------------------
typedef enum {
    OP_NONE = 0,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    // Multiply by 10^n for scientific notation (value × 10^exponent)
    OP_POWER10
} Operator;

// -----------------------------
// Calculator context
// -----------------------------
// This struct holds all runtime state for the calculator.
// One instance is created in main.c and passed into all functions.
typedef struct {
    char expression[MAX_EXPRESSION_LENGTH];  // Full expression/result shown on LCD line 2
    char input_buffer[MAX_INPUT_LENGTH];     // Current number being typed as a string
    int  input_pos;                          // Current position in input_buffer

    float    operands[MAX_OPERANDS];         // Stored operands for expression evaluation
    Operator operators[MAX_OPERATORS];       // Stored operators between operands
    int      operand_count;                  // Number of valid entries in operands[]
    int      operator_count;                 // Number of valid entries in operators[]

    float current_number;                    // Current numeric value being edited
    int   has_decimal;                       // 1 if a decimal point has been entered
    int   decimal_places;                    // Number of decimal digits entered so far

    CalcState state;                         // Current calculator state

    // Memory register used by MS/MR/MC/M+/M- functions
    float memory;

    // When 1, the next key is interpreted as a shifted function (×, ÷, E, clear, memory)
    int shift_active;

    // Short error message shown when STATE_ERROR is set
    char error_msg[20];
} Calculator;

// -----------------------------
// Core calculator functions
// -----------------------------
void Calculator_Init(Calculator* calc);           // Reset all state to power-on defaults
void Calculator_ProcessKey(Calculator* calc, char key); // Handle a single key press
void Calculator_Clear(Calculator* calc);          // Clear entire calculator state
void Calculator_ClearEntry(Calculator* calc);     // Clear only the current number
float Calculator_Calculate(Calculator* calc);     // Evaluate expression with operator precedence
void Calculator_DisplayUpdate(Calculator* calc);  // Refresh LCD based on current state

// -----------------------------
// Input processing helpers
// -----------------------------
void Calculator_EnterDigit(Calculator* calc, char digit); // Handle 0–9
void Calculator_EnterDecimal(Calculator* calc);           // Handle decimal point
void Calculator_EnterOperator(Calculator* calc, Operator op); // Handle +, -, ×, ÷, E
void Calculator_Equals(Calculator* calc);                 // Handle equals key
void Calculator_Backspace(Calculator* calc);              // Delete last input digit

// -----------------------------
// Shift key handling
// -----------------------------
void Calculator_ToggleShift(Calculator* calc);

// -----------------------------
// Memory functions (MS/MR/MC/M+/M-)
// -----------------------------
void Calculator_MemoryStore(Calculator* calc);    // MS – store current value in memory
void Calculator_MemoryRecall(Calculator* calc);   // MR – recall memory to display
void Calculator_MemoryClear(Calculator* calc);    // MC – clear memory register
void Calculator_MemoryAdd(Calculator* calc);      // M+ – add current value into memory
void Calculator_MemorySubtract(Calculator* calc); // M- – subtract current value from memory

// -----------------------------
// Utility helpers
// -----------------------------
int  Calculator_GetOperatorPrecedence(Operator op);
char Calculator_OperatorToChar(Operator op);
void Calculator_SetError(Calculator* calc, const char* msg);
void Calculator_FormatNumber(char* buffer, float number, int buffer_size);
void Calculator_UpdateInputBuffer(Calculator* calc);

// Map a raw keypad key to an operator depending on shift state
Operator Calculator_KeyToOperator(char key, int shifted);

#endif // CALCULATOR_H