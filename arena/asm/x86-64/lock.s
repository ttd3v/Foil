format ELF64
public lock_activate
public lock_deactivate
section '.text' executable

; SysV
; @arg0 -> pointer
; @arg1 -> can loop

; OUTPUT
; 0     -> activated
; -1    -> Failed to activate
;
lock_activate:
mov eax, 0
lock cmpxchg dword [rdi], 0xFFFFFFFF
je .return
test rsi,rsi
jnz lock_activate
pause
mov rax, -1
.return:
ret

; SysV
; @arg0 -> pointer
lock_deactivate:
mov dword [rdi], 0x00000000
ret
