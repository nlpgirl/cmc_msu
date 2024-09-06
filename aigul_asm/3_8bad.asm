%include "io.inc"

extern io_get_dec, io_print_dec, io_print_char

section .data
    n dd 0          ; number of rows
    m dd 0          ; number of columns
    k dd 0
    matrix1 times 10000 dd 0  ; static allocation of a 100*100-double word matrix (change size as needed)
    matrix2 times 10000 dd 0
    matrix times 10000 dd 0
    
section .text
global CMAIN
CMAIN:
    ;mov ebp, esp; for correct debugging
    ;mov ebp, esp; for correct debugging
    ; read n, m, k
    call io_get_dec
    mov [n], eax
    call io_get_dec
    mov [m], eax
    call io_get_dec
    mov [k], eax
    
    ; read in the matrix1 elements
    mov ebx, 0
    read_loop1:
        cmp ebx, [n]
        je end_read_loop1

        mov ecx, 0
        col_loop1:
            cmp ecx, [m]
            je end_col_loop1

            ; compute the memory offset of the current matrix element
            mov edx, ebx
            mul dword[m]
            add edx, ecx
            shl edx, 2  ; multiply by 4 to get byte offset

            ; read in the value of the current matrix element
            call io_get_dec
            mov dword[matrix1 + edx], eax

            ; increment the column index
            inc ecx
            jmp col_loop1

        end_col_loop1:
            ; increment the row index
            inc ebx
            jmp read_loop1

    end_read_loop1:
    
    ; read in the matrix2 elements
    mov ebx, 0
    read_loop2:
        cmp ebx, [m]
        je end_read_loop2

        mov ecx, 0
        col_loop2:
            cmp ecx, [k]
            je end_col_loop2

            ; compute the memory offset of the current matrix element
            mov edx, ebx ;multiply row index by number of columns
            mul dword[k]
            add edx, ecx ; add column index
            shl edx, 2  ; multiply by 4 to get byte offset

            ; read in the value of the current matrix element
            call io_get_dec
            mov dword[matrix2 + edx], eax

            ; increment the column index
            inc ecx
            jmp col_loop2

        end_col_loop2:
            ; increment the row index
            inc ebx
            jmp read_loop2

    end_read_loop2:

    ; multiply matrix1 and matrix2 and store the result in matrix
    mov ebx, 0
    mul_loop:
        cmp ebx, [n]
        je end_mul_loop

        mov ecx, 0
        mul_col_loop:
            cmp ecx, [k]
            je mul_end_col_loop

            ; compute the memory offset of the current n*k matrix element
            mov edx, ebx ;multiply row index by number of columns
            mul dword[k]
            add edx, ecx ; add column index
            shl edx, 2  ; multiply by 4 to get byte offset

            ; calculate current multiplication
            mov esi, 0
            mov edx, 0
            inner_loop:
                cmp edx, [m]
                je end_inner_loop

                ; compute the memory offset of the current element in matrix1
                mov eax, ebx ;multiply row index by number of columns
                mul dword[m]
                add eax, edx ; add column index
                shl eax, 2  ; multiply by 4 to get byte offset
                
                ; compute the memory offset of the current element in matrix2
                mov ecx, edx ;multiply row index by number of columns
                mul dword[k]
                add ecx, ebx ; add column index
                shl ecx, 2  ; multiply by 4 to get byte offset
    
                ; multiply the current elements and add to the current result
                mov eax, dword[matrix1 + eax]
                mov edx, dword[matrix2 + ecx]
                imul eax, edx
                add esi, eax
    
                ; increment the column index
                inc edx
                jmp inner_loop
    
            end_inner_loop:
                ; store the result in the appropriate element of the result matrix
                mov eax, ebx ;multiply row index by number of columns
                mul dword[k]
                add eax, ecx ; add column index
                shl eax, 2  ; multiply by 4 to get byte offset
                mov dword[matrix + eax], esi
    
            ; increment the column index
            inc ecx
            jmp mul_col_loop
    
        mul_end_col_loop:
            ; increment the row index
            inc ebx
            jmp mul_loop
    
    end_mul_loop:

; print the resulting matrix
mov ebx, 0
print_loop:
    cmp ebx, [n]
    je end_print_loop

    mov ecx, 0
    col_print_loop:
        cmp ecx, [k]
        je end_col_print_loop

        ; compute the memory offset of the current matrix element
        mov edx, ebx
        mul dword[k]
        add edx, ecx
        shl edx, 2  ; multiply by 4 to get byte offset

        ; print the value of the current matrix element
        mov eax, dword[matrix + edx]
        call io_print_dec
        mov eax, 0
        call io_print_char

        ; increment the column index
        inc ecx
        jmp col_print_loop

    end_col_print_loop:
        ; print a newline character to move to the next row
        mov eax, 10
        call io_print_char

        ; increment the row index
        inc ebx
        jmp print_loop

end_print_loop:

; return 0 to indicate successful completion of the program
mov eax, 0
ret
