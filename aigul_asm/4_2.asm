extern io_get_udec, io_print_udec

section . data
three dd 3
copy dd 0

section .text
global main
main:
    call io_get_udec
    mov ebx, eax
    xor eax, eax
    xor ecx, ecx
    xor edx, edx
    call f
    mov eax, ecx
    call io_print_udec
    xor eax, eax
    ret
    
f:
    cmp ebx, 0
    ja rec
    ;xor edx, edx
    ;xor ecx, ecx
    ret
    
rec:
    mov eax, ebx
    xor edx, edx
    div dword[three]
    mov ebx, eax
    call f
    and edx, 1
    add ecx, edx
    ;xor edx, edx
    ;mov eax, ecx
    ret
    