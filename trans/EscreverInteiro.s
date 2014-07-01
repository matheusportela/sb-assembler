section .data

section .bss
output_num: resb 11

section .text
global EscreverInteiro
EscreverInteiro:
	;mov eax,1234567890 	    	;input = eax    // REMOVE AFTERWARDS
	push eax		            ;push eax
    push ebx                    ;push ebx
    push ecx                    ;push ecx
    push edx                    ;push edx

    mov eax, ebx                ;will read from ebx
	mov ecx,9	          	    ;ecx = 0

_loop:
    mov ebx,10                  ;ebx = 10
    mov edx,0
    div ebx		                ;eax /= 10
	mov bl,dl		            ;bl = dl
	add bl,30h                  ;bl = (char)bl
	mov byte [output_num+ecx],bl;char *p = output_num, *(p+ecx*8) = bh 
    dec ecx  
	cmp eax,0h            		;if(eax == 0)  
	jne _loop

    mov byte [output_num+10],0Ah
    
	mov eax,4	    	        ;sys_write
	mov ebx,1		            ;ponteiro para stdout
    mov ecx,output_num  	    ;ponteiro para armazenamento da string
    mov edx,11		            ;size_t = 11 (nenhum número será maior que isso)
   	int 80h			            ;chama interrupção

_ret:
    pop edx                     ;pop edx
    pop ecx                     ;pop ecx
    pop ebx                     ;pop ebx
	pop eax                     ;pop eax

	ret
   	;mov eax,1                   ;REMOVE
	;int 80h                     ;REMOVE
