section .text
bits 64
GDT_ENTRY_SIZE equ 8

KERNEL_CODE_INDEX equ 1
KERNEL_DATA_INDEX equ 2
TASK_STATE_INDEX  equ 6

;
; void FlushGDT(GDTR* pGDTR);
;

global FlushGDT
FlushGDT:
    lgdt [rdi]

    ; Load task state register (index 6..7)
    mov ax, (TASK_STATE_INDEX * GDT_ENTRY_SIZE)
    ltr ax

    ; Kernel data segment (index 2)
    mov ax, (KERNEL_DATA_INDEX * GDT_ENTRY_SIZE)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Get return address and prepare for long jump
    pop rdi

    ; Kernel code segment (index 1)
    mov rax, (KERNEL_CODE_INDEX * GDT_ENTRY_SIZE)
    push rax
    push rdi

    ; Do a sick backwards long jump back to code
    retfq
