global f1
global f2
global f3

section .text
f1:
    push ebp
    mov ebp, esp
    finit
    fldz
    fld qword[ebp+8]
    fucomi 
    jb .negative
    push 0
    ja .positive
    fld1
    jmp .add_one
    .positive:
        fld1
        fucomip
        ja .under_one
        frndint
        fld qword[ebp+8]
        fsubrp
        f2xm1
        fld1
        faddp
        fld qword[ebp+8]
        frndint
        sub esp, 4
        fistp dword[esp]
        mov ecx, dword[esp]
        .lp:
        fld1
        fld1
        faddp
        fmulp
        loop .lp
        cmp dword[ebp-4], 0
        jne .divide
        jmp .add_one

    .under_one:
        f2xm1
        fld1
        faddp
        cmp dword[ebp-4], 0
        jne .divide
        jmp .add_one

    .negative:
        push 1
        fld qword[ebp+8]
        fchs
        fst qword[ebp+8]
        jmp .positive

    .divide:
        fld1
        fdivrp

    .add_one:
        fld1
        faddp    

    add esp, 4
    leave
    ret

f2:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp+8]
    mov ecx, 4
    .lp:
        fld qword[ebp+8]
        fmulp
        loop .lp
    leave
    ret
    
f3:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp+8]
    fchs
    fld1
    faddp
    fld1
    fld1
    fld1
    faddp
    faddp
    fdivp
    leave
    ret 