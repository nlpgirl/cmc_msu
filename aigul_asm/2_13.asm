%include "io.inc"

extern io_get_dec, io_get_char, io_print_dec

section .data
x1 db 0
y1 db 0
x2 db 0
y2 db 0 

section .text
global CMAIN
CMAIN:
    call io_get_char
    mov [x1], al
    call io_get_dec
    mov [y1], al
    
    call io_get_char
    
    call io_get_char
    mov [x2], al
    call io_get_dec
    mov [y2], al
    
    mov al, [x1]
    sub al, [x2]
    
    ;next four instructions write in AL an ansolute value of x1-x2
    mov bl, al       
    sar bl, 7        
    xor al, bl       
    sub al, bl       
    
    ;now in x1 will be stored |x1-x2|
    mov [x1], al 
    
    mov al, [y1]
    sub al, [y2]
    
    ;next four instructions write in AL an ansolute value of y1-y2
    mov bl, al       
    sar bl, 7        
    xor al, bl       
    sub al, bl
    
    add al, [x1]
    call io_print_dec
    
    xor eax, eax
    ret                