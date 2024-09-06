section .data
    format_in: db "%d", 0
    format_str: db "%s", 0
    format_out: db "%d", 0
    max_len equ 11

section .bss
    n resd 1
    string resb max_len
    count resd 1
    strs resb max_len * 500

section .text
    global main
    extern scanf, printf, strcmp, strcpy

main:
    push ebp
    mov ebp, esp
    and esp, ~15
    sub esp, 16

    sub esp, 8
    push n
    push format_in
    call scanf
    add esp, 16

    xor ebx, ebx

.loop:
    cmp ebx, [n]
    jge .end_loop
    
    sub esp, 8
    push string
    push format_str
    call scanf
    add esp, 16

    xor ecx, ecx
    mov edi, strs
    .search:
        cmp ecx, [count]
        jge .add_str

        push ecx
        sub esp, 4
        push edi
        push string
        call strcmp
        add esp, 12
        pop ecx

        test eax, eax
        jz .end_search

        add edi, max_len
        inc ecx
        jmp .search

    .add_str:
        sub esp, 8
        push string
        push edi
        call strcpy
        add esp, 16

        inc dword [count]
        add edi, max_len

    .end_search:
        
        inc ebx
        jmp .loop

    .end_loop:
    sub esp, 8
    push dword [count]
    push format_out
    call printf
    add esp, 16

    mov esp, ebp
    pop ebp
    xor eax, eax
    ret
