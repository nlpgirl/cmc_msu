extern fprintf, fscanf, fopen, fclose, qsort

section .rodata
    fw db "w", 0
    format_in db "%d", 0
    format_out db "%d ", 0
    inf db "input.txt", 0
    outf db "output.txt", 0
    fr db "r", 0

section .bss
    arr resd 1001

section .text
global main
main:
    push ebp
    mov ebp, esp
    and ESP, ~15
    sub ESP, 16

    xor EBX, EBX
    mov dword[ESP], inf
    mov dword[ESP + 4], fr
    call fopen
    mov ESI, EAX
    xor edi, edi
.loop:
    mov dword[ESP], ESI
    mov dword[ESP + 4], format_in
    lea EAX, [ESP + 12] 
    mov dword[ESP + 8], EAX
    call fscanf
    cmp eax, -1
    je .loop_end
    mov eax, dword[ESP + 12]
    lea ecx, [arr+4*edi]
    mov dword[ecx], eax
    inc edi
    jmp .loop
    
.loop_end:
    mov dword[ESP], ESI
    call fclose
    
    push comparator
    push 4
    push edi
    push arr
    call qsort
    add esp, 16 ; clear arguments from stack
    
.end:
    mov dword[ESP], outf
    mov dword[ESP + 4], fw
    call fopen
    mov ESI, EAX
    mov ebx, edi
    xor edi, edi
    mov dword[ESP], ESI
    mov dword[ESP + 4], format_out
.print:
    cmp edi, ebx
    jae .end_print
    lea ecx, [arr+4*edi]
    mov eax, dword[ecx]
    mov dword[esp+8], eax
    inc edi
    call fprintf
    jmp .print
.end_print:   
    mov dword[ESP], ESI
    call fclose
    mov esp, ebp
    pop ebp
    xor EAX, EAX
    ret

comparator:
    push EBP
    mov EBP, ESP
    mov EAX, dword[EBP + 8]
    mov EAX, dword[EAX]
    mov EDX, dword[EBP + 12]
    cmp EAX, dword[EDX]
    jl .LL
    je .LE
    mov EAX, 1
    jmp .end_comparator
.LL:
    mov EAX, -1
    jmp .end_comparator
.LE:
    mov EAX, 0
.end_comparator:
    mov ESP, EBP
    pop EBP
    ret