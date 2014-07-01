section .data

section .bss
input_num	resb	10

section .text
global LerInteiro
LerInteiro:
	    push eax		    ;push eax
        ;push ebx            ;push ebx
        ;push ecx            ;push ecx
        ;push edx            ;push edx

	    mov eax,3	    	;sys_read
	    mov ebx,0		    ;ponteiro para stdout
    	mov ecx,input_num	;ponteiro para armazenamento da string
    	mov edx,10	    	;size_t = 10 (nenhum número será maior que isso)
    	int 80h			    ;chama interrupção

    	push 0		    	;push 0
        mov eax,0           ;eax = 0
        mov ebx,0           ;ebx = 0
    	mov ecx,input_num	;int *ecx = input_num;
    	cmp byte [ecx],2Dh	;if(*ecx < 0)
    	jne _nneg
        push 2Dh            ;push '-'
        push 0
    	inc ecx		    	;ecx++
        jmp _loop

_nneg:	push 2Bh
        push 0

_loop:  cmp dword [ecx],0Ah ;while(*ecx != 0x0A) // '\n'
    	je _end

    	pop ax		    	;pop ax
    	push bx       	    ;ax *= 10
        mov bx,10
        mul bx
        pop bx
        push ecx            ;ax += (int)(*ecx)
        mov ecx,[ecx]
    	sub cx,30h
    	add al,cl
        pop ecx
    	push ax			    ;push ax
    	inc ecx		    	;ecx++
    	jmp _loop

_end:  	pop eax             ;pop eax
        pop ecx             ;pop ecx
        cmp ecx,2Dh         ;if(eax == '-')
        jne _ret
    	neg eax			    ;ax = ~ax+1	// complemento a dois

_ret:  	mov bx, ax
        add esp, 4
        ;pop edx             ;pop edx
        ;pop ecx             ;pop ecx
        ;pop ebx             ;pop ebx
	    pop eax             ;pop eax

        ret
        ;mov eax,1           ;REMOVE
        ;int 80h             ;REMOVE
