global testRegs

section .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

testRegs:
    pushState
	mov rax, 2
	mov rbx, 2
	mov rcx, 2
	mov rdx, 6
	mov rsi, 8
    mov r10, 10
	mov r11, 11
	mov r12, 12
	mov r13, 13
	mov r14, 14
	mov r15, 15
    mov r9, 9
    mov r8, 1000000000
	mov rbp, 1
	mov rdi, 1

loop:
    sub r8, 1     ; Subtract 1 from counter r8
    cmp r8, 0
    jne loop      ; Jumps to 'loop' if r8 is not equal to cero

    popState

    ret