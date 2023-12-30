section .text
bits 64
GDT_ENTRY_SIZE equ 8

;
; void FlushGDT(GDTR* pGDTR);
;

global FlushGDT
FlushGDT:
    lgdt [rdi]
    ; Kernel data segment (index 2)
    mov ax, (2 * GDT_ENTRY_SIZE)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Get return address and prepare for long jump
    pop rdi

    ; Kernel code segment (index 1)
    mov rax, (1 * GDT_ENTRY_SIZE)
    push rax
    push rdi

    ; Do a sick backwards long jump back to code
    retfq
