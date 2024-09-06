extern io_get_dec, io_print_dec, io_print_char

section .data
space dd ' '

section .text
global main
main:
    mov ecx, 1
    push ecx
    call rec
    add esp, 4
    xor eax, eax
    ret
    
rec:
    push ebp
    mov ebp, esp
    call io_get_dec
    cmp eax, 0
    je .end
    mov ecx, dword[ebp+8] ;ecx is for number
    test ecx, 1
    jnz .odd
    
    push eax
    inc ecx
    push ecx
    call rec
    ;add esp, 4 ;vopros
    pop eax
    call io_print_dec
    mov eax, [space]
    call io_print_char
    
    .odd:
    call io_print_dec
    mov eax, [space]
    call io_print_char
    inc ecx
    push ecx
    call rec
    add esp, 4
    
.end:
    mov esp, ebp
    pop ebp
    ret
    
    
    