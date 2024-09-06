extern malloc, realloc, scanf, printf, free

section .data
format db "%u", 0
space dd ' '

section .text
global main
main:
    push ebp
    mov ebp, esp
    and esp, ~15
    sub esp, 16
    xor ebx, ebx
    mov esi, 4
    mov dword[esp], esi
    call malloc
    mov edi, eax
    .read:
        mov dword[esp], format
        lea eax, [esp+8]
        mov dword[esp+4], eax
        call scanf
        mov eax, dword[esp+8]
        test eax, eax
        jz .end_read
        mov dword[edi+4*ebx], eax
        inc ebx
        sub esi, 4
        test esi, esi
        jg .read
        
        mov esi, ebx
        imul esi, 12
        mov dword[esp], edi
        mov dword[esp+4], esi
        call realloc
        mov edi, eax
        mov esi, ebx
        imul esi, 8
        jmp .read
    .end_read:
    xor esi, esi
    cmp ebx, 0
    je .endd
    mov eax, dword[edi+4*ebx-4]
    mov ecx, ebx
    xor edx, edx
    .compare:
        cmp dword[edi+4*edx], eax
        jae .skip
        inc esi
        .skip:
        inc edx
        loop .compare
        mov dword [esp+8], ebx
    .end:  
        mov dword [esp+4], esi
        mov dword [esp], format
        call printf
        .endd:
        mov dword[esp], edi
        call free
        mov esp, ebp
        pop ebp
    xor eax, eax
    ret