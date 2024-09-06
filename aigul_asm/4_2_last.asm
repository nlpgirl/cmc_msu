extern io_get_udec, io_print_udec

section .data
three dd 3

section .text
global main
main:
    call io_get_udec
    push eax
    call f
    call io_print_udec
    add esp, 4
    xor eax, eax
    ret
    
f:
    push ebp
    mov ebp, esp
    mov eax, dword[ebp+8]
    cmp eax, 0
    jz .end
    xor edx, edx
    div dword[three]
    push edx
    push eax
    call f
    add esp, 4
    pop edx
    
    and edx, 1
    add eax, edx
.end:
    mov esp, ebp
    pop ebp
    ret
    