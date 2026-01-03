/*
 * Calculator Implementation with Shift Key Support
 * 
 * Implements a floating-point calculator with PEMDAS operator precedence,
 * memory functions, and shift key for extended functionality.
 * 
 * Key Mappings:
 * Normal:  A=+  B=-  C=.  D=Shift  *=Equals  #=Backspace
 * Shifted: A=×  B=÷  C=E  D=Cancel #=Clear
 */

#include "calculator.h"
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Initialize calculator
void Calculator_Init(Calculator* calc) {
    calc->expression[0] = '0';
    calc->expression[1] = '\0';
    calc->input_buffer[0] = '\0';
    calc->input_pos = 0;
    calc->operand_count = 0;
    calc->operator_count = 0;
    calc->current_number = 0.0f;
    calc->has_decimal = 0;
    calc->decimal_places = 0;
    calc->state = STATE_ENTERING_NUMBER;
    calc->memory = 0.0f;
    calc->shift_active = 0;
    calc->error_msg[0] = '\0';
}

// Toggle shift key
void Calculator_ToggleShift(Calculator* calc) {
    calc->shift_active = !calc->shift_active;
    Calculator_DisplayUpdate(calc);
}

// Process keypad input with shift support
void Calculator_ProcessKey(Calculator* calc, char key) {
    // Clear error state on any key (except shift)
    if(calc->state == STATE_ERROR && key != 'D') {
        Calculator_Clear(calc);
    }
    
    // Handle shift key
    if(key == 'D') {
        Calculator_ToggleShift(calc);
        return;
    }
    
    // Process keys based on shift state
    if(calc->shift_active) {
        // Shifted mode
        if(key == 'A') {
            // Times (×)
            Calculator_EnterOperator(calc, OP_MULTIPLY);
            calc->shift_active = 0;
        }
        else if(key == 'B') {
            // Divide (÷)
            Calculator_EnterOperator(calc, OP_DIVIDE);
            calc->shift_active = 0;
        }
        else if(key == 'C') {
            // Scientific notation (E)
            Calculator_EnterOperator(calc, OP_POWER10);
            calc->shift_active = 0;
        }
        else if(key == '#') {
            // Delete entire entry (clear)
            Calculator_ClearEntry(calc);
            calc->shift_active = 0;
        }
        else {
            // Other keys cancel shift
            calc->shift_active = 0;
            // Process key normally
            Calculator_ProcessKey(calc, key);
            return;
        }
    }
    else {
        // Normal mode
        if(key >= '0' && key <= '9') {
            Calculator_EnterDigit(calc, key);
        }
        else if(key == 'A') {
            // Plus (+)
            Calculator_EnterOperator(calc, OP_ADD);
        }
        else if(key == 'B') {
            // Minus (-)
            Calculator_EnterOperator(calc, OP_SUBTRACT);
        }
        else if(key == 'C') {
            // Decimal point (.)
            Calculator_EnterDecimal(calc);
        }
        else if(key == '*') {
            // Equals
            Calculator_Equals(calc);
        }
        else if(key == '#') {
            // Backspace
            Calculator_Backspace(calc);
        }
    }
    
    // Update display
    Calculator_DisplayUpdate(calc);
}

// Enter a digit
void Calculator_EnterDigit(Calculator* calc, char digit) {
    if(calc->state == STATE_SHOW_RESULT) {
        // Start new calculation after result
        Calculator_Clear(calc);
    }
    
    if(calc->state != STATE_ENTERING_NUMBER) {
        // Starting a new number after operator
        calc->current_number = 0.0f;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        calc->input_buffer[0] = '\0';
        calc->input_pos = 0;
        calc->state = STATE_ENTERING_NUMBER;
    }
    
    // Add to input buffer
    if(calc->input_pos < MAX_INPUT_LENGTH - 1) {
        calc->input_buffer[calc->input_pos++] = digit;
        calc->input_buffer[calc->input_pos] = '\0';
    }
    
    int digit_value = digit - '0';
    
    if(calc->has_decimal) {
        // Add decimal digit
        float divisor = 1.0f;
        for(int i = 0; i <= calc->decimal_places; i++) {
            divisor *= 10.0f;
        }
        calc->current_number += digit_value / divisor;
        calc->decimal_places++;
    } else {
        // Add integer digit
        calc->current_number = calc->current_number * 10.0f + digit_value;
    }
    
    // Update expression display
    Calculator_UpdateInputBuffer(calc);
}

// Enter decimal point
void Calculator_EnterDecimal(Calculator* calc) {
    if(calc->state == STATE_SHOW_RESULT) {
        Calculator_Clear(calc);
    }
    
    if(calc->state != STATE_ENTERING_NUMBER) {
        calc->current_number = 0.0f;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        calc->input_buffer[0] = '0';
        calc->input_buffer[1] = '\0';
        calc->input_pos = 1;
        calc->state = STATE_ENTERING_NUMBER;
    }
    
    if(!calc->has_decimal) {
        calc->has_decimal = 1;
        calc->decimal_places = 0;
        
        // Add to input buffer
        if(calc->input_pos < MAX_INPUT_LENGTH - 1) {
            calc->input_buffer[calc->input_pos++] = '.';
            calc->input_buffer[calc->input_pos] = '\0';
        }
        
        Calculator_UpdateInputBuffer(calc);
    }
}

// Backspace - remove last character
void Calculator_Backspace(Calculator* calc) {
    if(calc->state == STATE_SHOW_RESULT || calc->state == STATE_ERROR) {
        return;
    }
    
    if(calc->state == STATE_ENTERING_NUMBER && calc->input_pos > 0) {
        // Remove last character from input buffer
        calc->input_pos--;
        calc->input_buffer[calc->input_pos] = '\0';
        
        // Recalculate current number from input buffer
        calc->current_number = 0.0f;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        
        int found_decimal = 0;
        for(int i = 0; i < calc->input_pos; i++) {
            if(calc->input_buffer[i] == '.') {
                found_decimal = 1;
                calc->has_decimal = 1;
            } else {
                int digit = calc->input_buffer[i] - '0';
                if(!found_decimal) {
                    calc->current_number = calc->current_number * 10.0f + digit;
                } else {
                    float divisor = 1.0f;
                    for(int j = 0; j <= calc->decimal_places; j++) {
                        divisor *= 10.0f;
                    }
                    calc->current_number += digit / divisor;
                    calc->decimal_places++;
                }
            }
        }
        
        // If buffer is empty, reset to 0
        if(calc->input_pos == 0) {
            calc->input_buffer[0] = '0';
            calc->input_buffer[1] = '\0';
            calc->input_pos = 1;
            calc->current_number = 0.0f;
        }
        
        Calculator_UpdateInputBuffer(calc);
    }
}

// Clear entry (keep expression)
void Calculator_ClearEntry(Calculator* calc) {
    if(calc->state == STATE_ENTERING_NUMBER) {
        calc->current_number = 0.0f;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        calc->input_buffer[0] = '0';
        calc->input_buffer[1] = '\0';
        calc->input_pos = 1;
        Calculator_UpdateInputBuffer(calc);
    } else {
        // If not entering number, clear everything
        Calculator_Clear(calc);
    }
}

// Update input buffer in expression
void Calculator_UpdateInputBuffer(Calculator* calc) {
    // Rebuild expression with current input
    int len = strlen(calc->expression);
    
    // Find last operator position
    int last_op = -1;
    for(int i = len - 1; i >= 0; i--) {
        if(calc->expression[i] == '+' || calc->expression[i] == '-' ||
           calc->expression[i] == '*' || calc->expression[i] == '/' ||
           calc->expression[i] == 'E') {
            last_op = i;
            break;
        }
    }
    
    // Rebuild from operator or start
    if(last_op >= 0) {
        calc->expression[last_op + 1] = '\0';
        strncat(calc->expression, calc->input_buffer, 
                MAX_EXPRESSION_LENGTH - strlen(calc->expression) - 1);
    } else {
        strncpy(calc->expression, calc->input_buffer, MAX_EXPRESSION_LENGTH - 1);
        calc->expression[MAX_EXPRESSION_LENGTH - 1] = '\0';
    }
}

// Enter an operator
void Calculator_EnterOperator(Calculator* calc, Operator op) {
    if(calc->state == STATE_ERROR) {
        return;
    }
    
    if(calc->state == STATE_ENTERING_NUMBER) {
        // Save current number as operand
        if(calc->operand_count < MAX_OPERANDS) {
            calc->operands[calc->operand_count++] = calc->current_number;
        }
        calc->current_number = 0.0f;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        calc->input_buffer[0] = '\0';
        calc->input_pos = 0;
    }
    
    // Add operator (or replace last operator if consecutive)
    if(calc->operator_count < MAX_OPERATORS) {
        if(calc->state == STATE_ENTERING_OPERATOR && calc->operator_count > 0) {
            // Replace last operator
            calc->operators[calc->operator_count - 1] = op;
            // Update display - remove last char and add new operator
            int len = strlen(calc->expression);
            if(len > 0) {
                calc->expression[len - 1] = Calculator_OperatorToChar(op);
            }
        } else {
            calc->operators[calc->operator_count++] = op;
            // Add operator to display
            int len = strlen(calc->expression);
            if(len < MAX_EXPRESSION_LENGTH - 1) {
                calc->expression[len] = Calculator_OperatorToChar(op);
                calc->expression[len + 1] = '\0';
            }
        }
    }
    
    calc->state = STATE_ENTERING_OPERATOR;
}

// Calculate result with PEMDAS precedence
float Calculator_Calculate(Calculator* calc) {
    // Add last number if we're entering one
    if(calc->state == STATE_ENTERING_NUMBER) {
        if(calc->operand_count < MAX_OPERANDS) {
            calc->operands[calc->operand_count++] = calc->current_number;
        }
    }
    
    // Need at least 2 operands to calculate
    if(calc->operand_count < 2) {
        return calc->operand_count == 1 ? calc->operands[0] : 0.0f;
    }
    
    // Create working copies for calculation
    float values[MAX_OPERANDS];
    Operator ops[MAX_OPERATORS];
    int value_count = calc->operand_count;
    int op_count = calc->operator_count;
    
    for(int i = 0; i < value_count; i++) {
        values[i] = calc->operands[i];
    }
    for(int i = 0; i < op_count; i++) {
        ops[i] = calc->operators[i];
    }
    
    // First pass: Handle *, /, and E (higher precedence)
    int i = 0;
    while(i < op_count) {
        if(ops[i] == OP_MULTIPLY || ops[i] == OP_DIVIDE || ops[i] == OP_POWER10) {
            float result;
            
            if(ops[i] == OP_MULTIPLY) {
                result = values[i] * values[i + 1];
            } else if(ops[i] == OP_DIVIDE) {
                if(values[i + 1] == 0.0f) {
                    Calculator_SetError(calc, "Div by 0");
                    return 0.0f;
                }
                result = values[i] / values[i + 1];
            } else { // OP_POWER10
                // Calculate values[i] × 10^values[i+1]
                float power = 1.0f;
                int exponent = (int)values[i + 1];
                if(exponent >= 0) {
                    for(int j = 0; j < exponent; j++) {
                        power *= 10.0f;
                    }
                } else {
                    for(int j = 0; j < -exponent; j++) {
                        power /= 10.0f;
                    }
                }
                result = values[i] * power;
            }
            
            // Replace two operands with result
            values[i] = result;
            
            // Shift remaining values left
            for(int j = i + 1; j < value_count - 1; j++) {
                values[j] = values[j + 1];
            }
            value_count--;
            
            // Shift remaining operators left
            for(int j = i; j < op_count - 1; j++) {
                ops[j] = ops[j + 1];
            }
            op_count--;
            
            // Don't increment i, check same position again
        } else {
            i++;
        }
    }
    
    // Second pass: Handle + and - (lower precedence)
    i = 0;
    while(i < op_count) {
        float result;
        
        if(ops[i] == OP_ADD) {
            result = values[i] + values[i + 1];
        } else { // OP_SUBTRACT
            result = values[i] - values[i + 1];
        }
        
        // Replace two operands with result
        values[i] = result;
        
        // Shift remaining values left
        for(int j = i + 1; j < value_count - 1; j++) {
            values[j] = values[j + 1];
        }
        value_count--;
        
        // Shift remaining operators left
        for(int j = i; j < op_count - 1; j++) {
            ops[j] = ops[j + 1];
        }
        op_count--;
        
        // Don't increment i, check same position again
    }
    
    return values[0];
}

// Calculate and display result
void Calculator_Equals(Calculator* calc) {
    if(calc->state == STATE_ERROR) {
        return;
    }
    
    float result = Calculator_Calculate(calc);
    
    if(calc->state != STATE_ERROR) {
        // Clear for new calculation but keep result
        calc->current_number = result;
        calc->operand_count = 0;
        calc->operator_count = 0;
        calc->has_decimal = 0;
        calc->decimal_places = 0;
        calc->input_buffer[0] = '\0';
        calc->input_pos = 0;
        
        // Format result for display
        Calculator_FormatNumber(calc->expression, result, MAX_EXPRESSION_LENGTH);
        
        calc->state = STATE_SHOW_RESULT;
    }
}

// Clear calculator
void Calculator_Clear(Calculator* calc) {
    Calculator_Init(calc);
}

// Update LCD display
void Calculator_DisplayUpdate(Calculator* calc) {
    LCD_Cmd(LCD_CLEAR);
    
    if(calc->state == STATE_ERROR) {
        LCD_String("Error:");
        LCD_Cmd(LCD_LINE2);
        LCD_String(calc->error_msg);
    } else {
        // Line 1: Shift indicator or Memory indicator
        if(calc->shift_active) {
            LCD_String("SHIFT");
        } else if(calc->memory != 0.0f) {
            LCD_String("M:");
            char mem_buf[12];
            Calculator_FormatNumber(mem_buf, calc->memory, 12);
            LCD_String(mem_buf);
        }
        
        // Line 2: Expression
        LCD_Cmd(LCD_LINE2);
        LCD_String(calc->expression);
    }
}

// Memory Store (MS) - Store current result in memory
void Calculator_MemoryStore(Calculator* calc) {
    if(calc->state == STATE_SHOW_RESULT) {
        calc->memory = calc->current_number;
    } else if(calc->state == STATE_ENTERING_NUMBER) {
        calc->memory = calc->current_number;
    }
}

// Memory Recall (MR) - Recall memory value
void Calculator_MemoryRecall(Calculator* calc) {
    Calculator_Clear(calc);
    calc->current_number = calc->memory;
    Calculator_FormatNumber(calc->expression, calc->memory, MAX_EXPRESSION_LENGTH);
    calc->state = STATE_SHOW_RESULT;
}

// Memory Clear (MC) - Clear memory
void Calculator_MemoryClear(Calculator* calc) {
    calc->memory = 0.0f;
}

// Memory Add (M+) - Add current value to memory
void Calculator_MemoryAdd(Calculator* calc) {
    if(calc->state == STATE_SHOW_RESULT) {
        calc->memory += calc->current_number;
    } else if(calc->state == STATE_ENTERING_NUMBER) {
        calc->memory += calc->current_number;
    }
}

// Memory Subtract (M-) - Subtract current value from memory
void Calculator_MemorySubtract(Calculator* calc) {
    if(calc->state == STATE_SHOW_RESULT) {
        calc->memory -= calc->current_number;
    } else if(calc->state == STATE_ENTERING_NUMBER) {
        calc->memory -= calc->current_number;
    }
}

// Helper: Get operator precedence
int Calculator_GetOperatorPrecedence(Operator op) {
    switch(op) {
        case OP_MULTIPLY:
        case OP_DIVIDE:
        case OP_POWER10:
            return 2;
        case OP_ADD:
        case OP_SUBTRACT:
            return 1;
        default:
            return 0;
    }
}

// Helper: Convert operator to character
char Calculator_OperatorToChar(Operator op) {
    switch(op) {
        case OP_ADD:      return '+';
        case OP_SUBTRACT: return '-';
        case OP_MULTIPLY: return '*';
        case OP_DIVIDE:   return '/';
        case OP_POWER10:  return 'E';
        default:          return ' ';
    }
}

// Helper: Map key to operator (with shift support)
Operator Calculator_KeyToOperator(char key, int shifted) {
    if(shifted) {
        switch(key) {
            case 'A': return OP_MULTIPLY;
            case 'B': return OP_DIVIDE;
            case 'C': return OP_POWER10;
            default:  return OP_NONE;
        }
    } else {
        switch(key) {
            case 'A': return OP_ADD;
            case 'B': return OP_SUBTRACT;
            default:  return OP_NONE;
        }
    }
}

// Helper: Set error state
void Calculator_SetError(Calculator* calc, const char* msg) {
    calc->state = STATE_ERROR;
    int i;
    for(i = 0; msg[i] != '\0' && i < 19; i++) {
        calc->error_msg[i] = msg[i];
    }
    calc->error_msg[i] = '\0';
}

// Helper: Format number for display
void Calculator_FormatNumber(char* buffer, float number, int buffer_size) {
    // Handle special cases
    if(number == 0.0f) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    
    // Convert float to string with appropriate precision
    int int_part = (int)number;
    float frac_part = number - int_part;
    
    if(frac_part < 0) frac_part = -frac_part;
    
    // Handle negative numbers
    int pos = 0;
    if(number < 0 && int_part == 0) {
        buffer[pos++] = '-';
        int_part = -int_part;
    }
    
    // Convert integer part
    if(int_part == 0) {
        buffer[pos++] = '0';
    } else {
        char temp[16];
        int temp_pos = 0;
        int num = int_part;
        if(num < 0) num = -num;
        
        while(num > 0 && temp_pos < 15) {
            temp[temp_pos++] = '0' + (num % 10);
            num /= 10;
        }
        
        if(int_part < 0 && pos == 0) {
            buffer[pos++] = '-';
        }
        
        // Reverse temp into buffer
        for(int i = temp_pos - 1; i >= 0 && pos < buffer_size - 1; i--) {
            buffer[pos++] = temp[i];
        }
    }
    
    // Add decimal part if exists and space allows
    if(frac_part > 0.0001f && pos < buffer_size - 3) {
        buffer[pos++] = '.';
        
        // Add up to 4 decimal places
        int decimal_digits = 0;
        while(frac_part > 0.0001f && decimal_digits < 4 && pos < buffer_size - 1) {
            frac_part *= 10;
            int digit = (int)frac_part;
            buffer[pos++] = '0' + digit;
            frac_part -= digit;
            decimal_digits++;
        }
    }
    
    buffer[pos] = '\0';
}