section .text
bits 64
align 4
; FIXME: Explain why / how the magic 0x08 and 0x10 are working

global FlushGDT
FlushGDT:
    push 0x08       ; Kernel code segment
    lea rax, [rel .flush]
    ret

.flush:
    mov ax, 0x10    ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax

    ret
