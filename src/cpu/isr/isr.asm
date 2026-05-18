bits 64

global isr_common
global isr_stub_table
extern isr_handler

section .text

; ----------------------------------------
; Common ISR entry
; ----------------------------------------
isr_common:
    ; Save general-purpose registers
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Save old base pointer
    mov rbp, ds
    push rbp

    ; Load segment selectors
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Pass pointer to stack to handler
    mov rdi, rsp
    call isr_handler

    ; Restore old segment selectors
    pop rbp
    mov bx, bp
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx

    ; Restore general-purpose registers
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16      ; Skip error code pushed for interrupts without errors
    iretq

; ----------------------------------------
; ISR stubs with macros
; ----------------------------------------
%macro isr_err_stub 1
global isr_stub%1
isr_stub%1:
    push %1
    jmp isr_common
%endmacro

%macro isr_no_err_stub 1
global isr_stub%1
isr_stub%1:
    push 0
    push %1
    jmp isr_common
%endmacro

; Generate all 256 ISR stubs
%assign i 0
%rep 256
    %if i = 8 || i = 10 || i = 11 || i = 12 || i = 13 || i = 14 || i = 17 || i = 21 || i = 29 || i = 30
        isr_err_stub i
    %else
        isr_no_err_stub i
    %endif
    %assign i i+1
%endrep

; ----------------------------------------
; ISR stub table
; ----------------------------------------

global isr_stub_table
isr_stub_table:
%assign i 0
%rep 256
    dq isr_stub %+ i
    %assign i i+1
%endrep
