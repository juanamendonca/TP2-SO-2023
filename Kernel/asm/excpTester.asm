GLOBAL divzerotester
GLOBAL opcodetester

divzerotester:

    mov rdx, 0
    mov rax, 3
    mov rcx, 0
    div rcx

    ret

opcodetester:

    ud2         ;test invalid opcode
    ret


