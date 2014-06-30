; @file   fibonacci.asm
; @author Matheus Vieira Portela
; @date   07/05/2014
;
; @brief Print Fibonacci series until a limit value (given by input)

SECTION TEXT
	COPY ZERO, OLDER
	COPY ONE, OLD[0]
	INPUT LIMIT
	OUTPUT OLD[0]
FRONT:	LOAD OLDER
	ADD OLD[0]
	STORE NEW[3]
	SUB LIMIT
	JMPP FINAL
	OUTPUT NEW[3]
	COPY OLD[0], OLDER
	COPY NEW[3], OLD[0]
	JMP FRONT
FINAL:	OUTPUT LIMIT
	STOP
	
SECTION DATA
ZERO:	CONST 0
ONE:	CONST 1
OLDER:	SPACE
OLD:	SPACE 2
NEW:	SPACE 4
LIMIT:	SPACE