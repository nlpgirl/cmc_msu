%include "io.inc"
extern io_get_dec, io_print_dec, io_print_char

section .data
    n dd 0
    space db ' '
    max1 dd -2147483648
    max2 dd -2147483648
    max3 dd -2147483648

section .text
global CMAIN
CMAIN:
    ;read n
    call io_get_dec
    mov [n], eax
    
    read_loop:
    ;check if the end of input has been reached
    cmp dword[n], 0
    je output_max_values
    dec dword[n]

    call io_get_dec
    
    ;check if the current number is greater than max1
    cmp eax, [max1]
    jle check_max2

    ;update max1, max2, max3
    mov ebx, [max2]
    mov [max3], ebx
    mov ebx, [max1]
    mov [max2], ebx
    mov [max1], eax
    jmp read_loop
    
    check_max2:
    ;check if the current number is greater than max2
    cmp eax, [max2]
    jle check_max3

    ;update max2 and max3
    mov ebx, [max2]
    mov [max3], ebx
    mov [max2], eax
    jmp read_loop

    check_max3:
    ;check if the current number is greater than max3
    cmp eax, [max3]
    jle read_loop

    ;update max3
    mov [max3], eax
    jmp read_loop
    
    output_max_values:
    mov eax, [max1]
    call io_print_dec
    
    mov al, [space]
    call io_print_char
    
    mov eax, [max2]
    call io_print_dec
    
    mov al, [space]
    call io_print_char
    
    mov eax, [max3]
    call io_print_dec
    
    xor eax, eax
    ret