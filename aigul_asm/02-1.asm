%include "io.inc"
extern io_get_dec, io_print_dec, io_print_char

section .bss
    vx resd 1
    vy resd 1
    ax2 resd 1
    ay2 resd 1
    t resd 1
    
section .data
    c db ' '

section .text
global CMAIN
CMAIN:
    call io_get_dec
    mov [vx], eax
    call io_get_dec
    mov [vy], eax
    call io_get_dec
    mov [ax2], eax
    call io_get_dec
    mov [ay2], eax
    call io_get_dec
    mov [t], eax
    ; x = vx * t + ax2 * t * t
    mov eax, [vx]
    imul dword [t]
    mov ebx, [ax2]
    imul ebx, [t]
    imul ebx, [t]
    add eax, ebx
    call io_print_dec
    mov al, [c]
    call io_print_char
    mov eax, [vy]
    imul dword [t]
    mov ebx, [ay2]
    imul ebx, [t]
    imul ebx, [t]
    add eax, ebx
    call io_print_dec
    
    
    
    xor eax, eax
    ret