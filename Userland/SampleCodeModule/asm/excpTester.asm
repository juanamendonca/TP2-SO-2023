GLOBAL divzerotester
GLOBAL opcodetester

divzerotester:

    mov rcx, 3
    mov rdx, 4
    mov rax, 0
	div rax
	ret

opcodetester:
    db 0xFF, 0xFF         ;test invalid opcode
    ret
    