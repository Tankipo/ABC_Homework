format PE console
entry start

include 'win32a.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable

        strInfo   db 'Woodall numbers less 2^32-1',10, 0
        strVecElemI  db '[%u] = ', 0   ; number of index in array
        strScanInt   db '%u',10, 0     ;number of array vec[i](Woodall number)
        strVecElemOut  db '[%u] = %u', 10, 0
        strWoodallNum db 'Max Woodall number = [%u]',10,0
        strWoodallPar db 'Parametr of max Woodall number n = [%u]',0

        vec_size     dd 1
        sum          dd 0
        i            dd 1  ; iterator
        tmp          dd ?  ;current number (vec[i])
        tmp2         dd 1  ;exp of 2
        tmp3         dd 1  ;vec[1] or vec[i-1]
        tmpStack     dd ?
        vec          rd 100  ;array of Woodall numbers

;--------------------------------------------------------------------------
section '.code' code readable executable
start:
; 1) generate array
        call GenerateArray

finish:
        call [getch]

        push 0
        call [ExitProcess]

;--------------------------------------------------------------------------
GenerateArray:
        push strInfo ; info about what numbers presented
        call [printf]
        add esp, 4

getArray:
        mov ecx, 1            ; iterator from 1
        mov ebx, vec          ; array of Woodall numbers
      ;  mov edx, 1            ; ebx = &vec
        mov eax, 1             ;exponentiation of 2
      ;  mov esi, 0

ArrayLoop:
        mov [tmp], ebx  ; move value of arary to tmp
        mov [i], ecx    ; move iterator to i

        push ecx
        push strVecElemI ; ordinal number, print it
        call [printf]
        add esp, 8
        mov eax, [tmp2]  ; taking a number from tmp2 exp of 2 previous number
        imul eax, 2     ;increasing degree
        mov [tmp2], eax
        mov ebx,1        ; vec[i] = 1(for right multiplication)
        imul ebx,[i]     ; formula
        imul ebx,[tmp2]  ; formula
        dec ebx          ; formula , where vec[i] = i*2^i-1

        push ebx         ;print calculated number
        push strScanInt
        call [printf]
        add esp, 8

        cmp [tmp3], ebx   ;if previous number of array < current number of array
        jb PlusOne        ;i++
        cmp [tmp3], ebx
        je PlusOne

        jmp  endGenerateArray ;if not, turn to the end


PlusOne:
        mov [tmp3], ebx    ; remembering vec[i] in tmp3
        mov ecx, [i]       ; jump to nex val of array
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp ArrayLoop     ;continue

endGenerateArray:
        push [tmp3]        ;latest appropriate number in array
        push strWoodallNum ; print it
        call [printf]
        add esp,8
        mov ecx,[i]        ;latest appropriate iterator always i-1
        dec ecx
        push ecx
        push strWoodallPar
        call [printf]
        add esp,8
        ret

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