section text  
; le a entrada
input a
input b
input c

; if (a != b && a != c) goto a_wins;
load  a
sub   b
jmpz  check_b
load  a
sub   c
jmpz  check_b
jmp   a_wins

; else if (b != a && b != c) goto b_wins;
check_b: load zero ; instrucao inutil
load  b
sub   a
jmpz  check_c
load  b
sub   c
jmpz  check_c
jmp   b_wins

; else if (c != a && c != b) goto c_wins; else goto tie;
check_c: load zero ; instrucao inutil
load  c
sub   a
jmpz  tie
load  c
sub   b
jmpz  tie
jmp   c_wins

tie: load zero ; instrucao inutil
output zero
stop

a_wins: load zero ; instrucao inutil
output one
stop

b_wins: load zero ; instrucao inutil
output two
stop

c_wins: load zero ; instrucao inutil
output three
stop

; Zerinho ou Um: Primeiro problema da sub-regional da Maratona de Programacao de 2013 do Brasil.
; Os tres valores lidos na entrada devem ser apenas 0 ou 1.
; Imprime zero se ninguem vence, one se A vence, two se B vence, ou three se C vence.

section data
  
a:      space
b:      space
c:      space
zero:   const 0
one:    const 1
two:    const 2
three:  const 3
  
