GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler

GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _int80Handler


GLOBAL _exception6Handler
GLOBAL _exception0Handler

GLOBAL regs
GLOBAL restore_stack

GLOBAL callTimer

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN sysDispatcher
EXTERN getStackBase
EXTERN scheduler

SECTION .text

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

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; passes parameter
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

restore_stack:
	mov [prevBS],rbx
	pop rbx
	call getStackBase
	mov rsp,rax
	push rbx
	mov rbx,[prevBS]
	ret

%macro exceptionHandler 1
	pushState

	mov [regs+8], rbx
	mov [regs+2*8], rcx
	mov [regs+3*8], rdx
	mov [regs+4*8], rsi
	mov [regs+5*8], rdi
	mov [regs+6*8], rbp
	mov [regs+7*8], r8
	mov [regs+8*8], r9
	mov [regs+9*8], r10
	mov [regs+10*8], r11
	mov [regs+11*8], r12
	mov [regs+12*8], r13
	mov [regs+13*8], r14
	mov [regs+14*8], r15

	mov rax, [rsp+15*8]
	mov [regs+15*8], rax ;RIP

	mov rax, [rsp+14*8]
	mov [regs], rax ;RAX

	mov rdi, %1 ; passing parameter
	call exceptionDispatcher

	popState


	call getStackBase ; Leaves result in RAX
	mov [rsp+3*8], rax

;--> STACK: RSPORIg : FLAGS : CS : RIP
;           0         1       2    3
;		  CALL BASE DEL STACK (CALL STACK BASE)

	mov rax, 0x400000
	mov [rsp], rax
	iretq

%endmacro

_int80Handler:
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
	call sysDispatcher
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
	iretq

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = 16 bit mask
    out	0A1h,al
    pop     rbp
    retn


;Timer Tick
_irq00Handler:
	pushState

	mov rdi, 0 ; passes parameter
	call irqDispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01Handler:
	pushState
	mov rax, 0
	in al, 60h

	;1Dh is the scancode for CTRL
	cmp al, 1Dh
	jne .continue

	;Order: "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"
	mov [regs+8], rbx
	mov [regs+2*8], rcx
	mov [regs+3*8], rdx
	mov [regs+4*8], rsi
	mov [regs+5*8], rdi
	mov [regs+6*8], rbp
	mov [regs+7*8], r8
	mov [regs+8*8], r9
	mov [regs+9*8], r10
	mov [regs+10*8], r11
	mov [regs+11*8], r12
	mov [regs+12*8], r13
	mov [regs+13*8], r14
	mov [regs+14*8], r15

	mov rax, [rsp+14*8]
	mov [regs], rax ;RAX

	mov al, 1Dh
	jmp .continue

.continue:

	; the parameter is the scancode
	mov rdi, 1
	mov rsi, rax
	call irqDispatcher

.end:

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Operation Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

callTimer:
    int 20h
    ret

SECTION .bss
	aux resq 1

section .bss
regs resb 8*18	 ;x86-64. Each register has 8 bytes.
prevBS 	resq 1


