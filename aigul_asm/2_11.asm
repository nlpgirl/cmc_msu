%include "io.inc"

extern io_get_char, io_print_udec

section .data
x dd 0
y dd 0

section .text
global CMAIN
CMAIN:
    
    call io_get_char
    mov [x], eax
    call io_get_char
    mov [y], eax
    
    sub dword[x], '@'  ; '@' is before 'A' in ASCII table
    sub dword[y], '0'
    
    ;result = (8-x)*(8-y)/2 + (x+y)%2
    mov eax, 8
    sub eax, [x]
    mov ebx, 8
    sub ebx, [y]
    mul ebx
    mov ebx, 2
    div ebx
    mov [x], eax
    
    
    mov eax, [x]
    add eax, [y]
    mov ebx, 2
    div ebx
    mov bl, ah
    add dword[x], ebx
    mov eax, [x]
    
    call io_print_udec
    
    xor eax, eax
    ret