; Special thanks to Brutal (sleepymonax et al) for this code
; the old Xyris code was mostly working, but because of the
; removal of long jump in x86_64, this Brutal provided a nice
; nasm implementation to reference.

section .text
bits 64
align 4
; FIXME: Explain why / how the magic 0x08 and 0x10 are working

global GDT_Flush
GDT_Flush:
    lgdt [rdi]
    mov ax, 0x10        ; Kernel data segment
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov rax, qword .blj ; Yahoo
    push qword 0x8      ; Kernel code segment
    push rax
    o64 retf

.blj:
    ret
