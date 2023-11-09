GLOBAL cpuVendor
GLOBAL kbFlag

GLOBAL getHours
GLOBAL getMinutes
GLOBAL getSeconds
GLOBAL outb
GLOBAL inb
GLOBAL _xchg
section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getSeconds:
    push rbp
    mov rbp,rsp
    mov al, 0
    out 70h, al
    in al, 71h
    mov rsp, rbp
    pop rbp
    ret

getMinutes:
    push rbp
    mov rbp,rsp
    mov al, 2
    out 70h, al
    in al, 71h
    mov rsp, rbp
    pop rbp
    ret

getHours:
    push rbp
    mov rbp, rsp
    mov rax,0
    mov al, 4
    out 70h, al
    in al, 71h
    mov rsp, rbp
    pop rbp
    ret

inb:
	push rbp
	mov rbp, rsp
	mov rdx, rdi
	mov rax, 0
    in al, dx
	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
	mov rbp, rsp
	mov rax, rsi
	mov rdx, rdi
	out dx, al
	mov rsp, rbp
	pop rbp
	ret

_xchg:
	mov rax, rsi
	xchg [rdi], eax
	ret