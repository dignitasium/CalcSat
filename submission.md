# Release / Demo version
This project implements a feature-rich scientific calculator on the TM4C123GH6PM microcontroller platform. 
It supports floating‑point arithmetic with PEMDAS operator precedence, addition, subtraction, multiplication, division, and scientific notation using ×10^n. 
A shift key extends the keypad, enabling advanced operators, clear‑entry, and full calculator‑style memory functions (MS, MR, MC, M+, M−). 
The user interface runs on a character LCD with real‑time expression feedback and clear error reporting, for example on division by zero. 
A set of numeric easter‑egg codes trigger custom messages and launch several mini‑games, demonstrating tight integration between the calculator core, keypad, and display subsystems.

# Project highlights
The most technically impressive part of the project is the calculator engine that parses a stream of key presses into operands and operators and then evaluates the expression using PEMDAS precedence rules. 
Instead of evaluating strictly left‑to‑right, the implementation stores operands and operators in arrays and performs a two‑pass reduction: first resolving multiplication, division, and scientific notation, then applying addition and subtraction. 
This mirrors a simple expression parser and shows modular decomposition between input handling, state management, and numeric evaluation.
The design encapsulates all calculator state in a single Calculator struct, which is passed by pointer to pure functions.
This follows software engineering principles such as separation of concerns, low coupling, and high cohesion, and makes the code base easier to test and extend.

The most creative and innovative feature is the integration of easter eggs and mini‑games into the calculator workflow. 
Specific numeric codes double as “cheat codes” that unlock a reaction test, snake, and other games when the equals key is pressed.
This required a clean interface between the calculator module and the games module, so that control can safely pass to a game and then return to a reset calculator context.
The same keypad and LCD hardware are reused across modes, maximising the value of the embedded platform and demonstrating how a constrained interface can deliver both a serious calculator tool and playful interactive experiences.

# Project reflection
Working on this mini‑project provided a practical way to address all of the module learning outcomes. 

## LO1: Deriving a specification began with mapping the requirements of a “scientific‑style” calculator onto the TM4C123GH6PM LaunchPad and a 4×4 keypad.
The need for PEMDAS precedence, floating‑point support, memory functions, and games led to clear functional and non‑functional requirements, including display limitations and key mappings.

## LO2: Applying embedded system design principles was central to the implementation. The project separates hardware drivers (LCD, keypad, system clock) from the calculator logic, which allowed the same calculator code to run irrespective of low‑level pin details. 
The use of a state machine for key processing simplified behaviour such as shift mode, error handling, and transitioning between entering numbers, operators, and results.

## LO3: Implementing the microcontroller solution in hardware involved configuring GPIO lines, timing the LCD, and debouncing the keypad to achieve reliable user input. 
Debugging on real hardware highlighted issues that do not appear in simulation, such as contact bounce and display update timing.

## LO4: Throughout the project, technical literature such as the TM4C123 datasheet, LCD controller documentation, and example keypad‑scanning code was consulted and evaluated. 
These sources informed design decisions, for example the debounce delay and pin configuration, while the final code was tailored to the specific board and assignment constraints. 

Overall, the project demonstrates meaningful progress against each learning outcome and provides a foundation for more complex embedded applications in the future.
