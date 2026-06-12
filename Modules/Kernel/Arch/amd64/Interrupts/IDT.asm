section .text
bits 64

;
; void FlushIDT(IDTR* pIDTR);
;

global FlushIDT
FlushIDT:
    lidt [rdi]
    ret
