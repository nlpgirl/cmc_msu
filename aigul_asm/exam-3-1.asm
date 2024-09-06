section .text
global f
f:
    push ebp
    mov ebp, esp
    mov edx, dword[ebp+8]
    test edx, edx
    jz .end_recur
    push edx
    add esp, 4
    push dword[edx+4]
    call f
    sub esp, 8
    pop edx
    movsx ecx, word[edx]
    add eax, ecx
    cdq
    movsx ecx, byte[edx+2]
    div ecx
    mov eax, edx
    jmp .end
    
    .end_recur:
    mov eax, 42
    
    .end:
    leave
    ret