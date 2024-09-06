section .rodata
string db "%d %d %d", 0

section .text
global main
main:
    ;write your code here
    ;xor eax, eax
    mov eax, 1
    ret