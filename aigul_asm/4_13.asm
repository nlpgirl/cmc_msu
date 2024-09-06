extern io_get_dec, io_print_dec, io_print_char

section .data
space dd ' '

section .bss
n resd 1
k resd 1
m resd 1
arr resd 11
copy resd 1

section .text
global main
main:
    call io_get_dec
    mov [n], eax
    call io_get_dec
    mov [k], eax
    call io_get_dec
    mov [m], eax
    
    ; ebx = N^k - 1
    mov ecx, [k]
    dec ecx
    mov eax, [n]
    .power:
    mul dword[n]
    loop .power
    
    dec eax
    mov ebx, eax
    mov esi, -1
    xor edi, edi
    
    
    ; esi iterates from 0 to N^k-1
    ; edi is counter for numbers we search
    
    .for:
        inc esi
        push esi
        call check
        add esp, 4
        cmp eax, 0
        je .next
        inc edi
        .next:
            cmp edi, [m]
            jne .for 
    
    ; print required sequence
    mov eax, esi
    xor ebx, ebx
    .print:
        inc ebx
        cmp ebx, [k]
        ja .end_print 
        xor edx, edx
        div dword[n]
        inc edx
        mov edi, eax
        mov eax, edx
        call io_print_dec
        mov eax, [space]
        call io_print_char
        mov eax, edi
        jmp .print
    .end_print:
    
    xor eax, eax
    ret
    
check:
    push ebp
    mov ebp, esp
    mov eax, dword[ebp+8]
    mov ecx, 10
    .clear:
        mov dword[arr+ecx*4], 0
        loop .clear
        
    mov ecx, [k]
    mov edi, [n]
    
    .forc:
        xor edx, edx
        div edi
        inc edx
        inc dword[arr+edx*4]
        loop .forc
        
    mov ecx, 10
    .check_arr:
        mov eax, dword[arr+ecx*4]
        cmp eax, 1
        ja .skip
        loop .check_arr
        mov eax, 1
        jmp .end
    .skip:
        xor eax, eax
    .end:
        mov esp, ebp
        pop ebp
        ret
        