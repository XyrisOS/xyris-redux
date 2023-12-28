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
    add rsp, 16
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
    global InterruptStub%1
    InterruptStub%1:
        push 0
        push %1
        jmp InterruptHandlerProxy
%endmacro

%macro InterruptError 1
    global InterruptStub%1
    InterruptStub%1:
        push %1
        jmp InterruptHandlerProxy
%endmacro

;
; Generates a table of all interrupt (and exception) symbols for
; referencing by the C/C++ side in order to reduce code duplication.
;
global InterruptTable
InterruptTable:
%assign i 0
%rep    256
    dq InterruptStub%+i
%assign i i+1
%endrep

Interrupt 0
Interrupt 1
Interrupt 2
Interrupt 3
Interrupt 4
Interrupt 5
Interrupt 6
Interrupt 7
InterruptError 8
Interrupt 9
InterruptError 10
InterruptError 11
InterruptError 12
InterruptError 13
InterruptError 14
Interrupt 15
Interrupt 16
Interrupt 17
Interrupt 18
Interrupt 19
Interrupt 20
Interrupt 21
Interrupt 22
Interrupt 23
Interrupt 24
Interrupt 25
Interrupt 26
Interrupt 27
Interrupt 28
Interrupt 29
Interrupt 30
Interrupt 31
Interrupt 32
Interrupt 33
Interrupt 34
Interrupt 35
Interrupt 36
Interrupt 37
Interrupt 38
Interrupt 39
Interrupt 40
Interrupt 41
Interrupt 42
Interrupt 43
Interrupt 44
Interrupt 45
Interrupt 46
Interrupt 47
Interrupt 48
Interrupt 49
Interrupt 50
Interrupt 51
Interrupt 52
Interrupt 53
Interrupt 54
Interrupt 55
Interrupt 56
Interrupt 57
Interrupt 58
Interrupt 59
Interrupt 60
Interrupt 61
Interrupt 62
Interrupt 63
Interrupt 64
Interrupt 65
Interrupt 66
Interrupt 67
Interrupt 68
Interrupt 69
Interrupt 70
Interrupt 71
Interrupt 72
Interrupt 73
Interrupt 74
Interrupt 75
Interrupt 76
Interrupt 77
Interrupt 78
Interrupt 79
Interrupt 80
Interrupt 81
Interrupt 82
Interrupt 83
Interrupt 84
Interrupt 85
Interrupt 86
Interrupt 87
Interrupt 88
Interrupt 89
Interrupt 90
Interrupt 91
Interrupt 92
Interrupt 93
Interrupt 94
Interrupt 95
Interrupt 96
Interrupt 97
Interrupt 98
Interrupt 99
Interrupt 100
Interrupt 101
Interrupt 102
Interrupt 103
Interrupt 104
Interrupt 105
Interrupt 106
Interrupt 107
Interrupt 108
Interrupt 109
Interrupt 110
Interrupt 111
Interrupt 112
Interrupt 113
Interrupt 114
Interrupt 115
Interrupt 116
Interrupt 117
Interrupt 118
Interrupt 119
Interrupt 120
Interrupt 121
Interrupt 122
Interrupt 123
Interrupt 124
Interrupt 125
Interrupt 126
Interrupt 127
Interrupt 128
Interrupt 129
Interrupt 130
Interrupt 131
Interrupt 132
Interrupt 133
Interrupt 134
Interrupt 135
Interrupt 136
Interrupt 137
Interrupt 138
Interrupt 139
Interrupt 140
Interrupt 141
Interrupt 142
Interrupt 143
Interrupt 144
Interrupt 145
Interrupt 146
Interrupt 147
Interrupt 148
Interrupt 149
Interrupt 150
Interrupt 151
Interrupt 152
Interrupt 153
Interrupt 154
Interrupt 155
Interrupt 156
Interrupt 157
Interrupt 158
Interrupt 159
Interrupt 160
Interrupt 161
Interrupt 162
Interrupt 163
Interrupt 164
Interrupt 165
Interrupt 166
Interrupt 167
Interrupt 168
Interrupt 169
Interrupt 170
Interrupt 171
Interrupt 172
Interrupt 173
Interrupt 174
Interrupt 175
Interrupt 176
Interrupt 177
Interrupt 178
Interrupt 179
Interrupt 180
Interrupt 181
Interrupt 182
Interrupt 183
Interrupt 184
Interrupt 185
Interrupt 186
Interrupt 187
Interrupt 188
Interrupt 189
Interrupt 190
Interrupt 191
Interrupt 192
Interrupt 193
Interrupt 194
Interrupt 195
Interrupt 196
Interrupt 197
Interrupt 198
Interrupt 199
Interrupt 200
Interrupt 201
Interrupt 202
Interrupt 203
Interrupt 204
Interrupt 205
Interrupt 206
Interrupt 207
Interrupt 208
Interrupt 209
Interrupt 210
Interrupt 211
Interrupt 212
Interrupt 213
Interrupt 214
Interrupt 215
Interrupt 216
Interrupt 217
Interrupt 218
Interrupt 219
Interrupt 220
Interrupt 221
Interrupt 222
Interrupt 223
Interrupt 224
Interrupt 225
Interrupt 226
Interrupt 227
Interrupt 228
Interrupt 229
Interrupt 230
Interrupt 231
Interrupt 232
Interrupt 233
Interrupt 234
Interrupt 235
Interrupt 236
Interrupt 237
Interrupt 238
Interrupt 239
Interrupt 240
Interrupt 241
Interrupt 242
Interrupt 243
Interrupt 244
Interrupt 245
Interrupt 246
Interrupt 247
Interrupt 248
Interrupt 249
Interrupt 250
Interrupt 251
Interrupt 252
Interrupt 253
Interrupt 254
Interrupt 255
