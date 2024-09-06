%include "io.inc"

extern io_get_dec, io_print_dec, io_print_char, io_newline

section .bss
n resd 1
m resd 1
k resd 1
matrix1 resw 10000
matrix2 resw 10000
matrix resd 10000

section .data
space db ' '

section .text
global CMAIN
CMAIN:

    call io_get_dec
    mov dword[n], eax
    call io_get_dec
    mov dword[m], eax
    call io_get_dec
    mov dword[k], eax
    
    ;multiply n and m
    mov edi, dword[n]
    imul edi, dword[m]
    
    xor ebx, ebx
    
  .input_matrix1:
    cmp ebx, edi
    jnl .end_input_matrix1
    call io_get_dec
    mov word[matrix1 + 2*ebx], ax
    inc ebx
    jmp .input_matrix1
  .end_input_matrix1:
    
    ;multiply m and k
    mov edi, dword[m]
    imul edi, dword[k]
    
    xor ebx, ebx
    
  .input_matrix2:
    cmp ebx, edi
    jnl .end_input_matrix2
    call io_get_dec
    mov word[2*ebx+matrix2], ax
    inc ebx
    jmp .input_matrix2
  .end_input_matrix2:
    
    ;edi = i
    ;esi = j
    xor edi, edi
    xor esi, esi
    
  ;iterate edi = i
  .loop1:          
   cmp edi, dword[n] 
   jnl .end
    
        ;iterate esi = j
        .loop2:               
        cmp esi, dword[k] ;                
        jnl .end_loop2
        xor ebx, ebx 
        ;eax = 4*k*esi
        mov eax, dword[k]          
        imul eax, edi
        sal eax, 2 ;eax = eax*4  
              
            ;iterate ebx = z        
            .loop3:  ;                   
            cmp ebx, dword[m] ;                      
            jnl .end_loop3
            
            mov ecx, dword[m]          
            imul ecx, edi 
            sal ecx, 1 ;ecx = m*i*2
            ;ecx = matrix1[i][z]
            movsx ecx, word[matrix1 + ecx + 2*ebx]
            
            mov edx, dword[k]  
            imul edx, ebx 
            sal edx, 1 ;edx = k*z*2
            ;edx = matrix2[z][j]
            movsx edx, word[matrix2 + edx + 2*esi] 
            imul ecx, edx 
            add dword[matrix + eax + 4*esi], ecx 
            
            inc ebx                                
            jmp .loop3
             
            ;end 3rd loop and print the line
            .end_loop3:
            mov eax, dword[matrix + eax + 4*esi]
            call io_print_dec
            movzx eax, byte[space]
            call io_print_char
            
            inc esi 
            jmp .loop2
        
      .end_loop2:
      
    inc edi       
    mov esi, 0    
    call io_newline
    jmp .loop1
   
   .end:
    xor eax, eax
    ret