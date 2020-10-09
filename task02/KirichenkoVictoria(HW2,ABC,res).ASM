;Кириченко Виктория БПИ198
;Вариант 12
;Сформировать массив B из массива А за исключением последнего отрицтельного элемента этого массива
format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strVecSize   db 'size of vector A? ', 0
        strIncorSize db 'Incorrect size of vector ', 0
        strVecElemI  db '[%d]? ', 0
        strScanInt   db '%d', 0
        strNewVec    db 'VecB =', 10, 0
        strEmptyVec  db 'VecB is empty',0
        strVecElemOut  db '[%d] = %d', 10, 0

        vecA_size    dd 0
        vecB_size    dd 0
        i            dd ?
        tmp          dd ?
        tmp2         dd -1
        tmpStack     dd ?
        vecA         rd 100
        vecB         rd 100

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
; 1) vector input
        call VectorInput
; 2) get vector sum
        call FindNegVector
        call GenerateNewVec
; 3) out of sum
        push strNewVec
        call [printf]
; 4) test vector out
        call VectorOut
finish:
        call [getch]

        push 0
        call [ExitProcess]

;--------------------------------------------------------------------------
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vecA_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vecA_size]
        cmp eax, 0
        jg  getVector
; fail size
        push strIncorSize
        call [printf]
        jmp finish
        push 0
        call [ExitProcess]
; else continue...
getVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA            ; ebx = &vec
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vecA_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8

        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;--------------------------------------------------------------------------
FindNegVector:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA           ; ebx = &vec
        mov esi, vecB
        mov eax,0
FindNegLoop:
        ;mov [tmp], ebx
        cmp ecx, [vecA_size]
        je endFindNegVector      ; to end of loop
        cmp [ebx],eax
        jl WriteI

        inc ecx
        add ebx, 4
        jmp FindNegLoop
WriteI:
        mov [tmp2], ecx
        inc ecx
        add ebx, 4
        jmp FindNegLoop
endFindNegVector:
        ret
GenerateNewVec:
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecA           ; ebx = &vec
        mov esi, vecB
GenerateVecLoop:
        cmp ecx, [vecA_size]
        je endGenerateVec
        cmp ecx,[tmp2]
        je SkipEl
        mov eax,[ebx]
        mov [esi],eax

        inc ecx
        add ebx,4
        add esi,4
        inc [vecB_size]
        jmp GenerateVecLoop
SkipEl:
        inc ecx
        add ebx,4
        jmp GenerateVecLoop
endGenerateVec:
        ret
;--------------------------------------------------------------------------
VectorOut:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
        mov ebx, vecB            ; ebx = &vec
        mov eax,0
putVecLoop:
        mov [tmp], ebx
        cmp [vecB_size],eax
        je PrintEmptyVec
        cmp ecx, [vecB_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret
PrintEmptyVec:
        push strEmptyVec
        call [printf]
        add esp,4
        jmp finish
;-------------------------------third act - including HeapApi--------------------------
                                                 
section '.idata' import data readable
    library kernel, 'kernel32.dll',\
            msvcrt, 'msvcrt.dll',\
            user32,'USER32.DLL'

include 'api\user32.inc'
include 'api\kernel32.inc'
    import kernel,\
           ExitProcess, 'ExitProcess',\
           HeapCreate,'HeapCreate',\
           HeapAlloc,'HeapAlloc'
  include 'api\kernel32.inc'
    import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'