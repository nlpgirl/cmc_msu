%include "io.inc"

extern io_get_udec
extern io_print_udec

section .data
a db 0
b db 0
c db 0
d db 0
x dd 0

section .text
global CMAIN

CMAIN:
call io_get_udec
mov [a], al
call io_get_udec
mov [b], al
call io_get_udec
mov [c], al
call io_get_udec
mov [d], al

mov eax, dword [a]
mov ebx, dword [b]
shl ebx, 8
or eax, ebx
mov ebx, dword [c]
shl ebx, 16
or eax, ebx
mov ebx, dword [d]
shl ebx, 24
or eax, ebx

call io_print_udec

xor eax, eax
ret
