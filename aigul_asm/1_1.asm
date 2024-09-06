section .text
global f
f:
    push ebp
    mov ebp, esp
    push ebx
    mov ecx, dword[ebp+8]
    test ecx, ecx
    jz .rec
    
    push ecx
    mov edx, dword[ecx+3]
    push edx
    call f
    add esp, 4
    pop ecx
    add eax, word[ecx]
    movsx ebx, byte[ecx+2]
    cdq
    idiv ebx
    mov eax, edx
    jmp .end
            
    .rec:
    mov eax, 42
    
    .end:
    pop ebx
    mov esp, ebp
    pop ebp
    ret
        