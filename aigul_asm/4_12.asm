extern io_get_dec, io_print_dec

section .data
b dd 0 ; b stands for boundary points in Pick's theorem

section .bss
x1 resd 1
x2 resd 1
x3 resd 1
y1 resd 1
y2 resd 1
y3 resd 1

section .text
global main
main:
    call io_get_dec
    mov [x1], eax
    call io_get_dec
    mov [y1], eax
    call io_get_dec
    mov [x2], eax
    call io_get_dec
    mov [y2], eax
    call io_get_dec
    mov [x3], eax
    call io_get_dec
    mov [y3], eax
    
    mov ebx, [x1]
    sub ebx, [x2]
    mov ecx, [y1]
    sub ecx, [y2]
    push ebx
    push ecx
    call int_points
    add esp, 8
    add dword[b], eax
    
    mov ebx, [x1]
    sub ebx, [x3]
    mov ecx, [y1]
    sub ecx, [y3]
    push ebx
    push ecx
    call int_points
    add esp, 8
    add dword[b], eax
    
    mov ebx, [x3]
    sub ebx, [x2]
    mov ecx, [y3]
    sub ecx, [y2]
    push ebx
    push ecx
    call int_points
    add esp, 8
    add dword[b], eax
    
    sub dword[b], 3
    
    ; s = 1/2 * |(x2-x1)*(y3-y1) - (x3-x1)*(y2-y1)|
    mov eax, [x2]
    sub eax, [x1]
    mov ebx, [y3]
    sub ebx, [y1]
    imul eax, ebx
    
    mov ebx, [x3]
    sub ebx, [x1]
    mov ecx, [y2]
    sub ecx, [y1]
    imul ebx, ecx
    
    sub eax, ebx
    cmp eax, 0
    jg .skip
    neg eax
    .skip:
    ; now eax = 2s
    
    ; i = (2s - b + 2) / 2
    sub eax, [b]
    add eax, 2
    mov esi, 2
    idiv esi
    call io_print_dec
    
    xor eax, eax
    ret
  
; to find integer boundary points we use formula
; b = 1 + gcd(|x2-x1| , |y2-y1|) 
int_points:
    push ebp
    mov ebp, esp
    mov esi, dword[esp+8]
    mov edi, dword[esp+12]
    
    ; finding absolute values of esi and edi
    cmp esi, 0
    jg .skip1
    neg esi
    .skip1:
    cmp edi, 0
    jg .skip2
    neg edi
    .skip2:
    
    push esi
    push edi
    call gcd
    add esp, 8
    inc eax
    
    mov esp, ebp
    pop ebp
    ret
        
  
gcd:
    push ebp
    mov ebp, esp
    mov esi, dword[ebp+8]
    mov edi, dword[ebp+12]
    
    .while:
        cmp esi, 0
        je .end_while
        cmp edi, 0
        je .end_while
        cmp esi, edi
        jg .greater
        mov eax, edi
        xor edx, edx
        div esi
        mov edi, edx
        jmp .while
        .greater:
        mov eax, esi
        xor edx, edx
        div edi
        mov esi, edx
        jmp .while
    .end_while:
    add esi, edi
    mov eax, esi
    
    mov esp, ebp
    pop ebp
    ret