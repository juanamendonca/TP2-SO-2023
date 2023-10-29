GLOBAL sys_int_80
GLOBAL _hlt
sys_int_80:
    push rbp
    mov rbp, rsp
    int 80h
    mov rsp, rbp
    pop rbp
    ret

    _hlt:
    sti
    hlt
    ret
