extern io_get_dec, io_print_dec

section .bss
n resd 1
k resd 1

section .data
ans dd 0

section .text
global main
main:
    call io_get_dec
    mov [n], eax
    call io_get_dec
    mov [k], eax
    
    mov ebx, 0
    mov edi, 2
    
    .for:
        inc ebx
        cmp ebx, dword[n]
        ja .end_for
        mov eax, ebx
        xor ecx, ecx
        
        .while:
            cmp eax, 0
            je .end_while
            xor edx, edx
            div edi
            cmp edx, 0
            je .inc
            jmp .while
            
        .inc:
            inc ecx
            jmp .while
            
        .end_while:
            cmp ecx, [k]
            je .inc_answer
            jmp .for
            
            .inc_answer:
                inc dword[ans]
                jmp .for
    .end_for:
        mov eax, [ans]
        call io_print_dec
        
        xor eax, eax
        ret
            
            
            
    