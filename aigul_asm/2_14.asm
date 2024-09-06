%include "io.inc"

extern io_get_dec, io_print_dec

section .bss
n resd 1
m resd 1
k resd 1
d resd 1
x resd 1
y resd 1

section .text
global CMAIN
CMAIN:
    call io_get_dec
    mov [n], eax 
    call io_get_dec
    mov [m], eax
    call io_get_dec
    mov [k], eax
    call io_get_dec
    mov [d], eax
    call io_get_dec
    mov [x], eax
    call io_get_dec
    mov [y], eax
    
    xor edx, edx
    mov eax, [n]
    mul dword[m]
    mul dword[k]
    
    add eax, [d]
    dec eax
    xor edx, edx
    div dword[d]
    mov [n], eax ; n - amount of boxes
    
    mov eax, [x]
    mov ebx, 6
    xor edx, edx
    div ebx
    add eax, 2
    mov ebx, 3
    xor edx, edx
    div ebx
    mov [x], eax ; eax=0, if 0<=x<=5 , eax=1, if 6<=x<=23
    
    mov eax, [n]
    add eax, 2
    mov ebx, 3
    xor edx, edx
    div ebx
    
    xor edx, edx
    mul dword[x]
    sub [n], eax
    mov eax, [n]
    call io_print_dec
    
    xor eax, eax
    ret