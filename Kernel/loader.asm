global loader
global reset
EXTERN main
EXTERN initializeKernelBinary
EXTERN  getStackBase

loader:
	call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
	mov rsp, rax				; Set stack with the returned address
	call main
hang:
	cli
	hlt	; halt machine should kernel return
	jmp hang





