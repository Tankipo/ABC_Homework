format PE console                            ; Win32 portable executable console format
entry _start                                 ; _start is the program's entry point

include 'win32a.inc'                         

section '.data' data readable writable       ; data definitions

hello db "Hello World!", 0
stringformat db "%s", 0ah, 0

section '.code' code readable executable     ; code

_start:
        invoke printf, stringformat, hello   ; call printf, defined in msvcrt.dll
        invoke getchar                       ; wait for any key
        invoke ExitProcess, 0                ; exit the process

section '.imports' import data readable      ; data imports

library kernel, 'kernel32.dll',\             ; link to kernel32.dll, msvcrt.dll
        msvcrt, 'msvcrt.dll'

import kernel, \                             ; import ExitProcess from kernel32.dll
       ExitProcess, 'ExitProcess'

import msvcrt, \                             ; import printf and getchar from msvcrt.dll
       printf, 'printf',\
       getchar, '_fgetchar'