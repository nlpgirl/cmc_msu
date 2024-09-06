%include "io.inc"

extern io_get_dec, io_print_dec

section .data
month dd 0
day dd 0
x dd 41
two dd 2

section .text
global CMAIN
CMAIN:
    call io_get_dec
    mov [month], eax
    call io_get_dec
    mov [day], eax
    ;result = (month-1)*41 + (month-1)/2 + day
    sub dword[month], 1
    mov eax, dword[month]
    mul dword[x]
    mov [x], eax
    mov eax, [month]
    div dword[two]
    add eax, [x]
    add eax, [day]
    
    call io_print_dec
   
    xor eax, eax
    ret