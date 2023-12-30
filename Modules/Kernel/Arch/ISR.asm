section .text
bits 64

;
; [ Interrupts.cpp ]
; void InterruptHandler(InterruptFrame* frame);
;

extern InterruptHandler

;
; `pusha` and `popa` (along with their i386 extensions) are not valid
; on x86_64 since registers r8 - r15 are not preserved, only i386 general
; purpose registers. As such all registers need to be pushed / popped
; manually.
;

%macro pushax 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

;
; Proxy call to InterruptHandler that preserves register information
; (via pushax, popax). Keep in mind that there is additional information
; already on the stack that was pushed by the CPU the moment an interrupt
; occured. The specified structure is defined to match the state of the
; stack set by this proxy call.
;
; struct InterruptFrame
; [ Interrupts.cpp ]
;

InterruptHandlerProxy:
    cld
    pushax
    mov rdi, rsp
    call InterruptHandler
    popax
    add rsp, 16 ; pop error code and interrupt number
    iretq

;
; Some interrupts have an associated error code (as is common with most
; exceptions) but most general purpose interrupts do not. These macros
; are defined such that an error code is preserved if provided, otherwise
; a default value of `0` is used.
;
; Additionally these macros define symbols that can be referenced by the
; C/C++ side so that the handlers can be registered with the IDT.
;

%macro Interrupt 1
    global Interrupt%1
    Interrupt%1:
        push qword 0  ; Default error code value
        push qword %1
        jmp InterruptHandlerProxy
%endmacro

%macro InterruptError 1
    global Interrupt%1
    Interrupt%1:
        ; Error code pushed by CPU
        push qword %1
        jmp InterruptHandlerProxy
%endmacro

;
; Defines all 256 interrupts by calling the appropriate macro with the
; associated interrupt number. Certain interrupts include an error code,
; and those interrupts call into `InterruptError` instead of `Interrupt`.
;
%assign i 0
%rep    256
    %if (i == 8 || (i >= 10 && i <= 14) || i == 17 || i == 30)
        InterruptError i
    %else
        Interrupt i
    %endif
%assign i i+1
%endrep

section .data

;
; Generates a table of all interrupt (and exception) symbols for
; referencing by the C/C++ side in order to reduce code duplication.
;
; Interrupts 0..32 (inclusive) are CPU exceptions.
;
global InterruptTable
InterruptTable:
%assign i 0
%rep    256
    dq Interrupt%+i
%assign i i+1
%endrep
