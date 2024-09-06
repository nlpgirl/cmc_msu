extern io_get_dec, io_print_dec

section .data
sum dd 0
n dd 0

section .text
global main
main:
    call io_get_dec
    mov ebx, eax  ; ebx = K
    xor esi, esi ; esi = counter for insufficient numbers
    mov ecx, 1 ; ecx = current number to be checked
    .while:
        push ecx
        call check  ;
        pop ecx
        cmp eax, 1
        je .next
        inc esi
        cmp esi, ebx
        je .end
        .next:
            inc ecx
            jmp .while
        
        .end:
            mov eax, ecx
            call io_print_dec
        
check:
    push ebp
    mov ebp, esp
    mov eax, 0
    mov dword[sum], eax
    mov eax, dword[ebp+8]
    mov dword[n], eax ; n = number to be checked
    cmp dword[n], 1
    je .next_step
    mov esi, 1 ; esi stands for current divider
    xor edx, edx
    jmp .skip ; skip division when divider is 1
    .whilef:
        mov eax, [n]
        xor edx, edx
        div esi
        cmp edx, 0
        jne .next_step
        add dword[sum], eax
        .skip:
        add dword[sum], esi
        .next_step:
        inc esi
        mov eax, esi
        mul eax
        cmp eax, [n]
        jb .whilef
    je .if
    jmp .end_if
    .if:
        add dword[sum], esi
    .end_if:
        mov eax, [sum]
        cmp eax, [n]
        jb .insufficient
        mov eax, 1
        jmp .endf
        .insufficient:
            mov eax, 0
    .endf:
        mov esp, ebp
        pop ebp
        ret
        
            
    xor eax, eax
    ret