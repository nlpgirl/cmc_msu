%include "io.inc"

extern io_get_dec, io_print_dec

section .data
    n dd 0          
    prev dd 0      
    max_len dd 1    
    curr_len dd 1   

section .text
global CMAIN
CMAIN:
    ;read n
    call io_get_dec
    mov [n], eax

    call io_get_dec
    mov [prev], eax

    lp:
        cmp dword[n], 1     ;if there is only one integer left, we should go out of the loop
        jle print_max_len

        dec dword [n]       
        call io_get_dec

        ;check if the next integer is greater than the previous integer
        cmp eax, [prev]
        jle reset_curr_len
        inc dword[curr_len]

        ;update the maximum length if necessary
        mov ebx, [max_len]
        cmp dword[curr_len], ebx
        jle continue

        mov ebx, [curr_len]
        mov dword[max_len], ebx

        continue:
            mov dword[prev], eax
            jmp lp

        reset_curr_len:
            mov dword[curr_len], 1
            mov dword[prev], eax
            jmp lp

    print_max_len:
        mov eax, [max_len]
        call io_print_dec

        xor eax, eax
        ret
