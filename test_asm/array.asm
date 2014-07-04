; @file   array.asm
; @author Matheus Vieira Portela
; @date   07/05/2014
;
; @brief Read an array from input and multiply by 3

SECTION TEXT
    ; Initialise vector
    INPUT V[0]
    INPUT V[1]
    INPUT V[2]
    INPUT V[3]
    
    ; Multiply each element by 3
    LOAD V[0]
    MULT THREE
    STORE V[0]
    
    LOAD V[1]
    MULT THREE
    STORE V[1]
    
    LOAD V[2]
    MULT THREE
    STORE V[2]
    
    LOAD V[3]
    MULT THREE
    STORE V[3]
    
    ; Print vector
    OUTPUT V[0]
    OUTPUT V[1]
    OUTPUT V[2]
    OUTPUT V[3]
    
    ; Exit
    STOP

SECTION DATA
ONE: CONST 1
TWO: CONST 2
THREE: CONST 3
FOUR: CONST 4
V:   SPACE 4