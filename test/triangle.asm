; @file   triangle.asm
; @author Matheus Vieira Portela
; @date   07/05/2014
;
; @brief Calculate a triangle area (base and area are given by input)

SECTION DATA
B:   SPACE
H:   SPACE
R:   SPACE
TWO: CONST  2

SECTION TEXT
INPUT   B
INPUT   H
LOAD    B
MULT    H
DIV     TWO
STORE   R
OUTPUT  R
STOP