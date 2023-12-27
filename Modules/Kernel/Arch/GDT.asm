; Special thanks to Brutal (sleepymonax et al) for this code
; the old Xyris code was mostly working, but because of the
; removal of long jump in x86_64, this Brutal provided a nice
; nasm implementation to reference.

section .text
bits 64
align 4
GDT_ENTRY_SIZE equ 8

global GDT_Flush
GDT_Flush:
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
