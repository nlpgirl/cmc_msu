%include "io.inc"
extern io_get_dec, io_print_dec

section .bss
n resd 1
k resd 1

section .data
count dd 0

global CMAIN
CMAIN:
    call io_get_dec
    mov [n], eax
    call io_get_dec
    mov [k], eax
    mov ecx, 0
    mov edi, 2
    
        .start:
        inc ecx
        cmp ecx, [n]
        jg .end
        mov eax, ecx
        not eax
            .sum:
            cmp eax, 0
            je .sum_found
            xor edx, edx
            div edi
            add esi, edx ;esi stores amount of zeros
            jmp .sum

            .sum_found:
            cmp esi, [k]
            je .inc_count
            jmp .start
            
            .inc_count:
             inc dword[count]
             jmp .start
        
        .end:
    mov eax, [count]
    call io_print_dec
    
    xor eax, eax
    ret